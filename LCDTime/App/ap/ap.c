#include "ap.h"
#include "stm32f4xx_hal_rtc.h"
#include <stdint.h>
#include <stdio.h>  // sprintf 사용을 위해
#include <string.h> // strlen 사용을 위해

extern I2C_HandleTypeDef hi2c1;
extern RTC_HandleTypeDef hrtc;

#define LCD_ADDR (0x27<<1)

RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef sDate = {0};
char timeStr[16];
char dateStr[16];

void lcd_send_cmd(char cmd){
    char dataUpper, dataLower;
    uint8_t dataCmd[4];

    dataUpper = (cmd & 0xF0);
    dataLower = ((cmd<<4) & 0xF0);
    dataCmd[0] = dataUpper | 0x0C;
    dataCmd[1] = dataUpper | 0x08;
    dataCmd[2] = dataLower | 0x0C;
    dataCmd[3] = dataLower | 0x08;

    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, (uint8_t *)dataCmd, 4, 100);
}

void lcd_send_data(char data){
    char dataUpper, dataLower;
    uint8_t dataCmd[4];

    dataUpper = (data & 0xF0);
    dataLower = ((data<<4) & 0xF0);
    dataCmd[0] = dataUpper | 0x0D;
    dataCmd[1] = dataUpper | 0x09;
    dataCmd[2] = dataLower | 0x0D;
    dataCmd[3] = dataLower | 0x09;

    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, (uint8_t *)dataCmd, 4, 100);
}

void lcd_init(void){

    HAL_Delay(50);

    lcd_send_cmd(0x30);
    HAL_Delay(5);
    lcd_send_cmd(0x30);
    HAL_Delay(1);
    lcd_send_cmd(0x30);
    HAL_Delay(10);
    lcd_send_cmd(0x20);
    HAL_Delay(10);

    lcd_send_cmd(0x28);
    HAL_Delay(1);
    lcd_send_cmd(0x08); // Display off
    HAL_Delay(1);
    lcd_send_cmd(0x01); // Clear display
    HAL_Delay(3); // Clear는 시간이 좀 걸림
    lcd_send_cmd(0x06); // Entry mode set
    HAL_Delay(1);
    lcd_send_cmd(0x0C); // Display on, Cursor off
}

void lcd_send_string(char* str){
    while(*str) lcd_send_data(*str++);
}

void LCD_PutCur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }
    lcd_send_cmd(col);
}

void LCD_Clear (void)
{
	lcd_send_cmd(0x01); // 전체 지우기
	HAL_Delay(2);
}

void apInit(void) {
    lcd_init(); 
}

void apMain(void) {
    
     while(1){
        
        // lcd_send_string("STM Clock");

        HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
        HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

        sprintf(dateStr, "%04d-%02d-%02d", 2000 + sDate.Year, sDate.Month, sDate.Date);
        sprintf(timeStr, "%02d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);

        // LCD에 날짜/시간 출력
 
        LCD_PutCur(0,0);
        lcd_send_string(dateStr);
        LCD_PutCur(1,0);
        lcd_send_string(timeStr);

        HAL_Delay(500);

    }
}