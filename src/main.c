#include "stm8s_conf.h"
#include "stm8s_it.h"
#include "globals.h"
#include "globals.h"
#include "config.h"
#include "wifi.h"

TYPE_BUFFER_S FlashBuffer;
bool storeeprom;

uint16_t button[MAX_BUTTONS][2] = { {BUTTON_1, LED_1}, {BUTTON_2, LED_2}, {BUTTON_3, LED_3} };
uint8_t button_debounce[MAX_BUTTONS] = {0};
uint32_t button_counter[MAX_BUTTONS] = {0};
bool button_state[MAX_BUTTONS] = {0};
bool last_button_state[MAX_BUTTONS] = {0};
bool input = 0;
uint16_t dim_value = 0;

void uart_putchar (char c) {
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);
  UART1_SendData8(c);
}

void GPIO_Config(){
  GPIO_Init(PWM_PORT, PWM, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(LED_PORT, LED_1 | LED_2 | LED_3, GPIO_MODE_OUT_PP_LOW_SLOW);
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
    (*(PointerAttr uint8_t *) (MemoryAddressCast)addr) = buf[i];
    while (FLASH_GetFlagStatus(FLASH_IAPSR_EOP) == RESET);
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
    LED_2_ON;
  }
  else {
    LED_2_OFF;
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
              disableInterrupts();
              LED_PORT->ODR ^= button[i][1];
              enableInterrupts();
              brightness_update();
            }
            else if((button[i][0] == BUTTON_3) && FlashBuffer.brightness > MIN_BRIGHNESS_VALUE) {
              FlashBuffer.brightness-=1;
              disableInterrupts();
              LED_PORT->ODR ^= button[i][1];
              enableInterrupts();
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
          LED_1_OFF;
          LED_3_OFF;
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
  LedStatusService();
  PersistentStateService();
  delayUs(10000);
}

void main() {
  setup();
  while(1) {
    loop();
  }
}
