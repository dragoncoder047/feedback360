/*
  feedback360.cpp - Library for reading the position and speed of a Parallax Feedback 360 servo.
  Created by dragoncoder047, August 1, 2022.
  Released into the public domain.
*/

#include "feedback360.h"
#pragma once
#if defined(ESP32)
#include <ESP32Servo.h>
#include <analogWrite.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>
#else
#include <Servo.h>
#endif
#include <Arduino.h>

#define map(x, il, ih, ol, oh) (((x)-(il))*((oh)-(ol))/((ih)-(il))+(ol))

class Feedback360 {
    private:
    uint8_t controlPin;
    uint8_t feedbackPin;
    Servo servo;
    double pTheta;
    int32_t pTurns;
    unsigned long t;
    public:
    double theta;
    int32_t turns;
    double speed;
    bool reversed;
    Feedback360(bool reversed = true) {
        this->controlPin = 0;
        this->feedbackPin = 0;
        this->theta = 0;
        this->pTheta = 0;
        this->turns = 0;
        this->pTurns = 0;
        this->speed = 0;
        this->t = millis();
        this->reversed = reversed;
    }
    void attach(uint8_t controlPin, uint8_t feedbackPin) {
        this->controlPin = controlPin;
        this->feedbackPin = feedbackPin;
        pinMode(this->controlPin, OUTPUT);
        pinMode(this->feedbackPin, INPUT);
        this->servo.attach(this->controlPin);
    }
    void detach() {
        this->servo.detach();
    }
    void readPosition() {
        this->pTheta = this->theta;
        this->pTurns = this->turns;
        uint16_t tCycle, tHigh, tLow;
        double dc;
        while (true) {
            tHigh = pulseIn(this->feedbackPin, HIGH);
            tLow = pulseIn(this->feedbackPin, LOW);
            tCycle = tHigh + tLow;
            if (1000 < tCycle && tCycle < 1200) break; // nominal time is 1100us
        }
        dc = (double)tHigh / tCycle;
        // 0.029 and 0.971 correspond to 2.9% and 97.1% duty cycle min and max
        this->theta = constrain(map(dc, 0.029, 0.971, 0.0, 1.0), 0.0, 1.0);
        if (this->theta < 0.25 && this->pTheta > 0.75) this->turns++;
        else if (this->pTheta < 0.25 && this->theta > 0.75) this->turns--;
        double dt = (millis() - this->t) / 1000.0;
        this->t = millis();
        this->speed = ((this->turns + this->theta) - (this->pTurns + this->pTheta)) / dt;
    }
    void setPower(double power) {
        this->servo.writeMicroseconds(map(this->reversed ? -power : power, -1.0, 1.0, 1280.0, 1720.0));
    }
};
