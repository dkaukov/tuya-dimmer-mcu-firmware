#ifndef __CONFIG_H
#define __CONFIG_H

#define DEBOUNCE 2
#define MAX_BUTTONS 3

#define LED_PORT GPIOC
#define LED_1 GPIO_PIN_4
#define LED_2 GPIO_PIN_5
#define LED_3 GPIO_PIN_3
#define PWM_PORT GPIOC
#define PWM GPIO_PIN_7

#define PWM_ON  __asm__("bset 0x500a, #7")
#define PWM_OFF __asm__("bres 0x500a, #7")

#define LED_1_ON  __asm__("bset 0x500a, #4") 
#define LED_1_OFF __asm__("bres 0x500a, #4") 
#define LED_2_ON  __asm__("bset 0x500a, #5")
#define LED_2_OFF __asm__("bres 0x500a, #5")
#define LED_3_ON  __asm__("bset 0x500a, #3")
#define LED_3_OFF __asm__("bres 0x500a, #3")

#define BUTTON_PORT GPIOA
#define BUTTON_1 GPIO_PIN_2
#define BUTTON_2 GPIO_PIN_3
#define BUTTON_3 GPIO_PIN_1
#define BUTTONS ( BUTTON_1 | BUTTON_2 | BUTTON_3 )

#define MISC_PORT GPIOD
#define ESP_GPIO0 GPIO_PIN_4
#define ZERO_X GPIO_PIN_2

#define OFFSET 150

#endif /* __CONFIG_H */
