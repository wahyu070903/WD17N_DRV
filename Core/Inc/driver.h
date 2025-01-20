#ifndef HARDWARE_DRIVER
#define HARDWARE_DRIVER
#include "stm32f1xx_hal.h"

	#define DRV_STEP 		GPIO_PIN_0
	#define DRV_DIR			GPIO_PIN_1
	#define DRV_MOD0		GPIO_PIN_2
	#define DRV_MOD1		GPIO_PIN_3
	#define DRV_MOD2		GPIO_PIN_4
	#define DRV_ENA			GPIO_PIN_5
	#define DRV_FAULT		GPIO_PIN_6
	#define DRV_SLEEP		GPIO_PIN_7
	#define DRV_RSET		GPIO_PIN_8

	#define STEP_ANGLE 1.8
	#define MIN_FREQ 0			// 0Hz
	#define MAX_FREQ 18000		// 18Khz
	#define PRESCALLER_FREQ 1000000	//1Mhz

	void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);
	void stepperInit(void);
	void setMicrostep(uint8_t, uint8_t, uint8_t);
	void configureSpeed(float);
	void generateStep(uint8_t);

#endif
