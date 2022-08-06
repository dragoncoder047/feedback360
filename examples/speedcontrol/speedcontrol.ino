#include "../../feedback360.h"

#ifdef ESP32
#define ANALOG_MAX 4095.0
#else
#define ANALOG_MAX 1023.0
#endif

#define FEEDBACK_PIN 14
#define CONTROL_PIN 4
#define POT_PIN A3

Feedback360 motor;

double currentPower = 0;

void setup() {
    motor.attach(CONTROL_PIN, FEEDBACK_PIN);
}

void loop() {
    motor.readPosition();
    double targetSpeed = map(analogRead(POT_PIN), 0, ANALOG_MAX, -2.0, 2.0);
    double error = motor.speed - targetSpeed;
    currentPower += error * 3 // Kp=3
    // no Ki or Kd
    motor.setPower(currentPower);
    // feel free to implement you own PID controller here!
}