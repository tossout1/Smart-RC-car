// Define the throttle input pin (Pin 2 is an Interrupt pin on Uno/Nano)
const int THROTTLE_PIN = 5;

// Volatile variables to handle values inside the Interrupt Service Routine (ISR)
volatile unsigned long throttle_start = 0;
volatile int throttle_pulse_width = 1500; // Defaults to neutral


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
  Serial.begin(115200); // Fast baud rate for clean serial plotting
  
  pinMode(THROTTLE_PIN, INPUT);
  
  // Attach interrupt to detect any change in signal (High to Low or Low to High)
  attachInterrupt(digitalPinToInterrupt(THROTTLE_PIN), readThrottle, CHANGE);
}

void ESC() {
  int localThrottle;
  
  // Temporarily disable interrupts to safely copy volatile data without corruption
  noInterrupts();
  localThrottle = throttle_pulse_width;
  interrupts();
  
  // Print out the decoded microsecond values to the Serial Monitor
  Serial.print("Throttle (Ch2): ");
  Serial.print(localThrottle);
  Serial.println(" us");

  // Optional: Map this value to a standard -100 (Full Reverse) to 100 (Full Forward) range
  // int throttlePercent = map(localThrottle, 1000, 2000, -100, 100);

  delay(50); // Small delay to keep the Serial Monitor readable
}
