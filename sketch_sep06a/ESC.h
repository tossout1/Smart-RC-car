// Define the throttle input pin (Pin 2 is an Interrupt pin on Uno/Nano)
const int THROTTLE_PIN = 2;

// Volatile variables to handle values inside the Interrupt Service Routine (ISR)
volatile unsigned long throttle_start = 0;
volatile int throttle_pulse_width = 1500;

// Interrupt Service Routine for Channel 2 (Throttle)
void readThrottle() {
  if (digitalRead(THROTTLE_PIN) == HIGH) {
    throttle_start = micros(); // Signal went HIGH, record the time
  } else {
    // Signal went LOW, calculate the difference
    throttle_pulse_width = (int)(micros() - throttle_start);
  }
}


void setupESC() {  
  pinMode(THROTTLE_PIN, INPUT);
  
  // Attach interrupt to detect any change in signal (High to Low or Low to High)
  attachInterrupt(digitalPinToInterrupt(THROTTLE_PIN), readThrottle, CHANGE);
  Serial.print("Set up");
}

int ESC() {
  int localThrottle;
  
  // Temporarily disable interrupts to safely copy volatile data without corruption
  noInterrupts();
  localThrottle = throttle_pulse_width;
  Serial.print("localThrottle");
  interrupts();
  
  // Print out the decoded microsecond values to the Serial Monitor
  Serial.print("Throttle: ");
  Serial.print(localThrottle);
  Serial.print("\n");

  // Optional: Map this value to a standard -100 (Full Reverse) to 100 (Full Forward) range
  // int throttlePercent = map(localThrottle, 1000, 2000, -100, 100);

  delay(100); // Small delay to keep the Serial Monitor readable
  return localThrottle; // Return the raw pulse width for further processing
}
