#include "DHT11.h"
#include "stm32f4xx_hal_tim.h"

extern TIM_HandleTypeDef htim11;

void delay_us(uint16_t us) {
    __HAL_TIM_SET_COUNTER(&htim11, 0);  // 타이머 카운터 0으로 초기화
    while (__HAL_TIM_GET_COUNTER(&htim11) < us); // 설정한 us만큼 대기
}

uint8_t DHT11_Start(void){
    uint8_t Response = 0;
    // 1. GPIO를 출력으로 설정
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 2. 버스를 Low로 18ms 이상 유지
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
    HAL_Delay(18);
    
    // 3. 버스를 High로 올리고 입력 모드로 전환
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);
    delay_us(20);
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 4. 센서 응답 대기 (80us Low -> 80us High)
    delay_us(40);
    if (!(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1))) {
        delay_us(80);
        if ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1))) Response = 1;
        else Response = -1;
    }
    while ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1))); // High 상태가 끝날 때까지 대기

    return Response;
}

uint8_t DHT11_Check_Response(void){
    uint8_t Response = 0;
    delay_us(40);
    if(!(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1))){
        delay_us(40);
        if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)) Response = 1;
        Response = -1;
    }
    while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)));

    return Response;
}

uint8_t DHT11_Read(void){
    uint8_t i = 0, j;
    for(j = 0; j < 8; j++){
        while (!(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)));
        delay_us(40);
        if(!(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1))){
            i &= ~(1<<(7-j));
        }
        else{
            i |= (1 << (7-j));
        }
        while ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)));
    }
    return i;

}
