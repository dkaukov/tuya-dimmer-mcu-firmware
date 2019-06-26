#ifndef __GLOBALS_H
#define __GLOBALS_H

#include  <stdlib.h>
#include "stm8s_conf.h"

typedef struct {
  uint32_t magic_code;
  uint8_t power_switch;
  uint8_t brightness;
} TYPE_BUFFER_S;

extern TYPE_BUFFER_S FlashBuffer;
extern bool storeeprom;

extern void uart_putchar (char c);

#endif /* __GLOBALS_H */
