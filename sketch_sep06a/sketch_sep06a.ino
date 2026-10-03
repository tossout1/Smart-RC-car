#include <Arduino_JSON.h>
#include <SoftwareSerial.h>
#include <Arduino.h>
#include <stdint.h>
#include "steering.h"
#include "ESC.h"
#include<Servo.h>

const int inputSteering = 3;
const int outputSteering = 4;
const int inputDrive = 2;
const int outputDrive = 6;
unsigned long steeringValue;
uint8_t forwardThrottle;
uint8_t backwardThrottle;
Servo steeringOutput;
Servo driveOutput;
SoftwareSerial wirelessSerial(8, 9);
int clampValue(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void setup() {
  // Attach interrupt to detect any change in signal (High to Low or Low to High)
  attachInterrupt(digitalPinToInterrupt(THROTTLE_PIN), readThrottle, CHANGE);
  pinMode(inputSteering, INPUT);
  pinMode(inputDrive, INPUT);
  wirelessSerial.begin(9600);
  Serial.println("Started serial connection on 9600.");
  steeringOutput.attach(outputSteering);
  driveOutput.attach(outputDrive);
  Serial.begin(115200); // Upgraded to 115200 for faster communication and less ISR latency
  pinMode(THROTTLE_PIN, INPUT_PULLUP); // Use PULLUP to prevent floating pin behavior if disconnected
  // Attach interrupt to detect any change in signal
  attachInterrupt(digitalPinToInterrupt(THROTTLE_PIN), readThrottle, CHANGE);
  }

int RCcontroller() {
  steeringOutput.writeMicroseconds(Steering());
  driveOutput.writeMicroseconds(ESC());
}

int16_t calculateSteering(int S) {
    if (S >= 1531 && S <= 1537) return 0;
    
    if (S > 1537) {
        if (S > 1984) S = 1984; // Clamp max left
        return (int16_t)(((S - 1537) * -32768) / 447);
    }
    
    if (S < 1039) S = 1039; // Clamp max right
    return (int16_t)(((1531 - S) * 32767) / 492);
}

uint8_t calculateForwardThrottle(int S) {
    if (S < 1496) S = 1496;
    if (S > 1996) S = 1996;
    return (uint8_t)((((S - 1496) * 255) + 250) / 500);
}

uint8_t calculateBackwardThrottle(int S) {
    // Clamp to ensure the input stays within the designated backward window
    S = clampValue(S, 996, 1492);
    
    // Process math with rounding (+248 is half of the 496 divisor)
    return static_cast<uint8_t>((((1492 - S) * 255) + 248) / 496);
}

void CarWheel() {
  if (wirelessSerial.available() > 0) {

    String jsonPayload = wirelessSerial.readStringUntil('\n');
    jsonPayload.trim();

    if (jsonPayload.length() > 0) {

      JSONVar doc = JSON.parse(jsonPayload);

      if (JSON.typeof(doc) == "undefined") {
        Serial.println("Parsing failed");
        return;
      }

      JSONVar keys = doc.keys();

      for (int i = 0; i < keys.length(); i++) {

        String eventCodeName = (const char*)keys[i];
        int eventValue = (int)doc[eventCodeName];

        Serial.print("Event Received -> [");
        Serial.print(eventCodeName);
        Serial.print("]: ");
        Serial.println(eventValue);

        if (eventCodeName == "ABS_X") {
          int Steering = calculateSteering(eventValue);
          digitalWrite(outputSteering, Steering);
        }

        if (eventCodeName == "ABS_RZ") {
          int ForwardThrottle = calculateForwardThrottle(eventValue);
          digitalWrite(outputDrive, ForwardThrottle);
        }

        if (eventCodeName == "ABS_Z") {
          int BackwardThrottle = calculateBackwardThrottle(eventValue);
          digitalWrite(outputDrive, BackwardThrottle);
        }
      }
    }
  }
}

void loop() {
  RCcontroller();
  }
