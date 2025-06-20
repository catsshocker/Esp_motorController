#include "motor_control.h"

Motor::Motor(int inAPin, int inBPin, int pwmPin)
    : inAPin(inAPin), inBPin(inBPin), pwmPin(pwmPin) {
    pinMode(inAPin, OUTPUT);
    pinMode(inBPin, OUTPUT);
    pwmController = new LedcPwm(LEDC_CHANNEL_0, LEDC_TIMER_0, pwmPin);
}

Motor::~Motor() {
    delete pwmController;
}

void Motor::setPower(float power) { // Power should be in the range of -1 to 1
    if (power > 1.0f) power = 1.0f;
    if (power < -1.0f) power = -1.0f;

    if (power > 0) {
        digitalWrite(inAPin, HIGH);
        digitalWrite(inBPin, LOW);
    } else if (power < 0) {
        digitalWrite(inAPin, LOW);
        digitalWrite(inBPin, HIGH);
        power = -power; // Make power positive for PWM
    } else {
        digitalWrite(inAPin, LOW);
        digitalWrite(inBPin, LOW);
    }

    pwmController->setDutyCycle(static_cast<uint16_t>(power * 65535 + 0.5f)); // Scale power to 16-bit duty cycle
}