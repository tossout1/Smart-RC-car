#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <algorithm> // For std::clamp
#include <cstdint>

const inputSteering = 2;
const outputSteering = 3;
const inputDrive = 4;
const outputDrive = 5;
SoftwareSerial wirelessSerial(8, 9);

void setup() {
  pinMode(inputSteering, INPUT);
  pinMode(outputSteering, OUTPUT);
  pinMode(inputDrive, INPUT);
  pinMode(outputDrive, OUTPUT);
  Serial.begin(9600);
  wirelessSerial.begin(9600);
  Serial.println("Started serial connection on 9600.")
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
    S = std::clamp(S, 996, 1492);
    
    // Process math with rounding (+248 is half of the 496 divisor)
    return static_cast<uint8_t>((((1492 - S) * 255) + 248) / 496);
}

int CarWheel() {
  if (wirelessSerial.available() > 0) {
     String jsonPayload = WirelessSerial.readStringUntil('\n');
     jsonPayload.trim(); 
      if (jsonPayload.length() > 0) {
      // 2. Allocate memory for the JSON document (V7 syntax)
      JsonDocument doc;

      // 3. Parse the string
      DeserializationError error = deserializeJson(doc, jsonPayload);

      if (error) {
        Serial.print("Parsing failed: ");
        Serial.println(error.f_str());
        return; 
      }

      // 4. Extract the dynamic key and integer value
      JsonObject root = doc.as<JsonObject>();
      for (JsonPair kv : root) {
        String eventCodeName = kv.key().c_str();
        int eventValue = kv.value().as<int>();

        // Print results to your standard USB Serial Monitor
        Serial.print("Event Received -> [");
        Serial.print(eventCodeName);
        Serial.print("]: ");
        Serial.println(eventValue);
        if (eventCodeName == "ABS_X") {
          Steering = calculateSteering(eventValue);
          digitalWrite(outputSteering, Steering);
        }
        if (eventCodeName == "ABS_RZ") {
          ForwardThrottle = calculateForwardThrottle(eventValue);
          digitalWrite(outputDrive, ForwardThrottle);
        }
        if (eventCodeName == "ABS_Z") {
          BackwardThrottle = calculateBackwardThrottle(eventValue);
          digitalWrite(outputDrive, BackwardThrottle);
        }
      }
    }
  }
}
