//--- Licence -------------------------------------------------------------
// This is free and unencumbered software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
// 
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// 
// For more information, please refer to <http://unlicense.org/>

#include "stm8s_conf.h"
#include "stm8s_it.h"
#include "globals.h"
#include "globals.h"
#include "config.h"
#include "wifi.h"
#include "bicubic_curve.h"
#include "log_curve.h"
#include <pt.h>
#include <pt-sem.h>

TYPE_BUFFER_S FlashBuffer;
bool storeeprom;

uint16_t dim_value = 0;

static timer_small_t tmBrightnessUpdate;

static struct pt ptPowerButtonController;
static struct pt ptIncButtonController;
static struct pt ptDecButtonController;
static struct pt ptWifiButtonController;
static struct pt ptBrighnessUpdateService;
static struct pt ptMaxBrigtnessButtonController;
static struct pt ptMinBrigtnessButtonController;

void uart_putchar (char c) {
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);
  UART1_SendData8(c);
}

inline uint16_t __interval(uint16_t i_start, uint16_t i_end) {
  return (i_end - i_start);
}

uint8_t timer_expired(struct timer_small *t, uint16_t systick) {
  uint16_t dt = __interval(t->last_systick, systick);
  t->last_systick = systick;
  if (t->elapsed <= dt) {
    t->elapsed = t->interval;
    return 1;
  } ;
  t->elapsed -= dt;
  return 0;
}

uint8_t timer_expired_big(timer_big_t *t, uint16_t systick) {
  uint16_t dt = __interval(t->last_systick, systick);
  t->last_systick = systick;
  if (t->elapsed < dt) {
    t->elapsed = t->interval;
    return 1;
  } ;
  t->elapsed -= dt;
  return 0;
}

void timer_reset(timer_small_t *t, uint16_t elapsed, uint16_t systick) {
  t->elapsed = elapsed;
  t->last_systick = systick;
}

void timer_reset_big(timer_big_t *t, uint32_t elapsed, uint16_t systick) {
  t->elapsed = elapsed;
  t->last_systick = systick;
}

void GPIO_Config(){
  GPIO_Init(PWM_PORT, PWM, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(LED_PORT, LED_1 | LED_2 | LED_3, GPIO_MODE_OUT_OD_LOW_SLOW);
  GPIO_Init(BUTTON_PORT, BUTTON_1 | BUTTON_2 | BUTTON_3, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(MISC_PORT, ESP_GPIO0, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(MISC_PORT, ZERO_X, GPIO_MODE_IN_FL_IT);
}

void CLK_Config(){
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

void EEPROM_Config(){
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);
}

void EEPROM_read(uint16_t addr, uint8_t *buf, uint16_t len) {
  uint16_t i;
  for (i = 0; i < len; i++, addr++)
    buf[i] = (*(PointerAttr uint8_t *) (MemoryAddressCast)addr);
}

void EEPROM_write(uint16_t addr, uint8_t *buf, uint16_t len) {
  uint16_t i;
  for (i = 0; i < len; i++, addr++) {
    if ((*(PointerAttr uint8_t *) (MemoryAddressCast)addr) != buf[i]) {
      (*(PointerAttr uint8_t *) (MemoryAddressCast)addr) = buf[i];
      while (FLASH_GetFlagStatus(FLASH_IAPSR_EOP) == RESET);
    }
  }
}

void EEPROMLoadState() {
  EEPROM_read(FLASH_DATA_START_PHYSICAL_ADDRESS, (uint8_t *)&FlashBuffer, sizeof(FlashBuffer));
}

void EEPROMSaveState() {
  EEPROM_write(FLASH_DATA_START_PHYSICAL_ADDRESS, (uint8_t *)&FlashBuffer, sizeof(FlashBuffer));
}

void UART_Config(){
  UART1_DeInit();
  UART1_Init((uint32_t)9600, UART1_WORDLENGTH_8D,UART1_STOPBITS_1, UART1_PARITY_NO,
    UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
}

void TIM1_Config(){
  TIM1_DeInit();
  TIM1_TimeBaseInit(15, TIM1_COUNTERMODE_UP, 0xFFFF, 0);
  TIM1_Cmd(ENABLE);
}

void EXTI_Config(){ 
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_RISE_ONLY);
}

void ITC_Config() {
  ITC_SetSoftwarePriority(ITC_IRQ_PORTD, ITC_PRIORITYLEVEL_1);
  ITC_SetSoftwarePriority(ITC_IRQ_TIM1_CAPCOM, ITC_PRIORITYLEVEL_3);
}

uint16_t micros(){
  uint16_t tmp;
  disableInterrupts();
  tmp = TIM1_GetCounter();
  enableInterrupts();
  return tmp;
}

void LedStatusService() {
  if (FlashBuffer.power_switch){
    if (tmBrightnessUpdate.elapsed == 0) {
      MCLED_2_OFF;
    }
    if (FlashBuffer.brightness > MIN_BRIGHNESS_VALUE + 1) {
      MCLED_3_BLUE;
    } else {
      MCLED_3_RED;
    }
    if (FlashBuffer.brightness < 254 - 1) {
      MCLED_1_BLUE;
    } else {
      MCLED_1_RED;
    }
  }
  else {
    MCLED_1_OFF;
    MCLED_2_BLUE;
    MCLED_3_OFF;
  }
}

void PersistentStateService() {
  if(storeeprom && (dim_value <= (1 << FADE_SPEED))) {
    EEPROMSaveState();
    storeeprom = 0;
  }
}

static PT_THREAD(brighnessUpdateService(struct pt *pt, uint16_t dt)) {
  PT_BEGIN(pt);
  PT_WAIT_UNTIL(pt, !timer_expired(&tmBrightnessUpdate, dt));
  PT_WAIT_UNTIL(pt, timer_expired(&tmBrightnessUpdate, dt));
  brightness_update();
  PT_END(pt);
 }

static PT_THREAD(powerButtonController(struct pt *pt, uint16_t dt)) {
  static timer_small_t tm;
  uint8_t expired = 0;
  PT_BEGIN(pt);
  PT_WAIT_UNTIL(pt, !(GPIO_ReadInputData(BUTTON_PORT) & BUTTON_2));
  timer_reset(&tm, 5000, dt);
  PT_WAIT_UNTIL(pt, (GPIO_ReadInputData(BUTTON_PORT) & BUTTON_2) || (expired = timer_expired(&tm, dt)));
  if (!expired) {
    PT_EXIT(pt);
  }
  FlashBuffer.power_switch = !FlashBuffer.power_switch;
  storeeprom = 1;
  if (FlashBuffer.power_switch) {
    PersistentStateService();
  }
  switch_update();
  PT_WAIT_UNTIL(pt, (GPIO_ReadInputData(BUTTON_PORT) & BUTTON_2));
  timer_reset(&tm, 50000, dt);
  PT_WAIT_UNTIL(pt, timer_expired(&tm, dt));
  PT_END(pt);
}

static PT_THREAD(incButtonController(struct pt *pt, uint16_t dt)) {
  static timer_small_t tm;
  PT_BEGIN(pt);
  PT_WAIT_UNTIL(pt, !(GPIO_ReadInputData(BUTTON_PORT) & BUTTON_1) && FlashBuffer.power_switch && (FlashBuffer.brightness <= 254));
  FlashBuffer.brightness+=1;
  MCLED_2_RED;
  MCLED_1_OFF;
  timer_reset(&tmBrightnessUpdate, 20000, dt);
  timer_reset(&tm, 10000, dt);
  PT_WAIT_UNTIL(pt, timer_expired(&tm, dt));
  PT_END(pt);
}

static PT_THREAD(decButtonController(struct pt *pt, uint16_t dt)) {
  static timer_small_t tm;
  PT_BEGIN(pt);
  PT_WAIT_UNTIL(pt, !(GPIO_ReadInputData(BUTTON_PORT) & BUTTON_3) && FlashBuffer.power_switch && (FlashBuffer.brightness > MIN_BRIGHNESS_VALUE));
  FlashBuffer.brightness-=1;
  MCLED_2_BLUE;
  MCLED_3_OFF;
  timer_reset(&tmBrightnessUpdate, 20000, dt);
  timer_reset(&tm, 10000, dt);
  PT_WAIT_UNTIL(pt, timer_expired(&tm, dt));
  PT_END(pt);
}

static PT_THREAD(wifiButtonController(struct pt *pt, uint16_t dt)) {
  static timer_big_t tm;
  uint8_t expired = 0;
  PT_BEGIN(pt);
  PT_WAIT_UNTIL(pt, !(GPIO_ReadInputData(BUTTON_PORT) & BUTTON_3) && !FlashBuffer.power_switch);
  timer_reset_big(&tm, 15000000, dt);
  PT_WAIT_UNTIL(pt, (GPIO_ReadInputData(BUTTON_PORT) & BUTTON_3) || (expired = timer_expired_big(&tm, dt)));
  if (!expired) {
    PT_EXIT(pt);
  }
  GPIO_WriteLow(MISC_PORT, ESP_GPIO0);
  PT_WAIT_UNTIL(pt, (GPIO_ReadInputData(BUTTON_PORT) & BUTTON_3));
  GPIO_WriteHigh(MISC_PORT, ESP_GPIO0);
  PT_END(pt);
}

static PT_THREAD(maxBrigtnessButtonController(struct pt *pt, uint16_t dt)) {
  static timer_big_t tm;
  uint8_t expired = 0;
  PT_BEGIN(pt);
  PT_WAIT_UNTIL(pt, !(GPIO_ReadInputData(BUTTON_PORT) & BUTTON_1) && !FlashBuffer.power_switch);
  timer_reset_big(&tm, 1000000, dt);
  PT_WAIT_UNTIL(pt, (GPIO_ReadInputData(BUTTON_PORT) & BUTTON_1) || (expired = timer_expired_big(&tm, dt)));
  if (!expired) {
    PT_EXIT(pt);
  }
  FlashBuffer.power_switch = 1;
  EEPROMSaveState();
  switch_update();
  timer_reset_big(&tm, 100000, dt);
  PT_WAIT_UNTIL(pt, timer_expired_big(&tm, dt));
  FlashBuffer.brightness = 255;
  //brightness_update();
  //PT_WAIT_UNTIL(pt, (GPIO_ReadInputData(BUTTON_PORT) & BUTTON_1));
  PT_END(pt);
}

static PT_THREAD(minBrigtnessButtonController(struct pt *pt, uint16_t dt)) {
  static timer_big_t tm;
  uint8_t expired = 0;
  PT_BEGIN(pt);
  PT_WAIT_UNTIL(pt, !(GPIO_ReadInputData(BUTTON_PORT) & BUTTON_3) && !FlashBuffer.power_switch);
  timer_reset_big(&tm, 1000000, dt);
  PT_WAIT_UNTIL(pt, (GPIO_ReadInputData(BUTTON_PORT) & BUTTON_3) || (expired = timer_expired_big(&tm, dt)));
  if (!expired) {
    PT_EXIT(pt);
  }
  FlashBuffer.power_switch = 1;
  EEPROMSaveState();
  switch_update();
  timer_reset_big(&tm, 100000, dt);
  PT_WAIT_UNTIL(pt, timer_expired_big(&tm, dt));
  FlashBuffer.brightness = MIN_BRIGHNESS_VALUE;
  //brightness_update();
  //PT_WAIT_UNTIL(pt, (GPIO_ReadInputData(BUTTON_PORT) & BUTTON_3));
  PT_END(pt);
}

void setup() {
  uint8_t i;
  CLK_Config(); 
  GPIO_Config();
  EEPROM_Config();
  UART_Config();
  TIM1_Config();
  EXTI_Config();
  ITC_Config();
  wifi_protocol_init();
  EEPROMLoadState();
  LedStatusService();
  enableInterrupts();
  PT_INIT(&ptPowerButtonController);
  PT_INIT(&ptIncButtonController);
  PT_INIT(&ptDecButtonController);
  PT_INIT(&ptWifiButtonController);
  PT_INIT(&ptBrighnessUpdateService);
  PT_INIT(&ptMaxBrigtnessButtonController);
  PT_INIT(&ptMinBrigtnessButtonController);
}

void loop() {
  uint16_t current_tick = micros();
  wifi_uart_service();
  LedStatusService();
  PersistentStateService();
  PT_SCHEDULE(powerButtonController(&ptPowerButtonController, current_tick));
  PT_SCHEDULE(incButtonController(&ptIncButtonController, current_tick));
  PT_SCHEDULE(decButtonController(&ptDecButtonController, current_tick));
  PT_SCHEDULE(wifiButtonController(&ptWifiButtonController, current_tick));
  PT_SCHEDULE(brighnessUpdateService(&ptBrighnessUpdateService, current_tick));
  PT_SCHEDULE(maxBrigtnessButtonController(&ptMaxBrigtnessButtonController, current_tick));
  PT_SCHEDULE(minBrigtnessButtonController(&ptMinBrigtnessButtonController, current_tick));
}

void main() {
  setup();
  while(1) {
    loop();
  }
}
