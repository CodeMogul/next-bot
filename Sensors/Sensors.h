/**
 *  Sensors library for geting values of all sensors.
 *  
 *  @author Siddhesh Nachane
 *  @version 0.9 06-05-2017
 */

#ifndef Sensors_h
#define Sensors_h

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Set pins according to PCB Layout

#define PROX_1  (A3)
#define PROX_2  (A2)
#define PROX_3  (A1)
#define PROX_4  (A0)
#define PROX_5  (A7)

#define TOUCH_1    (2)
#define TOUCH_2    (3)
#define TOUCH_3    (4)
#define TOUCH_4    (5)
#define TOUCH_5    (8)

#define MIC         (A6)
#define BUZZER      (6)
#define LED         (7)

#define NUMPIXELS   (2)

class Sensors {
    public:
        Sensors();
        void begin();
        bool getTouch(uint8_t number);
        int getProximity(uint8_t number);
        int getMic();
        bool longTouch(uint8_t number);

        void setLed(uint8_t number, uint8_t r, uint8_t g, uint8_t b);
        void clearLed();
        void setBuzzer(bool state);

    private:
        uint8_t _prox_pin[5],
                _touch_pin[5],
                _mic_pin,
                _buzzer_pin,
                _led_pin;

        Adafruit_NeoPixel _neopixels;
};

#endif