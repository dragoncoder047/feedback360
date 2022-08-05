/*
  feedback360.h - Library for reading the position and speed of a Parallax Feedback 360 servo.
  Created by dragoncoder047, August 1, 2022.
  Released into the public domain.
*/

#ifndef feedback360_h
#define feedback360_h

#include "Arduino.h"

#if defined(ESP32)
    #include <ESP32Servo.h>
    #include <analogWrite.h>
    #include <ESP32PWM.h>
#else
    #include <Servo.h>
#endif

#define map(x, il, ih, ol, oh) (((x)-(il))*((oh)-(ol))/((ih)-(il))+(ol))


class Feedback360 {
    private:
        uint8_t feedbackPin;
        double pTheta;
        int32_t pTurns;
        unsigned long t;
        Servo servo;
    public:
        double theta;
        int32_t turns;
        double speed;
        Feedback360(Servo *s, uint8_t fbPin);
        void readPosition();
        void setPower(double power);
};


#endif
