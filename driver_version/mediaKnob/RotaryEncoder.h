#ifndef RotaryEncoder_h
#define RotaryEncoder_h

#include "Arduino.h"


// for modes
#define MEDIA_CONTROL 0
#define SCROLLING 1
#define NUM_LINES_PER_SCROLL 2

// delays tuned to adafruit qt py
#define DEBOUNCE_DELAY 200    // the debounce time; increase if the output flickers
#define DOUBLE_PRESS_TIME 600
#define LONG_PRESS_TIME 400

// hid interface commands
#define VOLUME_UP 0x80
#define VOLUME_DOWN 0x81
#define NEXT_SONG 0xeb
#define PREV_SONG 0xea

// hardware settings
#define DIR_CW 0x10
#define DIR_CCW 0x20
#define R_START 0x0

#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6

const unsigned char ttable[7][4] = {
  // R_START
  {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
  // R_CW_FINAL
  {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CCW},
  // R_CW_BEGIN
  {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
  // R_CW_NEXT
  {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
  // R_CCW_BEGIN
  {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
  // R_CCW_FINAL
  {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CW},
  // R_CCW_NEXT
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};

class RotaryEncoder {
    public:
        RotaryEncoder(uint8_t _pin1, uint8_t _pin2, uint8_t _buttonPin);
        uint8_t process();
        bool checkButton();
    private:
        uint8_t state;
        uint8_t pin1;
        uint8_t pin2;
        uint8_t buttonPin;
};

#endif
