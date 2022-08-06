#include "../../feedback360.h"

#define FEEDBACK_PIN 14

Feedback360 motor;

void setup() {
    pinMode(FEEDBACK_PIN, OUTPUT);
    motor.attach(NULL, FEEDBACK_PIN);
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
