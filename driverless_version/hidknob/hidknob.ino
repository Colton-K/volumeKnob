#include <Keyboard.h>
#include <HID-Project.h>
/* #include "./TrinketHidCombo/TrinketHidCombo.h" */

#define PIN_SWITCH 0
#define PIN_ENCODER_A 2
#define PIN_ENCODER_B 1

bool switchState = HIGH;
bool encoderAState = HIGH;
bool encoderBState = HIGH;
bool previousSwitchState = HIGH;
bool previousEncoderAState = HIGH;
bool previousEncoderBState = HIGH;
bool canClick = true;

// the setup routine runs once when you press reset:
void setup() {
    /* pinMode(PIN_SWITCH, INPUT); */
    /* pinMode(PIN_ENCODER_A, INPUT); */
    /* pinMode(PIN_ENCODER_B, INPUT); */

    /* digitalWrite(PIN_SWITCH, HIGH); */
    /* digitalWrite(PIN_ENCODER_A, HIGH); */
    /* digitalWrite(PIN_ENCODER_B, HIGH); */

    TrinketHidCombo.begin(); // start the USB device engine and enumerate

    /* TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_DOWN); */
    /* encoderAState = digitalRead(PIN_ENCODER_A); */
    /* encoderBState = digitalRead(PIN_ENCODER_B); */
    /* previousSwitchState = switchState; */
    /* previousEncoderAState = encoderAState; */
    /* previousEncoderBState = encoderBState; */
}

/* // the loop routine runs over and over again forever: */
/* void loop() { */
/*     bool wasChanged = false; */

/*     previousSwitchState = switchState; */
/*     previousEncoderBState = encoderBState; */
/*     previousEncoderAState = encoderAState; */

/*     bool switchStateReading = digitalRead(PIN_SWITCH); */
/*     bool encoderAStateReading = digitalRead(PIN_ENCODER_A); */
/*     bool encoderBStateReading = digitalRead(PIN_ENCODER_B); */

/*     delayMicroseconds(750); */

/*     if(digitalRead(PIN_SWITCH) == switchStateReading && switchStateReading != switchState) */
/*     { */
/*     switchState = switchStateReading; */
/*     wasChanged = true; */
/*     if(switchState == LOW) */
/*     { */
/*     canClick = true; */
/*     } */
/*     } */

/*     if(digitalRead(PIN_ENCODER_A) == encoderAStateReading && encoderAStateReading != encoderAState) */
/*     { */
/*     encoderAState = encoderAStateReading; */
/*     wasChanged = true; */
/*     } */

/*     if(digitalRead(PIN_ENCODER_B) == encoderBStateReading && encoderBStateReading != encoderBState) */
/*     { */
/*     encoderBState = encoderBStateReading; */
/*     wasChanged = true; */
/*     } */

/*     if(wasChanged == true) */
/*     { */
/*     if(encoderAState == LOW && encoderBState == LOW) */
/*     { */
/*     if(previousEncoderAState == HIGH && previousEncoderBState == LOW) */
/*     left(switchState); */
/*     if(previousEncoderAState == LOW && previousEncoderBState == HIGH) */
/*     right(switchState); */
/*     } */

/*     if(encoderAState == HIGH && encoderBState == HIGH) */
/*     {  */
/*       if(previousEncoderAState == LOW && previousEncoderBState == HIGH)       */
/*         left(switchState); */
/*       if(previousEncoderAState == HIGH && previousEncoderBState == LOW) */
/*         right(switchState);        */
/*     } */

/*     if(switchState == HIGH && previousSwitchState == LOW)     */
/*       click();     */

/*     } */
/*     TrinketHidCombo.poll(); */
/*     delayMicroseconds(1000); */
/* } */

/* void left(bool switchState){ */
/*     canClick = false; */
/*     if(switchState) */
/*     TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_DOWN); */
/*     else */
/*     TrinketHidCombo.pressMultimediaKey(MMKEY_SCAN_PREV_TRACK); */
/* } */

/* void right(bool switchState){ */
/*     canClick = false; */
/*     if(switchState) */
/*     TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_UP); */
/*     else */
/*     TrinketHidCombo.pressMultimediaKey(MMKEY_SCAN_NEXT_TRACK); */
/* } */

/* void click(){ */
/*     if(canClick) */
/*     TrinketHidCombo.pressMultimediaKey(MMKEY_PLAYPAUSE);//MMKEY_MUTE */
/* } */
