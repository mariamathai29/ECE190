/// S1G7_sensor.h
// Provides user-defined functions to interface with an HCSR04 ultrasonic sensor

// Libraries necessary for user-defined functions
#include <string.h>  // Format data properly before being sent to serial console
#include <stdio.h>  // Enables use of printf() to send messages to serial console

// Header files/libraries ***already implicitly included by STM32CubeIDE***
#include "stm32f4xx_hal.h"   // Defines the registers associated with each GPIO port
#include <stdint.h>  // Provides fixed-width integer types (already included in STM32CubeIDE)

/// Helper Function Declarations
int _write(int file, char *data, int len);
void delay_us(uint16_t us);

/// Core Function Declarations
// Core Function Declaratons
float measure_pulse_duration(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void send_trigger_pulse(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/// Helper Function Definitions

// Override printf() to transmit message through UART
int _write(int file, char *data, int len) {
	HAL_UART_Transmit(&huart2, (uint8_t*)data, len, HAL_MAX_DELAY);
	return len;
}

// Create a delay in microseconds (us)
void delay_us (uint16_t time_us) {
	// Reset Timer 1 Counter to initiate new count
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	// Wait for Timer 1 Counter to reach input time in us
	while(__HAL_TIM_GET_COUNTER(&htim1) < time_us);

	return;
}

/// Core Function Definitions

// Initiate sensor time measurement
void send_trigger_pulse(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	//Reset Trig Pin to ensure clean signal
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
	delay_us(2);  // Buffer time

	// Send trigger signal to Trig Pin
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
	delay_us(10);  // Supply 10 us pulse to start ranging
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

// Measure how long Echo Pin is HIGH after being SET from RESET using Timer 1
float measure_pulse_duration(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	// Time Measurement Variables
	uint32_t start_time = 0.0;
	uint32_t end_time = 0.0;
	uint32_t pulse_duration_ticks = 0.0;
	float pulse_duration_us = 0.0f;

	// Wait for Echo Pin to be SET (rising edge of pulse)
	while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET);

	// Start timing when Echo Pin is SET
	start_time = __HAL_TIM_GET_COUNTER(&htim1);  // Capture current timer value

	// Wait for Echo Pin to be RESET (falling edge of pulse)
	while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET);

	// Capture end time when pin is RESET
	end_time = __HAL_TIM_GET_COUNTER(&htim1);

	// Calculate pulse duration in terms of timer ticks
	if (end_time >= start_time) {
		pulse_duration_ticks = end_time - start_time;
	} else {
		pulse_duration_ticks = (0xFFFFFFFF - start_time) + end_time;  // Handle timer overflow
	}

	// Note: Each timer tick corresponds to 1 microsecond
	// Convert pulse duration from uint32_t to float
	pulse_duration_us = (float)pulse_duration_ticks;  // Cast as float

	return pulse_duration_us;
}