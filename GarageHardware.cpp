#if 0
#include"GarageHardware.h"

// TODO: Define any variables or constants here
const int red = D1;
const int green = D2;
const int blue = D3;
const int button = D4;
const int openButton = D5;
const int closeButton = D6;
const int faultWire = D0;
const int garageLight = D7;
/**
 * Setup the door hardware (all I/O should be configured here)
 *
 * This routine should be called only once from setup()
 */
void setupHardware() {
    pinMode(button, INPUT_PULLUP);
    pinMode(openButton, INPUT_PULLUP);
    pinMode(closeButton, INPUT_PULLUP);
    pinMode(faultWire, INPUT_PULLUP);

    pinMode(garageLight, OUTPUT);
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(blue, OUTPUT);

    analogWrite(red, 255);
    analogWrite(green, 255);
    analogWrite(blue, 0);
    digitalWrite(garageLight, LOW);
}

/**
 * Return true if the door open/close button is pressed
 *
 * Note: this is directly based on hardware.  No debouncing or
 *       other processing is performed.
 *
 * return  true if buttons is currently pressed, false otherwise
 */
bool isButtonPressed() {
    if(digitalRead(button) == 0) {
        return true;
    }
    return false;
}

/**
 * Return true if the door is fully closed
 *
 * Note: This is directly based on hardware.  No debouncing or
 *       other processing is performed.
 *
 * return  true if the door is completely closed, false otherwise
 */
bool isDoorFullyClosed() {
    if(digitalRead(closeButton) == 0) {
        return true;
    }
    return false;
}

/**
 * Return true if the door has experienced a fault
 *
 * Note: This is directly based on hardware.  No debouncing or
 *       other processing is performed.
 *
 * return  true if the door is has experienced a fault
 */
bool isFaultActive() {
    if(digitalRead(faultWire) == 0) {
        return true;
    }
    return false;
}

/**
 * Return true if the door is fully open
 *
 * Note: This is directly based on hardware.  No debouncing or
 *       other processing is performed.
 *
 * return  true if the door is completely open, false otherwise
 */
bool isDoorFullyOpen() {
    if(digitalRead(openButton) == 0) {
        return true;
    }
    return false;
}

/**
 * This function will start the motor moving in a direction that opens the door.
 *
 * Note: This is a non-blocking function.  It will return immediately
 *       and the motor will continue to opperate until stopped or reversed.
 *
 * return void
 */
void startMotorOpening() {
    analogWrite(red, 0);
    analogWrite(green, 255);
    analogWrite(blue, 255);
    digitalWrite(garageLight, HIGH);
}

/**
 * This function will start the motor moving in a direction closes the door.
 *
 * Note: This is a non-blocking function.  It will return immediately
 *       and the motor will continue to opperate until stopped or reversed.
 *
 * return void
 */
void startMotorClosing() {
    analogWrite(red, 255);
    analogWrite(green, 0);
    analogWrite(blue, 255);
    digitalWrite(garageLight, HIGH);
}

/**
 * This function will stop all motor movement.
 *
 * Note: This is a non-blocking function.  It will return immediately.
 *
 * return void
 */
void stopMotor() {
    analogWrite(red, 255);
    analogWrite(green, 255);
    analogWrite(blue, 0);
    digitalWrite(garageLight, LOW);
}

/**
 * This function will control the state of the light on the opener.
 *
 * Parameter: on: true indicates the light should enter the "on" state;
 *                false indicates the light should enter the "off" state
 *
 * Note: This is a non-blocking function.  It will return immediately.
 *
 * return void
 */
void setLight(boolean on) {
    if(on == true) {
        digitalWrite(garageLight, HIGH);
    } else {
        digitalWrite(garageLight, LOW);
    }
}


/**
 * This function will control the state of the light on the opener.
 * (OPTIONAL:  This is only needed for the extra credit part of assignment 3)
 *
 * Parameter: cycle (0-100).  0 indicates completely Off, 100 indicates completely on.
 *            intermediate values are the duty cycle (as a percent)
 *
 * Note: This is a non-blocking function.  It will return immediately.
 *
 * return void
 */
void setLightPWM(int cyclePct) {
  // TODO: Your code to simulate the light
  //       Use an individual LED
}
#endif
