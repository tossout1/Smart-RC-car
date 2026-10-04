const int THROTTLE_PIN = 2;

volatile unsigned long throttle_start = 0;
volatile int throttle_pulse_width = 1500;
volatile bool throttle_high = false;

int ESC() {
    int localThrottle;

    noInterrupts();
    localThrottle = throttle_pulse_width;
    interrupts();

    if (localThrottle < 1490) {
    Serial.print("Throttle (Ch2): ");
    Serial.print(localThrottle);
    Serial.println(" us");
    }
    return localThrottle;
}

void readThrottle() {
    unsigned long current_time = micros();
    bool pinState = digitalRead(THROTTLE_PIN);

    if (pinState == HIGH) {

        // Only accept a rising edge if we aren't already
        // waiting for the falling edge.
        if (!throttle_high) {
            throttle_start = current_time;
            throttle_high = true;
        }

    } else {

        // Only calculate a pulse if we previously
        // received a valid rising edge.
        if (throttle_high) {
            unsigned long duration = current_time - throttle_start;

            throttle_high = false;

            // Only accept plausible RC PWM pulses.
            if (duration >= 800 && duration <= 2200) {
                throttle_pulse_width = (int)duration;
            }
        }
    }
}
