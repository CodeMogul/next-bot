/**
 *  Sensors library for geting values of all sensors.
 *  
 *  @author Siddhesh Nachane
 *  @version 0.9 06-05-2017
 */

#include "Sensors.h"

Sensors::Sensors() {
    _prox_pin[0] = PROX_1;
    _prox_pin[1] = PROX_2;
    _prox_pin[2] = PROX_3;
    _prox_pin[3] = PROX_4;
    _prox_pin[4] = PROX_5;

    _touch_pin[0] = TOUCH_1;
    _touch_pin[1] = TOUCH_2;
    _touch_pin[2] = TOUCH_3;
    _touch_pin[3] = TOUCH_4;
    _touch_pin[4] = TOUCH_5;

    _mic_pin = MIC;
    _buzzer_pin = BUZZER;
    _led_pin = LED;

    _neopixels = Adafruit_NeoPixel(NUMPIXELS, LED, NEO_GRB + NEO_KHZ800);
}

void Sensors::begin() {
    for(int i = 0 ; i < 5 ; i++) {
        pinMode(_touch_pin[i], INPUT);
    }

    pinMode(_buzzer_pin, OUTPUT);
    pinMode(_led_pin, OUTPUT);
    _neopixels.begin();
}

bool Sensors::getTouch(uint8_t number) {
    bool state = digitalRead(_touch_pin[number]);
    return state;
}

bool Sensors::longTouch(uint8_t number) {
    bool state = true;
    for(uint8_t i = 0 ; i < 6 ; i++) {
        state = state & digitalRead(_touch_pin[number]);
        delay(500);
    }
    return state;
}

int Sensors::getProximity(uint8_t number) {
    int val = analogRead(_prox_pin[number]);
    return val;
}

int Sensors::getMic() {
    int val = analogRead(_mic_pin);
    return val;
}

void Sensors::setLed(uint8_t number, uint8_t r, uint8_t g, uint8_t b) {
    _neopixels.setPixelColor(number, r, g, b);
    _neopixels.show();
}

void Sensors::clearLed() {
    _neopixels.clear();
}

void Sensors::setBuzzer(bool state) {
    if(state)   digitalWrite(_buzzer_pin, HIGH);
    else        digitalWrite(_buzzer_pin, LOW);
}