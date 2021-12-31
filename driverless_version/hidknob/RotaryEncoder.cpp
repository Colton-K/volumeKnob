#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder(uint8_t _pin1, uint8_t _pin2, uint8_t _buttonPin) {
    pin1 = _pin1;
    pin2 = _pin2;
    buttonPin = _buttonPin;
    state = R_START;

    // configure pins
    pinMode(pin1, INPUT);
    pinMode(pin2, INPUT);
    // enable pullups
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, HIGH);
}

uint8_t RotaryEncoder::process() {
    unsigned char pinstate = (digitalRead(pin2) << 1) | digitalRead(pin1);
    state = ttable[state & 0xf][pinstate];
    return state & 0x30;
}

bool RotaryEncoder::checkButton() {
    return !digitalRead(buttonPin);
}



