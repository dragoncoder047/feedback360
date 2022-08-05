#include "../../feedback360.h"

#if defined(ESP32)
    #include <ESP32Servo.h>
    #include <analogWrite.h>
    #include <ESP32PWM.h>
    #define ANALOG_MAX 4095.0
#else
    #include <Servo.h>
    #define ANALOG_MAX 1023.0
#endif

#define FEEDBACK_PIN 14
#define CONTROL_PIN 4
#define POT_PIN A3

Servo servo;
Feedback360 motor(&servo, FEEDBACK_PIN);

double currentPower = 0;

void setup() {
    pinMode(FEEDBACK_PIN, OUTPUT);
    servo.attach(CONTROL_PIN);
}

void loop() {
    motor.readPosition();
    double targetSpeed = map(analogRead(POT_PIN), 0, ANALOG_MAX, -2.0, 2.0);
    double error = motor.speed - targetSpeed;
    currentPower += error * 0.8 // Kp
    // no Ki or Kd
    motor.setPower(-currentPower); // -: see NOTE 2 on readme
    // feel free to implement you own PID controller here!
}