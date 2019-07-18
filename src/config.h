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

#define ZERO_CROSSING_DELAY_US 150
#define FADE_SPEED 3
#define MIN_BRIGHNESS_VALUE 30

#endif /* __CONFIG_H */
