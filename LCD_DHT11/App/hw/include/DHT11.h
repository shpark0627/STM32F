#ifndef INC_I2C_DHT11_H_
#define INC_I2C_DHT11_H_

#include "stm32f4xx_hal.h"

uint8_t DHT11_Start(void);
uint8_t DHT11_Check_Response(void);
uint8_t DHT11_Read(void);


#endif