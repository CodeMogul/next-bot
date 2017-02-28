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
        NextBotMotors(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

        /** begin() functions needs to be called to initiate hardware pins. */
        void begin();

        /** Sets the minimum and maximum output limit of PWM pins.*/
        void setOutputLimits(uint16_t, uint16_t);

        /** move() function for movement of the robot at a given speed.
         *   
         *  @param dir  Direction of movement (value can be FORWARD or REVERSE).
         *  @param speed Speed of motor between 0-100
         */
        void move(uint8_t, uint8_t);

        void moveLeftMotor(uint8_t dir, uint8_t velocity);

        void moveRightMotor(uint8_t dir, uint8_t velocity);

        void brake();

        void stop();
    
    private:
        uint8_t _mL_dirA, _mL_dirB, _mL_pwm;
        uint8_t _mR_dirA, _mR_dirB, _mR_pwm;
        uint16_t _outMin, _outMax;

        /** Sets the direction of Left Motor.
         * 
         *  @param dir Direction, values can be (FORWARD, REVERSE, STOP, BRAKE)
         */
        void _setLeftMotorDir(uint8_t);

        /** Sets the direction of Right Motor.
         * 
         *  @param dir Direction, values can be (FORWARD, REVERSE, STOP, BRAKE)
         */
        void _setRightMotorDir(uint8_t);
};

#endif