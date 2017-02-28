#include "NextBotMotors.h"

NextBotMotors::NextBotMotors(uint8_t mL_dirA, uint8_t mL_dirB, uint8_t mL_pwm,
    uint8_t mR_dirA, uint8_t mR_dirB, uint8_t mR_pwm):
    _mL_dirA(mL_dirA), _mL_dirB(mL_dirB), _mL_pwm(mL_pwm),
    _mR_dirA(mR_dirA), _mR_dirB(mR_dirB), _mR_pwm(mR_pwm)
{
    setOutputLimits(0, 255);
}

void NextBotMotors::begin()
{
    pinMode(_mL_dirA, OUTPUT);
    pinMode(_mL_dirB, OUTPUT);
    pinMode(_mR_dirA, OUTPUT);
    pinMode(_mR_dirB, OUTPUT);
}

void NextBotMotors::setOutputLimits(uint16_t Min, uint16_t Max)
{
    if (Min >= Max)
        return;

    _outMin = Min;
    _outMax = Max;
}

void NextBotMotors::_setLeftMotorDir(uint8_t dir)
{
    switch(dir)
    {
        case FORWARD:
            digitalWrite(_mL_dirA, HIGH);
            digitalWrite(_mL_dirB, LOW);
            break;
        case REVERSE:
            digitalWrite(_mL_dirA, LOW);
            digitalWrite(_mL_dirB, HIGH);
            break;
        case STOP:
            digitalWrite(_mL_dirA, LOW);
            digitalWrite(_mL_dirB, LOW);
            break;
        case BRAKE:
            digitalWrite(_mL_dirA, HIGH);
            digitalWrite(_mL_dirB, HIGH);
            break;
    }
}

void NextBotMotors::_setRightMotorDir(uint8_t dir)
{
    switch(dir)
    {
        case FORWARD:
            digitalWrite(_mR_dirA, HIGH);
            digitalWrite(_mR_dirB, LOW);
            break;
        case REVERSE:
            digitalWrite(_mR_dirA, LOW);
            digitalWrite(_mR_dirB, HIGH);
            break;
        case STOP:
            digitalWrite(_mR_dirA, LOW);
            digitalWrite(_mR_dirB, LOW);
            break;
        case BRAKE:
            digitalWrite(_mR_dirA, HIGH);
            digitalWrite(_mR_dirB, HIGH);
            break;
    }
}

void NextBotMotors::move(uint8_t dir, uint8_t velocity)
{
    velocity = map(velocity, 0, 100, _outMin, _outMax);

    _setLeftMotorDir(dir);
    _setRightMotorDir(dir);

    analogWrite(_mL_pwm, velocity);
    analogWrite(_mR_pwm, velocity);
}

void NextBotMotors::moveLeftMotor(uint8_t dir, uint8_t velocity)
{
    velocity = map(velocity, 0, 100, _outMin, _outMax);
    _setLeftMotorDir(dir);
    analogWrite(_mL_pwm, velocity);
}

void NextBotMotors::moveRightMotor(uint8_t dir, uint8_t velocity)
{
    velocity = map(velocity, 0, 100, _outMin, _outMax);
    _setRightMotorDir(dir);
    analogWrite(_mR_pwm, velocity);
}


