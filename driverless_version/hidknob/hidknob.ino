#include "HID-Project.h"
#include "RotaryEncoder.h"

// TODO: enter pin config 
#define PIN_ENCODER_CLK 0
#define PIN_ENCODER_DT 1
#define PIN_ENCODER_SW 2

void setup() {
    // debug
    Serial.begin(9600);
    Serial.println("Attaching interrupts");

    // initialize rotary encoder and attach the interrupts
    attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_CLK), rotate, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_DT), rotate, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_SW), pressed, CHANGE);

    // be ready to send things
    Consumer.begin();
    Keyboard.begin();
}

void loop() {
}


RotaryEncoder enc = RotaryEncoder(PIN_ENCODER_CLK, PIN_ENCODER_DT, PIN_ENCODER_SW);
uint8_t mode = MEDIA_CONTROL; 

unsigned long lastPress = 0; // make var global for pressed function

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
                // Serial.println(" Long press");
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
            // Serial.println("Increasing volume");
            // if consumer ever stops working, try using Keyboard.write(KEY_VOLUME_UP);
            // https://github.com/NicoHood/HID/blob/master/src/KeyboardLayouts/ImprovedKeylayouts.h#L61
            Consumer.press(MEDIA_VOLUME_UP);
            Consumer.release(MEDIA_VOLUME_UP);
        }
        else if (result == DIR_CCW && !button_pushed) {
            // Serial.println("Decreasing volume");
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
