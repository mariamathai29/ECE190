///S1G7_buzzer.h
// Provides user-defined functions to interface with a passive piezoelectric buzzer

/// Clock and Timer Configuration
// 1) Choose a pin to control the buzzer with PWM
// 2) Check the pinout diagram to see which Timer and Channel the pin is tied to
// 3) Find timer clock frequency by referring to the MCU block diagram in data sheet
//    - the timer's associated APB clock frequency will be its clock frequency
// 4) In Mode, enable Clock Source as Internal Clock
// 5) In Mode, enable Channel Y with PWM Generation CHY
// 6) In Configuration, set Counter Period (ARR) to 1000 and Pulse to 500
//    - Duty Cycle = ARR/Pulse => 50%

/// Core Function Declarations
uint16_t prescaler_for_frequency(uint32_t frequency_output_Hz);
void pause_buzzer(void);

/// Main (outside of while loop)
// Start Timer X on Channel Y in PWM mode after everything else has been initialized
// NOTE: Make sure timer # and channel # are consistent with chosen pin's PWM config,
// which is specified on the pinout diagram (ex. PWM 2/1 implies Timer 2, Channel 1)
HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

/// Change frequency by changing prescaler according to calculation based on desired frequency
__HAL_TIM_SET_PRESCALER( &htim2, prescaler_for_frequency(1000) );
HAL_Delay(250);

/// Core Function Definitions

// Calculate the prescaler value depending on the desired frequency
uint16_t prescaler_for_frequency(uint32_t output_frequency_Hz) {
	if ( output_frequency_Hz == 0 ) {
		return 0;
	} else {
		// Note: "- 1" is to compensate for 1 being automatically added in the prescaler register
		return ( ( TIM2_FREQUENCY / (1000 * output_frequency_Hz) ) - 1 );
	}
}

// Pause the buzzer noise indefinitely
void pause_buzzer(void) {
	__HAL_TIM_SET_PRESCALER(&htim2, 0);
}

