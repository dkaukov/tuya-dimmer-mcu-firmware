#include "globals.h"
#include "wifi.h"


const DOWNLOAD_CMD_S download_cmd[] =
{
  {DPID_SWITCH, DP_TYPE_BOOL},
  {DPID_BRIGHTNESS, DP_TYPE_VALUE},
  {DPID_FREQUENCY, DP_TYPE_VALUE},
};

void uart_transmit_output(unsigned char value)
{
  //extern void uart_putchar(unsigned char value);
  uart_putchar(value);

}
void all_data_update(void)
{
  mcu_dp_bool_update(DPID_SWITCH,FlashBuffer.power_switch);
  mcu_dp_value_update(DPID_BRIGHTNESS,FlashBuffer.brightness);
}

void switch_update(void)
{
  mcu_dp_bool_update(DPID_SWITCH,FlashBuffer.power_switch);
}

void brightness_update(void)
{
  mcu_dp_value_update(DPID_BRIGHTNESS,FlashBuffer.brightness);
}

static unsigned char dp_download_switch_handle(const unsigned char value[], unsigned short length)
{
  //unsigned char ret;
  unsigned char power_switch;
  
  power_switch = mcu_get_dp_download_bool(value,length);
  
  FlashBuffer.power_switch = power_switch;

  storeeprom = 1;
  return SUCCESS;
  
/*
  ret = mcu_dp_bool_update(DPID_SWITCH,power_switch);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
*/
}

static unsigned char dp_download_brightness_handle(const unsigned char value[], unsigned short length)
{
  //unsigned char ret;
  unsigned int  brightness;
  
  brightness = mcu_get_dp_download_value(value,length);

  /*
  if(brightness < FlashBuffer.brightness){
    LED_PORT->ODR ^= LED_3;
    Delay(0x3FF);
    LED_PORT->ODR ^= LED_3;
  }
  else if(brightness > FlashBuffer.brightness){
    LED_PORT->ODR ^= LED_1;
    Delay(0x3FF);
    LED_PORT->ODR ^= LED_1;
  }
  */

  FlashBuffer.brightness = brightness;

  return SUCCESS;

/*
  ret = mcu_dp_value_update(DPID_BRIGHTNESS,brightness);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
*/
}

#ifdef SUPPORT_MCU_RTC_CHECK
void mcu_write_rtctime(unsigned char time[])
{
  if(time[0] == 1)
  {
	 
  }
  else
  {
  }
}
#endif

#ifdef WIFI_TEST_ENABLE
void wifi_test_result(unsigned char result,unsigned char rssi)
{
  extern unsigned char wifi_test_mode;

  if(result == 0)
  {
    if(rssi == 0x00)
    {
      LED_RGB_Control(255,0,0);
    }
    else if(rssi == 0x01)
    {
      LED_RGB_Control(0,0,255);
    }
  }
  else
  {
    if(rssi >= 30)
      LED_RGB_Control(0,255,0);
    else
      LED_RGB_Control(255,0,0);
    wifi_test_mode = 0;
  }
  
}
#endif

#ifdef SUPPORT_MCU_FIRM_UPDATE
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length)
{
  unsigned long addr;
 
  if(length == 0)
  {
#ifdef ENABLE_BOOT
    FlashBuffer.magic_code = FIREWARE_UPDATE_FLAG;
    
    if(Earse_Flash(PARA_ADDR) == ERROR)
      return ERROR;
    
    if(Write_Flash(PARA_ADDR,(unsigned char *)&FlashBuffer,sizeof(FlashBuffer)) == ERROR)
      return ERROR;
    
    Reset();
#endif
  }
  else
  {
#ifdef ENABLE_BOOT
    addr = FIREWARE_ADDR_H;
     
    if(position % 1024 == 0)
    {
      if(Earse_Flash(addr + position) == ERROR)
        return ERROR;
    }
    
    if(Write_Flash(addr + position,(unsigned char *)value,length) == ERROR)
      return ERROR;
#endif
  }

  return SUCCESS;
}
#endif

unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length)
{
  unsigned char ret;
  switch(dpid)
  {
    case DPID_SWITCH:
      ret = dp_download_switch_handle(value,length);
      break;
    case DPID_BRIGHTNESS:
      ret = dp_download_brightness_handle(value,length);
      break;

  default:
    break;
  }
  return ret;
}

unsigned char get_download_cmd_total(void)
{
  return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}
