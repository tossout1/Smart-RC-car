// Define the throttle input pin (Pin 2 is an Interrupt pin on Uno/Nano)
const int THROTTLE_PIN = 2;

// Volatile variables to handle values inside the Interrupt Service Routine (ISR)
volatile unsigned long throttle_start = 0;
volatile int throttle_pulse_width = 1500;
volatile bool new_throttle_pulse = false;

// Interrupt Service Routine for Channel 2 (Throttle)
void readThrottle() {
  if (digitalRead(THROTTLE_PIN) == HIGH) {
    throttle_start = micros(); // Signal went HIGH, record the time
  } else {
    // Signal went LOW, calculate the difference
    if (throttle_start != 0) {
      long duration = micros() - throttle_start;
      
      // CRITICAL FILTER: Only accept standard RC pulse widths (950us to 2050us)
      // This permanently blocks the 80, 156, 912 us ghost ramps
      if (duration >= 950 && duration <= 2050) {
        throttle_pulse_width = (int)duration;
        new_throttle_pulse = true; 
      }
    }
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
  interrupts();
  
  // Print out the decoded microsecond values to the Serial Monitor

  // Optional: Map this value to a standard -100 (Full Reverse) to 100 (Full Forward) range
  // int throttlePercent = map(localThrottle, 1000, 2000, -100, 100)
  if (localThrottle < 1490 || localThrottle > 1500) {
    Serial.print("Throttle: ");
    Serial.print(localThrottle);
    Serial.print("\n");
    return localThrottle; // Return the raw pulse width for further processing
  }
  else{
    Serial.print("Throttle: ");
    Serial.print(1500);
    Serial.print("\n");
    return 1500;
    }
}
