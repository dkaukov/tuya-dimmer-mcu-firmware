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

TYPE_BUFFER_S FlashBuffer;
bool storeeprom;

const uint16_t button[MAX_BUTTONS][2] = { {BUTTON_1, LED_1}, {BUTTON_2, LED_2}, {BUTTON_3, LED_3} };
uint8_t button_debounce[MAX_BUTTONS] = {0};
uint32_t button_counter[MAX_BUTTONS] = {0};
bool button_state[MAX_BUTTONS] = {0};
bool last_button_state[MAX_BUTTONS] = {0};
bool input = 0;
uint16_t dim_value = 0;
const uint16_t log_dim_curve[] = {288,288,288,288,289,289,289,289,289,289,290,290,290,290,290,291,291,291,291,292,292,292,292,292,293,293,293,294,294,294,294,295,295,295,296,296,296,297,297,297,298,298,298,299,299,299,300,300,301,301,302,302,302,303,303,304,304,305,305,306,307,307,308,308,309,309,310,311,311,312,313,313,314,315,316,316,317,318,319,320,320,321,322,323,324,325,326,327,328,329,330,331,332,333,334,335,337,338,339,340,342,343,344,346,347,349,350,352,353,355,356,358,360,362,363,365,367,369,371,373,375,377,379,381,383,385,388,390,393,395,397,400,403,405,408,411,414,417,420,423,426,429,432,435,439,442,446,449,453,457,461,465,469,473,477,481,486,490,495,500,504,509,514,519,525,530,536,541,547,553,559,565,571,577,584,590,597,604,611,619,626,634,641,649,657,666,674,683,691,701,710,719,729,739,749,759,769,780,791,802,814,825,837,850,862,875,888,901,915,929,943,958,972,988,1003,1019,1035,1052,1069,1086,1104,1122,1140,1159,1178,1198,1218,1239,1260,1281,1303,1326,1349,1372,1396,1421,1446,1471,1497,1524,1552,1579,1608,1637,1667,1697,1728,1760,1793,1826,1860,1895,1930,1966,2003,2041};

void uart_putchar (char c) {
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);
  UART1_SendData8(c);
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

void delayUs(uint16_t ms){
  uint16_t deadline = micros() + ms;
  while(micros() < deadline);
}

void LedStatusService() {
  if (FlashBuffer.power_switch){
    MCLED_2_OFF;
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

void ButtonInputService(){
  static uint16_t last_micros = 0;
  uint8_t i;
  uint16_t this_micros = micros();
  uint16_t diff = this_micros - last_micros;
  last_micros = this_micros;
  for (i = 0; i < MAX_BUTTONS; ++i){
    input = GPIO_ReadInputData(BUTTON_PORT) & button[i][0];
    if(input != last_button_state[i]){
      button_debounce[i] = 0;
    }
    if(++button_debounce[i] >= DEBOUNCE) {
      if(!input){
        button_counter[i] += diff;
        if(FlashBuffer.power_switch){
          if(button_counter[i] >= (10000)){ // 100 ms
            if((button[i][0] == BUTTON_1) && FlashBuffer.brightness <= 254) {
              FlashBuffer.brightness+=1;
              MCLED_2_RED;
              brightness_update();
            }
            else if((button[i][0] == BUTTON_3) && FlashBuffer.brightness > MIN_BRIGHNESS_VALUE) {
              FlashBuffer.brightness-=1;
              MCLED_2_BLUE;
              brightness_update();
            }
            button_counter[i] = 0;
          }
        }
        else{
          if(button_counter[i] >= (7500000)){ // 15 s
            if((button[i][0] == BUTTON_3)) {
              GPIO_WriteLow(MISC_PORT, ESP_GPIO0);
            }
            button_counter[i] = 0;
          }
        }
      }
      else{
        button_counter[i] = 0;
      }
      
      if(input != button_state[i]){
        button_state[i] = input;
        if(button_state[i]){
          if((button[i][0] == BUTTON_2)){
            FlashBuffer.power_switch = !FlashBuffer.power_switch;
            storeeprom = 1;
            if (FlashBuffer.power_switch) {
              PersistentStateService();
            }
            switch_update();
          }
          MCLED_1_OFF;
          MCLED_3_OFF;
          GPIO_WriteHigh(MISC_PORT, ESP_GPIO0);
        }
      }
    }
    last_button_state[i] = input;
  }
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
  for (i = 0; i < MAX_BUTTONS; ++i){
    last_button_state[i] = button_state[i] = GPIO_ReadInputData(BUTTON_PORT) & button[i][0];
  }  
  LedStatusService();
  enableInterrupts();
}

void loop() {
  wifi_uart_service();
  ButtonInputService();
  delayUs(5000);
  LedStatusService();
  PersistentStateService();
  delayUs(30000);
}

void main() {
  setup();
  while(1) {
    loop();
  }
}
