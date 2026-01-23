#ifndef AP_H_
#define AP_H_

#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "ap_def.h"
#include <stdint.h>
#include <stdio.h>  // sprintf 사용을 위해
#include <string.h> // strlen 사용을 위해
#include "lcd.h"
#include "stm32f4xx_hal_tim.h"
#include "tim.h"
#include "DHT11.h"

void apInit(void);
void apMain(void);

#endif