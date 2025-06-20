#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "ledc_pwm/ledc_pwm.hpp"

class Motor {
private:
    int inAPin;
    int inBPin;
    int pwmPin;
    LedcPwm* pwmController;

public:
    Motor(int inAPin, int inBPin, int pwmPin);
    ~Motor();

    void setPower(float power);
};

#endif