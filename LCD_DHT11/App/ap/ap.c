#include "ap.h"

void apInit(void) {
    lcd_init();
    HAL_TIM_Base_Start(&htim11);
    HAL_Delay(100);
}

void apMain(void) {
volatile float Temperature, Humidity;
uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2, CheckSum;
char T_buffer[20];
char H_buffer[20];

while (1) {

    if (DHT11_Start()) {
        Rh_byte1 = DHT11_Read();
        Rh_byte2 = DHT11_Read();
        Temp_byte1 = DHT11_Read();
        Temp_byte2 = DHT11_Read();
        CheckSum = DHT11_Read();

        if (Rh_byte1 + Rh_byte2 + Temp_byte1 + Temp_byte2 == CheckSum) {
            Humidity = Rh_byte1 + Rh_byte2;
            Temperature = Temp_byte1 + Temp_byte2;
        }
    }
    lcd_putcur(0, 0);
    sprintf(T_buffer, "T:%d.%d ",(int)Temperature, (int)((Temperature - (int)Temperature) * 10));
    lcd_send_string(T_buffer);
    sprintf(H_buffer, "H:%d.%d ",(int)Humidity, (int)((Humidity - (int)Humidity)*10));
    lcd_send_string(H_buffer);
    HAL_Delay(2000);

}
}