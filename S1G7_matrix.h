/// S1G7_matrix.h
// Provides user-defined functions to interface with a 1088BS 8x8 LED Dot Matrix and 74HC595 Shift Registers

/// Notes
// 1) Configure a specific timer for the delay function
// 2) Use if statements to display a pattern on the matrix at one time
//    - Can't do a blinking pattern without for loops, so default to blank matrix

/// Helper Function Declarations
void delay_us(uint16_t us);

/// Core Function Declarations
void store(void);
void reset_display(void);
void shift_out(GPIO_TypeDef *data_port, uint16_t data_pin,
        	   GPIO_TypeDef *shift_port, uint16_t shift_pin,
               uint8_t bit_order, uint8_t value);
void display_pattern(uint8_t * pattern_array);

/// Helper Function Definitions

// Create a delay in microseconds (us)
void delay_us (uint16_t time_us) {
	// Reset Timer 1 Counter to initiate new count
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	// Wait for Timer 1 Counter to reach input time in us
	while(__HAL_TIM_GET_COUNTER(&htim1) < time_us);

	return;
}

/// Core Function Definitions

// "Print" to the matrix by copying the shift register data to the storage register
void store(void) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, SET);
	delay_us(1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, RESET)

	return;
}

// Turn off all the lights by printing nothing
void reset_display(void) {
	shift_out(GPIOB, GPIO_PIN_10,
		      GPIOA, GPIO_PIN_8,
			  1, 0b11111111);
	shift_out(GPIOB, GPIO_PIN_10,
			  GPIOA, GPIO_PIN_8,
			  1, 0b00000000);
	store();
}

// Shift out a byte of data one bit at a time
void shift_out(GPIO_TypeDef *data_port, uint16_t data_pin,
        	   GPIO_TypeDef *shift_port, uint16_t shift_pin,
               uint8_t bit_order, uint8_t value) {
	//Ensure data type of iterable is size_t to prevent overflow
	for ( size_t i = 0; i < 8; ++i ) {
		// LSBFIRST (shift least significant bit first)
		if ( bit_order == 0 ) {
			/// Check if the value in the i-th bit is 1
			//  - Use bitwise & to mask the i-th bit and check if the result is non-zero
			if ( value & (1 << i) ) {
				// Write a 1 to the serial data pin
				HAL_GPIO_WritePin(data_port, data_pin, SET);
			// i-th bit is 0
			} else {
				// Write a 0 to the serial data pin
				HAL_GPIO_WritePin(data_port, data_pin, RESET);
			}
		// MSBFIRST (shift most significant bit first))
		} else if ( bit_order == 1 ) {
			// Instead  of checking from the right, check from left to right
			if ( value & ( 1 << (7 - i) ) ) {
				HAL_GPIO_WritePin(data_port, data_pin, SET);
			} else {
				HAL_GPIO_WritePin(data_port, data_pin, RESET);
			}
		}

		// Pulse to clock pin to shift the bit out ot the shift register
		HAL_GPIO_WritePin(shift_port, shift_pin, SET);
		delay_us(1);
		HAL_GPIO_WritePin(shift_port, shift_pin, RESET);
	}
}

// Show pattern on LED matrix for an instant by method of scanning
void display_pattern(uint8_t * pattern_array) {
	for ( size_t i = 0; i < 8; ++i ) {
		/// NOTE: since sent first, stored in SR #2
		// Displays the pattern for each row
		// - Tilda (~) flips the bits; needed since columns require LOW to complete circuit
		// -> To light up an LED in a row, its bit must be 0 (LOW)
		shift_out(GPIOB, GPIO_PIN_10,
				  GPIOA, GPIO_PIN_8,
				  0, ~pattern_array[i]);
		/// NOTE: since sent last, stored in SR #2
		// Scan (turn on) through each column
		shift_out(GPIOB, GPIO_PIN_10,
				  GPIOA, GPIO_PIN_8,
				  0, 0b10000000 >> i);

		store();
	}
}