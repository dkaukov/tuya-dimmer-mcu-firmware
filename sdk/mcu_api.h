/****************************************Copyright (c)*************************
**                                 (C), 2015-2020
**
**                                 http://www.tuya.com
**
******************************************************************************/
#ifndef __MCU_API_H_
#define __MCU_API_H_


#ifdef MCU_API_GLOBAL
  #define MCU_API_EXTERN
#else
  #define MCU_API_EXTERN   extern
#endif

unsigned char hex_to_bcd(unsigned char Value_H,unsigned char Value_L);
unsigned long my_strlen(unsigned char *str);
void *my_memset(void *src,unsigned char ch,unsigned short count);
void *my_memcpy(void *dest, const void *src, unsigned short count);
void int_to_byte(unsigned long number,unsigned char value[4]);
unsigned long byte_to_int(const unsigned char value[4]);

#ifndef WIFI_CONTROL_SELF_MODE
unsigned char mcu_get_reset_wifi_flag(void);
void mcu_reset_wifi(void);
unsigned char mcu_get_wifimode_flag(void);
void mcu_set_wifi_mode(unsigned char mode);
unsigned char mcu_get_wifi_work_state(void);
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
void mcu_get_system_time(void);
#endif

#ifdef WIFI_TEST_ENABLE
void mcu_start_wifitest(void);
#endif
unsigned char mcu_dp_raw_update(unsigned char dpid,const unsigned char value[],unsigned short len);
unsigned char mcu_dp_bool_update(unsigned char dpid,unsigned char value);
unsigned char mcu_dp_value_update(unsigned char dpid,unsigned long value);
unsigned char mcu_dp_string_update(unsigned char dpid,const unsigned char value[],unsigned short len);
unsigned char mcu_dp_enum_update(unsigned char dpid,unsigned char value);
unsigned char mcu_dp_fault_update(unsigned char dpid,unsigned long value);
unsigned char mcu_get_dp_download_bool(const unsigned char value[],unsigned short len);
unsigned long mcu_get_dp_download_value(const unsigned char value[],unsigned short len);
unsigned char mcu_get_dp_download_enum(const unsigned char value[],unsigned short len);
void uart_receive_input(unsigned char value);
void wifi_uart_service(void);
void wifi_protocol_init(void);
void uart_transmit_output(unsigned char value);

#endif
