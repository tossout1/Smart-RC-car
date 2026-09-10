#include <SoftwareSerial.h>
#include <ArduinoJson.h>

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

int SteeringWheel() {
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
      }
    }
  }
