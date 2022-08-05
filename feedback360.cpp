/*
  feedback360.cpp - Library for reading the position and speed of a Parallax Feedback 360 servo.
  Created by dragoncoder047, August 1, 2022.
  Released into the public domain.
*/

#include "Arduino.h"
#include "feedback360.h"

Feedback360::Feedback360(Servo *s, uint8_t fbPin) {
    servo = s;
    feedbackPin = fbPin;
    theta = 0;
    pTheta = 0;
    turns = 0;
    pTurns = 0;
    speed = 0;
    t = millis();
}

void Feedback360::readPosition() {
    pTheta = theta;
    pTurns = turns;
    uint16_t tCycle, tHigh, tLow;
    double dc;
    while (true) {
        tHigh = pulseIn(feedbackPin, HIGH);
        tLow = pulseIn(feedbackPin, LOW);
        tCycle = tHigh + tLow;
        if (1000 < tCycle && tCycle < 1200) break; // nominal time is 1100us
    }
    dc = (double)tHigh / tCycle;
    // 0.029 and 0.971 correspond to 2.9% and 97.1% duty cycle min and max
    theta = constrain(map(dc, 0.029, 0.971, 0.0, 1.0), 0.0, 1.0);
    if (theta < 0.25 && pTheta > 0.75) turns++;
    else if (pTheta < 0.25 && theta > 0.75) turns--;
    double dt = (millis() - t) / 1000.0;
    t = millis();
    speed = ((turns + theta) - (pTurns + pTheta)) / dt;
}

void Feedback360::setPower(double power) {
    servo.writeMicroseconds(map(power, -1.0, 1.0, 1280.0, 1720.0));
}
