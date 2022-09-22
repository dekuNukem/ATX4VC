#include "stm32f0xx_hal.h"
#include "ds18b20.h"
#include "my_1wire.h"
#include "delay_us.h"

#define SPAD_SIZE 9

uint8_t ds18b20_buf[SPAD_SIZE];

void ds18b20_start_conversion(void)
{
  __disable_irq();
  my_1wire_reset();
  my_1wire_write_byte(0xcc);
  my_1wire_write_byte(0x44);
  __enable_irq();
}

int16_t ds18b20_get_temp(void)
{
  __disable_irq();
  my_1wire_reset();
  my_1wire_write_byte(0xcc);
  my_1wire_write_byte(0xbe);
  my_1wire_read_bytes(ds18b20_buf, SPAD_SIZE);
  __enable_irq();
  return ds18b20_buf[0] | ds18b20_buf[1] << 8;
}

