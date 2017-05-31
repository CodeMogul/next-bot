/**
 *  A motor control library for the NextBot .
 *  Motor Driver: DRV8835
 *
 *  Control individual motor and robot. Also includes PID for straight-line motion.
 *
 *  @author Siddhesh Nachane
 *  @version 0.9 03-03-2017
 *
 *  Updates:
 *  1. @SiddheshNachane - changed the library for DRV8835 - 31-05-2017
 */

#include "NextBotMotors.h"

NextBotMotors::NextBotMotors(uint8_t mL_dir, uint8_t mL_pwm, 
    uint8_t mR_dir, uint8_t mR_pwm):
    _mL_dir(mL_dir), _mL_pwm(mL_pwm),
    _mR_dir(mR_dir), _mR_pwm(mR_pwm)
{
    setOutputLimits(0, 255);
    _mL_encAdd = _mR_encAdd = 1;
    _mL_encCount = _mR_encCount = 0;
    _moving = false;
}

void NextBotMotors::begin()
{
    pinMode(_mL_dir, OUTPUT);
    pinMode(_mR_dir, OUTPUT);  
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
            digitalWrite(_mL_dir, LOW);
            _mL_encAdd = 1;
            break;
        case REVERSE:
            digitalWrite(_mL_dir, HIGH);
            _mL_encAdd = -1;
            break;
    }
}

void NextBotMotors::_setRightMotorDir(uint8_t dir)
{
    switch(dir)
    {
        case FORWARD:
            digitalWrite(_mR_dir, LOW);
            _mR_encAdd = 1;
            break;
        case REVERSE:
            digitalWrite(_mR_dir, HIGH);
            _mR_encAdd = -1;
            break;
    }
}

void NextBotMotors::move(uint8_t dir, uint8_t velocity)
{
    leftMotor(dir, velocity);
    rightMotor(dir, velocity);
    _iTerm = _mL_out;
    _moving = true;
}

void NextBotMotors::stop()
{
    analogWrite(_mL_pwm, 0);
    analogWrite(_mR_pwm, 0);

    _moving = false;
}

void NextBotMotors::leftMotor(uint8_t dir, uint8_t velocity)
{
    if(dir == STOP) _mL_out = 0;
    else {
        _setLeftMotorDir(dir);
        _mL_out = map(velocity, 0, 100, _outMin, _outMax);
    }

    analogWrite(_mL_pwm, _mL_out);
}

void NextBotMotors::rightMotor(uint8_t dir, uint8_t velocity)
{
    if(dir == STOP) _mR_out = 0;
    else {
        _setRightMotorDir(dir);
        _mR_out = map(velocity, 0, 100, _outMin, _outMax);
    };

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
    analogWrite(_mL_pwm, _mL_out);

    _lastInput = input;
    _lastUpdate = now;

    return true;
}
