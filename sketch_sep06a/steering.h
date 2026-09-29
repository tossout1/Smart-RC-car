
const int steerPin = 3; // Signal wire connected to Digital Pin 3

static void setupSteering() {
  pinMode(steerPin, INPUT);
  Serial.begin(9600);
  Serial.println("--- Starting Traxxas Signal Test ---");
}

unsigned long Steering() {
  // Read the raw microsecond pulse with an increased 50ms timeout
  unsigned long rawPulse = pulseIn(steerPin, HIGH, 50000); 
  
  // Print the raw value exactly as it is received
  Serial.print("Steering Raw Pulse Width: ");
  Serial.print(rawPulse);
  Serial.println(" us"); 
  return rawPulse;
}
