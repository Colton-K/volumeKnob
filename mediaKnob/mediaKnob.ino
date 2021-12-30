#include "arduino.h"
#include <Adafruit_NeoPixel.h> // for indicator rgb
/* #include <Keyboard.h> // for registering as keyboard */
#include "Keyboard.h"
/* #include "TrinketHidCombo.h" */
/* #include "HID-Project.h" */

// delay workaround
/*
#if !defined(ARDUINO_ARCH_SAM) && !defined(ARDUINO_ARCH_SAMD) && !defined(ESP8266) && !defined(ARDUINO_ARCH_STM32F2)
 #include <util/delay.h>
#endif
*/

#define PIN_ENCODER_CLK 0
#define PIN_ENCODER_DT 1
#define PIN_ENCODER_SW 2
#define TRINKET_PINx PINB

#define SET_LED_RED pixels.setPixelColor(0, pixels.Color(255, 0, 0)); pixels.show();
#define SET_LED_GREEN pixels.setPixelColor(0, pixels.Color(0, 255, 0)); pixels.show();

// for modes
#define MEDIA_CONTROL 0
#define SCROLLING 1

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
        RotaryEncoder(uint8_t _pin1, uint8_t _pin2, uint8_t _buttonPin) {
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

        uint8_t process() {
            unsigned char pinstate = (digitalRead(pin2) << 1) | digitalRead(pin1);
            state = ttable[state & 0xf][pinstate];
            return state & 0x30;
        }

        bool checkButton() {
            return !digitalRead(buttonPin);

        }

    private:
        uint8_t state;
        uint8_t pin1;
        uint8_t pin2;
        uint8_t buttonPin;
};


RotaryEncoder enc = RotaryEncoder(PIN_ENCODER_CLK, PIN_ENCODER_DT, PIN_ENCODER_SW);
uint8_t mode = MEDIA_CONTROL; 

// debug pin
Adafruit_NeoPixel pixels(1, PIN_NEOPIXEL);
Adafruit_NeoPixel ring = Adafruit_NeoPixel(16, 3, NEO_RGBW);

void setup() {
    // debug
    Serial.begin(9600);
    Serial.println("Attaching interrupts");
    pixels.begin();
    pixels.setPixelColor(0, pixels.Color(255, 255, 0)); // start yellow for disconnected with software
    pixels.show();
    // initialize rotary encoder and attach the interrupts
    attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_CLK), rotate, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_DT), rotate, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_SW), pressed, CHANGE);

    // init keyboard for sending commands to host
    Keyboard.begin();
    /* TrinketHidCombo.begin(); // start the USB device engine and enumerate */

    ring.begin();
    /* ring.setBrightness(255); */
    for (int i = 0; i < 17; i++) {
        ring.setPixelColor(i, 0,0,0,0);
    }
    ring.show();
}

void loop() {
    if (!Serial) { SET_LED_RED; }

    if (Serial.available() > 0) {
        String line = Serial.readStringUntil('\n');
        if (line.compareTo("Are these cowboy times?") == 0) {
            // getting queried from serial- send a response and change to green led
            SET_LED_GREEN;
            Serial.println("hello cowboy");
        }
        else {
            // handle volume percentage for neopixel
            int percent = line.toInt();
            // light 16 / 100 * percent diodes 
            int numLit = (16 * percent) / 100;
            int remainingPercent = (16 * percent) % 100;
            /* Serial.println(numLit); */
            for (int i = 0; i < numLit; i++) {
                ring.setPixelColor(i, 0,0,0,255);
            }
            ring.show();
        }
    }
}

/* unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled */
unsigned long debounceDelay = 100;    // the debounce time; increase if the output flickers
int doublePressTime = 300;
int longPressTime = 800;
unsigned long lastPress = 0;

void pressed() {
    unsigned long now = (unsigned long)millis();
    /* Serial.print(now); */
    bool state = enc.checkButton(); // 1 is pressed
    /* Serial.println(state); */
    /* Serial.println(now); */
    if ((now - lastPress) > debounceDelay) {
        if (state == true) {
            if (now - lastPress < doublePressTime) {
                Serial.println(" Double press");
            }
            else {
                Serial.println(" Single press");
            }
        }
        else {
            if (now - lastPress > longPressTime) {
                Serial.println(" Long press");
            }
            else {
                /* Serial.println(); */
            }
        }
        lastPress = (unsigned long) millis();
    }
}

#define VOLUME_UP 0x80
#define VOLUME_DOWN 0x81
#define NEXT_SONG 0xeb
#define PREV_SONG 0xea

void rotate() {
    if (mode == MEDIA_CONTROL) {
        unsigned char result = enc.process();
        bool button_pushed = enc.checkButton();
        if (result == DIR_CW && !button_pushed) {
            // increase volume
            Serial.println("Increasing volume");
            /* TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_UP); */
            /* Keyboard.print(VOLUME_UP+136); */
            Keyboard.writeRaw(0x78);
        }
        else if (result == DIR_CCW && !button_pushed) {
            // decrease volume
            Serial.println("Decreasing volume");
            /* TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_DOWN); */
            /* Keyboard.print(VOLUME_DOWN+136); */
            Keyboard.writeRaw(0x7F);
        }
        else if (result == DIR_CW && button_pushed) {
            Serial.println("Skipping song");
            /* TrinketHidCombo.pressMultimediaKey(MMKEY_SCAN_NEXT_TRACK); */
            /* Keyboard.print(NEXT_SONG); */
        }
        else if (result == DIR_CCW && button_pushed) {
            Serial.println("Reversing song");
            /* TrinketHidCombo.pressMultimediaKey(MMKEY_SCAN_PREV_TRACK); */
            /* Keyboard.print(PREV_SONG); */
        }
    }
    /* else if (mode == ) */
}

