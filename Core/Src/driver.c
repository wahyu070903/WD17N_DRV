#include "Driver.h"

extern TIM_HandleTypeDef htim2;
volatile uint32_t pulseCount = 0;
volatile uint32_t target_count = 0;
static float microstep_factor = 0;
static uint8_t gen_complete = 1;

void stepperInit(void) {
	HAL_GPIO_WritePin(GPIOA, DRV_ENA, GPIO_PIN_RESET);	// DRV ENABLE = 0 (on)
	HAL_GPIO_WritePin(GPIOA, DRV_SLEEP, GPIO_PIN_SET);  // DRV SLEEP = 1 (wake up)
	HAL_GPIO_WritePin(GPIOA, DRV_RSET, GPIO_PIN_SET);  // DRV RST = 1 (reset)
	HAL_Delay(10);
}
void setMicrostep(uint8_t mod2, uint8_t mod1, uint8_t mod0) {
	HAL_GPIO_WritePin(GPIOA, DRV_MOD2, mod2 ? GPIO_PIN_SET : GPIO_PIN_RESET); // MOD2
	HAL_GPIO_WritePin(GPIOA, DRV_MOD1, mod1 ? GPIO_PIN_SET : GPIO_PIN_RESET); // MOD1
	HAL_GPIO_WritePin(GPIOA, DRV_MOD0, mod0 ? GPIO_PIN_SET : GPIO_PIN_RESET); // MOD0
	microstep_factor = 1;
	if (mod2 == 0 && mod1 == 0 && mod0 == 0) {
	    microstep_factor = 1.0;
	}
	else if (mod2 == 0 && mod1 == 0 && mod0 == 1) {
	    microstep_factor = 2.0;
	}
	else if (mod2 == 0 && mod1 == 1 && mod0 == 0) {
	    microstep_factor = 4.0;
	}
	else if (mod2 == 0 && mod1 == 1 && mod0 == 1) {
	    microstep_factor = 1.0 / 8.0;
	}
	else if (mod2 == 1) {
	    microstep_factor = 1.0 / 32.0;
	}
}
void generateStep(uint8_t stepCount){
	if(gen_complete == 1){
		pulseCount = 0;
		target_count = stepCount;
		HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_1);
		gen_complete = 0;
	}
}
void configureSpeed(float rps){
	float step_per_rev = 360.0 / 1.8;
	step_per_rev = step_per_rev * microstep_factor;
	float freq = 0;
	freq = rps * step_per_rev;
	freq = PRESCALLER_FREQ / freq;
	 __HAL_TIM_SET_AUTORELOAD(&htim2, freq);
	
}

// 1 pulse finish callback
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
    if (htim->Instance == TIM2) {
			pulseCount++;
			if(pulseCount >= target_count){
				HAL_TIM_PWM_Stop_IT(htim, TIM_CHANNEL_1);
				pulseCount = 0;
				gen_complete = 1;
			}
    }
}

