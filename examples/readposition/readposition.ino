#include "../../feedback360.h"

#define FEEDBACK_PIN 14

Feedback360 motor(NULL, FEEDBACK_PIN);

void setup() {
    pinMode(FEEDBACK_PIN, OUTPUT);
    servo.attach(CONTROL_PIN);
    Serial.begin(9600);
}

void loop() {
    motor.readPosition();
    Serial.println("Position\tSpeed");
    Serial.print(motor.turns + motor.theta)
    Serial.write('\t');
    Serial.println(motor.speed);
    // spin the motor with your hands and see what happens
}
