#include "ap.h"
#include "stm32f4xx_hal_adc.h"
#include <stdint.h>
#include <stdio.h>  // sprintf 사용을 위해
#include <string.h> // strlen 사용을 위해
#include "lcd.h"

volatile uint16_t adc_val;
double temperature;

#define TEMP30_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFF7A2C))
#define TEMP110_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFF7A2E))
#define VDDA_CALIB 3300 // 보정 시 VDD 전압(mV)
#define VDDA_APPLI 3300


void apInit(void) {
    lcd_init();
}

void apMain(void) {
    char tx_buffer[50];

    while(1){
        // 1. ADC 변환 시작
        HAL_ADC_Start(&hadc1);
        
        // 2. 변환 완료 대기
        if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
            // 3. 값 읽기
            adc_val = HAL_ADC_GetValue(&hadc1);
            
            // 4. 온도 계산 (공식은 STM32F4 기준)
            // Vsense = (ADC_Value * 3.3 / 4095.0)
            // Temperature = ((Vsense - V25) / Avg_Slope) + 25
            float vsense = ((float)adc_val * 3.3 / 4095.0);
            temperature = ((vsense - 0.76) / 0.0025) + 25.0;

            sprintf(tx_buffer, "Temp: %d.%d C   ", (int)temperature, (int)((temperature - (int)temperature) * 10));
        }

        // 6. LCD 출력
        lcd_putcur(0, 0);
        lcd_send_string(tx_buffer);

        // 7. 1초 대기 후 반복
        HAL_Delay(1000);
    }

}