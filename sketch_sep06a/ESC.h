// Define the throttle input pin (Pin 2 is an Interrupt pin on Uno/Nano)
const int THROTTLE_PIN = 2;

// Volatile variables to handle values inside the Interrupt Service Routine (ISR)
volatile unsigned long throttle_start = 0;
volatile int throttle_pulse_width = 1500; // Defaults to neutral (1500 us)

int ESC() {
  int localThrottle;

  // Critical section: safely copy volatile data without corruption
  noInterrupts();
  localThrottle = throttle_pulse_width;
  interrupts();

  // Constrain the value to valid RC pulse widths to ignore spikes/glitches
  localThrottle = constrain(localThrottle, 900, 2100);

  // Print out the decoded microsecond values
  Serial.print("Throttle (Ch2): ");
  Serial.print(localThrottle);
  Serial.print(" us\t");

  // Map to a standard -100 to 100 range
  int throttlePercent = map(localThrottle, 1000, 2000, -100, 100);
  throttlePercent = constrain(throttlePercent, -100, 100); // Ensure it stays within bounds
  
  Serial.print("Percent: ");
  Serial.print(throttlePercent);
  Serial.println("%");

  delay(50); // Small delay to keep the Serial Monitor readable
  return localThrottle;
}

// Interrupt Service Routine for Channel 2 (Throttle)
void readThrottle() {
  unsigned long current_time = micros();
  
  if (digitalRead(THROTTLE_PIN) == HIGH) {
    throttle_start = current_time; // Signal went HIGH, record the time
  } else {
    // Signal went LOW, calculate the difference
    long duration = current_time - throttle_start;
    
    // Basic glitch filtering: only update if the pulse width is physically possible for an RC receiver
    if (duration >= 800 && duration <= 2200) {
      throttle_pulse_width = (int)duration;
    }
  }
}
