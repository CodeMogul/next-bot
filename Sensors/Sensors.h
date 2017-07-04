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

#define PROX_1  ()
#define PROX_2  ()
#define PROX_3  ()
#define PROX_4  ()
#define PROX_5  ()

#define BUTTON_1    ()
#define BUTTON_2    ()
#define BUTTON_3    ()
#define BUTTON_4    ()
#define BUTTON_5    ()

#define MIC         ()
#define BUZZER      ()
#define LED         ()

#define NUMPIXELS   (8)

class Sensors {
    public:
        Sensors();
        void begin();
        bool getButton(uint8_t number);
        int getProximity(uint8_t number);
        int getMic();

        void setLed(uint8_t number, uint8_t r, uint8_t g, uint8_t b);
        void clearLed();
        void setBuzzer(bool state);

    private:
        uint8_t _prox_pin[5],
                _button_pin[5],
                _mic_pin,
                _buzzer_pin,
                _led_pin;

        Adafruit_NeoPixel _neopixels;
};

#endif