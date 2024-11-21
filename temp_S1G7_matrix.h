const int DATA = 6;  // -> P14 SER (Serial Input)
const int SHIFT_CLOCK = 7;  // -> SRCLK (Shift Register Clock)
const int STORE_CLOCK = 8;  // -> RCLK (Storage/Latch Clock)

int picture[] = {0,36,36,36,0,66,60,0};

int right_arrow[] = {
              0b00001000,
              0b00001100,
              0b11111110,
              0b11111111,
              0b11111111,
              0b11111110,
              0b00001100,
              0b00001000
              };

int left_arrow[] = {
              0b00010000,
              0b00110000,
              0b01111111,
              0b11111111,
              0b11111111,
              0b01111111,
              0b00110000,
              0b00010000
              };

int down_arrow[] = {
              0b00111100,
              0b00111100,
              0b00111100,
              0b00111100,
              0b11111111,
              0b01111110,
              0b00111100,
              0b00011000
};

int up_arrow[] = {
              0b00011000,
              0b00111100,
              0b01111110,
              0b11111111,
              0b00111100,
              0b00111100,
              0b00111100,
              0b00111100
};

// Conditional Variables
int x = true;

void store();
void display( int *pattern_array );
void reset_display();

void setup() {
  
  pinMode(DATA, OUTPUT);
  pinMode(SHIFT_CLOCK, OUTPUT);
  pinMode(STORE_CLOCK, OUTPUT);
  
  //shiftOut(DATA, SHIFT_CLOCK, MSBFIRST, 0);  // Pushed to SR #2, so rows are LOW
  //shiftOut(DATA, SHIFT_CLOCK, MSBFIRST, 255);  // Pushed to SR #1, so columns are HIGH
  //store();  // "Prints" data to LED Matrix
}

void loop() {
  // Control the pattern displayed by LED Matrix using for loops

  // Display pattern 1 for 1 second
  for (int i = 0; i < 1000; ++i) { // 1000 iterations at 1ms each = 1 second
    display(left_arrow);
    delay(1); // Scan rows quickly to avoid flicker
  }

  // Display pattern 2 for 1 second
  for (int i = 0; i < 1000; ++i) { // 1000 iterations at 1ms each = 1 second
    display(right_arrow);
    delay(1);
  }

  // Display the pattern by the method of scanning
  // display();

    // delay(1000);  **uncomment to see how the scanning works**
}

void store() {
  digitalWrite(STORE_CLOCK, HIGH);
  delayMicroseconds(10);
  digitalWrite(STORE_CLOCK, LOW);
  delayMicroseconds(10);
}

void display( int *pattern_array ) {
  for ( int i = 0; i < 8; ++i ) {
    /// NOTE: since sent first, stored in SR #2
    // Displays the pattern for each row
    // - Tilda (~) flips the bits; needed since columns require LOW to complete circuit
    // -> To light up an LED in a row, its bit must be 0 (LOW)
    shiftOut(DATA, SHIFT_CLOCK, LSBFIRST, ~pattern_array[i]);
    /// NOTE: since sent last, stored in SR #2
    // Scan (turn on) through each column
    shiftOut(DATA, SHIFT_CLOCK, LSBFIRST, 128 >> i);
    store();
  }
}

void reset_display() {
  shiftOut(DATA, SHIFT_CLOCK, MSBFIRST, 255);
  shiftOut(DATA, SHIFT_CLOCK, MSBFIRST, 0);
  store();
}