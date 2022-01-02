#include <Adafruit_NeoPixel.h> // for indicator rgb
#include "HID-Project.h"

#define PIN_ENCODER_CLK 0
#define PIN_ENCODER_DT 1
#define PIN_ENCODER_SW 2

#include "RotaryEncoder.h"
RotaryEncoder enc = RotaryEncoder(PIN_ENCODER_CLK, PIN_ENCODER_DT, PIN_ENCODER_SW);
uint8_t mode = MEDIA_CONTROL; 
unsigned long lastPress = 0; // make var global for pressed function

// for leds
#ifdef PIN_NEOPIXEL
#define SET_LED_OFF pixels.setPixelColor(0, 0,0,0,0); pixels.show();
#define SET_LED_RED pixels.setPixelColor(0, 255,0,0,0); pixels.show();
#define SET_LED_GREEN pixels.setPixelColor(0, 0,255,0,0); pixels.show();
Adafruit_NeoPixel pixels(1, PIN_NEOPIXEL);
#endif
#define SET_RING_OFF ring.clear(); ring.show();
Adafruit_NeoPixel ring = Adafruit_NeoPixel(16, 3, NEO_RGBW);

// for displaying volume percent
int percent = 0;

void setup() {
    // debug
    Serial.begin(9600);
    Serial.println("Attaching interrupts");
    #ifdef PIN_NEOPIXEL
    pixels.begin();
    pixels.setBrightness(20);
    pixels.setPixelColor(0, pixels.Color(255, 255, 0)); // start yellow for disconnected with software
    pixels.show();
    #endif
    // initialize rotary encoder and attach the interrupts
    attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_CLK), rotate, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_DT), rotate, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_SW), pressed, CHANGE);

    // init keyboard for sending commands to host
    Consumer.begin();
    Keyboard.begin();

    ring.begin();
    ring.setBrightness(20);
    for (int i = 0; i < 17; i++) {
        ring.setPixelColor(i, 0,0,0,0);
    }
    ring.show();
}

void loop() {
    #ifdef PIN_NEOPIXEL
    if (!Serial) { SET_LED_RED; }
    #endif    
    if (Serial.available() > 0) {
        String line = Serial.readStringUntil('\n');
        if (line.compareTo("Are these cowboy times?") == 0) {
            // getting queried from serial- send a response and change to green led

            #ifdef PIN_NEOPIXEL
            SET_LED_GREEN;
            Serial.println("hello cowboy");
            #endif
        }
        else if (mode == MEDIA_CONTROL) {
            // get volume
            if (line.compareTo("") != 0) {
                percent = line.toInt();
            }
            int remainingPercent = (16 * percent) % 100;
            int numLit = (16 * percent) / 100;
            for (int i = 0; i < 16; i++) {
                if (i < numLit) {
                    ring.setPixelColor(i, 0,0,0,255);
                }
                else if (i == numLit) {
                    ring.setPixelColor(i, 0,0,0,remainingPercent);
                }
                else {
                    ring.setPixelColor(i, 0,0,0,0);
                }
            }
            ring.show();
        }
    }
    else {
        if (mode == MEDIA_CONTROL) {
            for (int i = 0; i < 16; i++) {
                ring.setPixelColor(i, 0,255,0,0);
            }
            ring.show();            
        }
        else if (mode == SCROLLING) {
            for (int i = 0; i < 16; i++) {
                ring.setPixelColor(i, 0,0,0,255);
            }
            ring.show();
        }
    }
}


void pressed() {
    unsigned long now = (unsigned long)millis();
    bool state = enc.checkButton(); // 1 is pressed
    if ((now - lastPress) > DEBOUNCE_DELAY) {
        if (state == true) {
            if ((now - lastPress) < DOUBLE_PRESS_TIME) {
                if (mode == MEDIA_CONTROL) {
                  Consumer.press(MEDIA_PLAY_PAUSE);
                  Consumer.release(MEDIA_PLAY_PAUSE);
                  lastPress = 0;
                }
            }
            else {
                // Serial.println(" Single press");
            }
        }
        else {
            if (now - lastPress > LONG_PRESS_TIME ) {
                /* Serial.println("Long press"); */
                SET_RING_OFF;
                if (mode == MEDIA_CONTROL) {
                    mode = SCROLLING;
                }
                else if (mode == SCROLLING) {
                    mode = MEDIA_CONTROL;
                }
            }
        }
        lastPress = now;
    }

    
}

void rotate() {
    unsigned char result = enc.process();
    bool button_pushed = enc.checkButton();
        
    if (mode == MEDIA_CONTROL) {
        if (result == DIR_CW && !button_pushed) {
             Serial.println("Increasing volume");
            // if consumer ever stops working, try using Keyboard.write(KEY_VOLUME_UP);
            // https://github.com/NicoHood/HID/blob/master/src/KeyboardLayouts/ImprovedKeylayouts.h#L61
            Consumer.press(MEDIA_VOLUME_UP);
            Consumer.release(MEDIA_VOLUME_UP);
        }
        else if (result == DIR_CCW && !button_pushed) {
             Serial.println("Decreasing volume");
            Consumer.press(MEDIA_VOLUME_DOWN);
            Consumer.release(MEDIA_VOLUME_DOWN);
        }
        else if (result == DIR_CW && button_pushed) {
            // Serial.println("Skipping song");
            Consumer.press(MEDIA_NEXT);
            Consumer.release(MEDIA_NEXT);
          
        }
        else if (result == DIR_CCW && button_pushed) {
            // Serial.println("Reversing song");
            Consumer.press(MEDIA_PREVIOUS);
            Consumer.release(MEDIA_PREVIOUS);
        }
    }
    else if (mode == SCROLLING) {
        if (result == DIR_CW && !button_pushed) {
            // scroll down
            for (int i = 0; i < NUM_LINES_PER_SCROLL; i++) {
                Keyboard.write(KEY_DOWN_ARROW);              
            }
            lastPress = millis();
        }
        else if (result == DIR_CCW && !button_pushed) {
            // scroll up
            for (int i = 0; i < NUM_LINES_PER_SCROLL; i++) {
                Keyboard.write(KEY_UP_ARROW);              
            }
            lastPress = millis();
        }
        else if (result == DIR_CW && button_pushed) {
            // scroll right
            Keyboard.write(KEY_RIGHT_ARROW);
            lastPress = millis();
        }
        else if (result == DIR_CCW && button_pushed) {
            // scroll left
            Keyboard.write(KEY_LEFT_ARROW);
            lastPress = millis();
        }        
    }
}
