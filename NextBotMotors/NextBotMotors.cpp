#include "NextBotMotors.h"

NextBotMotors::NextBotMotors(uint8_t mL_dirA, uint8_t mL_dirB, uint8_t mL_pwm,
    uint8_t mR_dirA, uint8_t mR_dirB, uint8_t mR_pwm):
    _mL_dirA(mL_dirA), _mL_dirB(mL_dirB), _mL_pwm(mL_pwm),
    _mR_dirA(mR_dirA), _mR_dirB(mR_dirB), _mR_pwm(mR_pwm)
{
    setOutputLimits(0, 255);
    _mL_encAdd = _mR_encAdd = 1;
    _mL_encCount = _mR_encCount = 0;
    _moving = false;
}

void NextBotMotors::begin()
{
    pinMode(_mL_dirA, OUTPUT);
    pinMode(_mL_dirB, OUTPUT);
    pinMode(_mR_dirA, OUTPUT);
    pinMode(_mR_dirB, OUTPUT);  
    _encEnable = false;  
}

void NextBotMotors::begin(float Kp, float Ki, float Kd)
{
    begin();
    _encEnable = true;
    _updateTime = 100;
    _iTerm = 0;
    setTunings(Kp, Ki, Kd);
    _lastUpdate = millis();
}

void NextBotMotors::setOutputLimits(uint16_t min, uint16_t max)
{
    if (min >= max)
        return;

    _outMin = min;
    _outMax = max;
}

void NextBotMotors::setTunings(float kp, float ki, float kd)
{
    float updateTimeInSec = (float)_updateTime/1000;
    _kp = kp;
    _ki = ki * updateTimeInSec;
    _kd = kd / updateTimeInSec;
}

void NextBotMotors::_setLeftMotorDir(uint8_t dir)
{
    switch(dir)
    {
        case FORWARD:
            digitalWrite(_mL_dirA, HIGH);
            digitalWrite(_mL_dirB, LOW);
            _mL_encAdd = 1;
            break;
        case REVERSE:
            digitalWrite(_mL_dirA, LOW);
            digitalWrite(_mL_dirB, HIGH);
            _mL_encAdd = -1;
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
            _mR_encAdd = 1;
            break;
        case REVERSE:
            digitalWrite(_mR_dirA, LOW);
            digitalWrite(_mR_dirB, HIGH);
            _mR_encAdd = -1;
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
    if(dir == STOP || dir == BRAKE) _mL_out = 0;
    else _mL_out = map(velocity, 0, 100, _outMin, _outMax);
    _iTerm = _mR_out = _mL_out;

    _setLeftMotorDir(dir);
    _setRightMotorDir(dir);

    analogWrite(_mL_pwm, _mL_out);
    analogWrite(_mR_pwm, _mR_out);

    _moving = true;
}

void NextBotMotors::stop()
{
    _setLeftMotorDir(STOP);
    _setRightMotorDir(STOP);

    analogWrite(_mL_pwm, 0);
    analogWrite(_mR_pwm, 0);

    _moving = false;
}

void NextBotMotors::brake()
{
    _setLeftMotorDir(BRAKE);
    _setRightMotorDir(BRAKE);

    analogWrite(_mL_pwm, 0);
    analogWrite(_mR_pwm, 0);

    _moving = false;
}

void NextBotMotors::leftMotor(uint8_t dir, uint8_t velocity)
{
    if(dir == STOP || dir == BRAKE) _mL_out = 0;
    else _mL_out = map(velocity, 0, 100, _outMin, _outMax);

    _setLeftMotorDir(dir);
    analogWrite(_mL_pwm, _mL_out);
}

void NextBotMotors::rightMotor(uint8_t dir, uint8_t velocity)
{
    if(dir == STOP || dir == BRAKE) _mR_out = 0;
    else _mR_out = map(velocity, 0, 100, _outMin, _outMax);

    _setRightMotorDir(dir);
    analogWrite(_mR_pwm, _mR_out);
}

bool NextBotMotors::updateState()
{
    uint32_t now = millis();
    if(!_moving || !_encEnable || (now - _lastUpdate) < _updateTime) return false;
    
    int32_t input = _mL_encCount, setPoint = _mR_encCount;

    int16_t error = setPoint - input;
    _iTerm += _ki * error;
    if(_iTerm > _outMax) _iTerm = _outMax;
    if(_iTerm < _outMin) _iTerm = _outMin;
    float dInput = input - _lastInput;

    float output = _kp * error + _iTerm + (_kd * dInput);

    if(output > _outMax) output = _outMax;
    if(output < _outMin) output = _outMin;
    
    _mL_out = output;
    analogWrite(_mR_pwm, _mL_out);

    _lastInput = input;
    _lastUpdate = now;

    return true;
}
