#ifndef NextBotMotors_h
#define NextBotMotors_h

#include "Arduino.h"

#define FORWARD 101
#define REVERSE 102
#define STOP 103
#define BRAKE 104

class NextBotMotors 
{
    public:
        NextBotMotors(uint8_t mL_dirA, uint8_t mL_dirB, uint8_t mL_pwm,
                    uint8_t mR_dirA, uint8_t mR_dirB, uint8_t mR_pwm);

        /** begin() functions needs to be called to initiate hardware pins. 
         *
         *  @param encEnable Boolean value to enable encoder based calculations.
         */
        void begin();

        void begin(float Kp, float Ki, float Kd);

        /** Sets the minimum and maximum output limit of PWM pins.*/
        void setOutputLimits(uint16_t min, uint16_t max);

        /** move() function for movement of the robot at a given speed.
         *   
         *  @param dir  Direction of movement (value can be FORWARD or REVERSE).
         *  @param speed Speed of motor between 0-100
         */
        void move(uint8_t dir, uint8_t velocity);
        void brake();
        void stop();
        void leftMotor(uint8_t dir, uint8_t velocity);
        void rightMotor(uint8_t dir, uint8_t velocity);
    
        /** Updates left motor encoder count.
         *  
         *  Call this function in function attached to the hardware interrupt pin.
         */
        void updateL_enc()  { _mL_encCount += _mL_encAdd; }

        /** Updates right motor encoder count.
         *  
         *  Call this function in function attached to the hardware interrupt pin.
         */
        void updateR_enc()  { _mR_encCount += _mR_encAdd; }

        void setTunings(float kp, float ki, float kd);

        bool updateState();

    private:
        bool _moving;
        uint8_t _mL_dirA, _mL_dirB, _mL_pwm;
        uint8_t _mR_dirA, _mR_dirB, _mR_pwm;
        uint16_t _mL_out, _mR_out;
        uint16_t _outMin, _outMax;

        bool _encEnable;
        volatile int32_t _mL_encCount, _mR_encCount;
        int8_t _mL_encAdd, _mR_encAdd;
        uint32_t _lastUpdate, _updateTime, _lastInput;
        float _iTerm;
        float _kp, _ki, _kd;

        /** Sets the direction of Left Motor.
         * 
         *  @param dir Direction, values can be (FORWARD, REVERSE, STOP, BRAKE)
         */
        void _setLeftMotorDir(uint8_t dir);

        /** Sets the direction of Right Motor.
         * 
         *  @param dir Direction, values can be (FORWARD, REVERSE, STOP, BRAKE)
         */
        void _setRightMotorDir(uint8_t dir);
};

#endif