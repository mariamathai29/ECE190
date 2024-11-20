/// S1G7_uart.h
// Enable UART communication between two STM32s and their TX & RX pins

/// NOTE: This file is for reference and is not designed to be included in main.c
// Included are two pieces of code to perform the following actions: 
//     1) Transmitting data from the Input MCU to the Output MCU
//     2) Receiving data at the Output MCU from the Input MCU

/// Input MCU - Transmitting data

// i) Initialize variables for UART [declare in main()]
char *msg;  // Set up a variable that can be modified to store data before TX
char UART1_txBuffer[20];  // Set up a buffer string to transmit data (size 20 is more than safe)

// ii) Figure out what you're going to send based on the current state, 
//    whether it's a sentence or number. This is just an example: 
if ( (counter % 2) == 0 ) {
		  msg = "X";
	  } else {
		  msg = "Y";
	  }

// iii) Format the message and package it up in the buffer
sprintf(UART1_txBuffer, "%s\r\n", msg)

// iv) Finally, transmit the data 
//     - Ensure the particular UART peripheral matches the OUTPUT MCU
//     - Cast the TX buffer to the uint8_t type so the f'n can handle it
HAL_UART_Transmit(&huart1, (uint8_t*)UART1_txBuffer, 20, HAL_MAX_DELAY);

/// Output MCU - Receiving Data

// i) Initialize variables for UART [declare in main()]
char UART1_rxBuffer[20];  // Set up a buffer string to receive data

// ii) Clear RX buffer before receiving new data
//     - Avoids conflicts with previous data stored in RX buffer
memset(UART1_rxBuffer, 0, 20);

// iii) Finally, receive the data
//     - Ensure the particular UART peripheral matches the OUTPUT MCU
//     - Cast the TX buffer to the uint8_t type so the f'n can handle it
//     - Don't be afraid to play around with the Timeout parameter (100 - 5000)
HAL_UART_Receive(&huart1, (uint8_t*)UART1_rxBuffer, 20, 1000);

// OPTIONAL: iv) Print received data to serial console
printf("Received Data: %s\r\n", UART1_rxBuffer);
HAL_Delay(1000);  // Control speed of serial monitor