// This #include statement was automatically added by the Particle IDE.
#include "GarageHardware.h"

// RED indicates that the garage door is OPENED
// GREEN indicates that the garage door is CLOSED
// BLUE indicates that the garage door is in FAULT
// PURPLE indicates that the garage door is MOVING
// D7 LIGHT indicates the garage door LIGHT

enum ButtonStates {idle, pressed, held};
ButtonStates buttonState = idle;

boolean temp = false;
boolean buttonRelease = false;

String status = "";

const unsigned long MIN_BUTTON_PRESS_TIME = 150;

unsigned long previousTime = 0;
unsigned long faultTime = 0;

unsigned long previousLight = 0;
unsigned long lightTime = 3000;

unsigned long autoClose = 0;

void setup() {
    setupHardware();
    Serial.begin(9600);

    initialState();

    //Particle.function("dTimer", dTimer);
    Particle.function("afterTimer", toClosingAfterTimer);
    Particle.function("toClosing", toClosing);
    Particle.function("toClosed", toClosed);
    Particle.function("toOpened", toOpened);
    Particle.function("toOpening", toOpening);
    Particle.function("writeStatus", writeStatus);
    Particle.function("toFault", toFault);

    Particle.subscribe("garagedoor/button", remoteButtonCheck);
}

boolean remoteButton = false;

bool remoteButtonCheck(String eventName, String data) {
    if(data == "pressed"){
        remoteButton = true;
    }
}

void initialState() {
    if(isDoorFullyClosed() == true) {
        status = "closed";
    } else if(isDoorFullyOpen() == true) {
        status = "opened";
    } else {
        status = "faultOpen";
    }
}

int writeStatus(String x) {
    if(status == "closed") {
        Particle.publish("garageClosed", "garageClosed");
    } else if(status == "opened") {
        Particle.publish("garageOpened", "garageOpened");
    } else if(status == "faultOpen") {
        Particle.publish("faultOpen", "faultOpen");
    }
}




int toClosingAfterTimer(String data){
    int val = data.toInt();
    int closeTiming = 1000*val;

    autoClose = millis();
    while(millis() - autoClose < closeTiming) {
        stopMotorIdle();

    }
    motorIsClosing();
}

int toClosing(String x) {
    motorIsClosing();
}

int toClosed(String x){
    closeFully();
}

int toOpening(String x){
    motorIsOpening();
}

int toOpened(String x){
    openFully();
}

int toFault(String x){
    stopMotorIdle();
}



void loop() {
    //below are the functions that change the status of the door (ie. changes when button pressed..... these should
    //be forced to be called by the js inorder to change the door status)

    if(status == "faultClose" && (isButtonHeld() == true || remoteButton == true)) { // in fault state

        motorIsOpening();
        remoteButton = false;

    }
    else if (status == "faultOpen" && (isButtonHeld() == true || remoteButton == true)) { // in fault state

        motorIsClosing();
        remoteButton = false;

    }
    if(status == "closed" && (isButtonHeld() == true || remoteButton == true)) { // in fault state

        motorIsOpening();
        remoteButton = false;

    }
    else if (status == "opened" && (isButtonHeld() == true || remoteButton == true)) { // in fault state

        motorIsClosing();
        remoteButton = false;

    }
    else if(status =="opening" && (isFaultActive()== true || (isButtonHeld() == true || remoteButton == true))){ // goes into fault state

        stopMotorIdle();
        remoteButton = false;

    }
    else if(status =="closing" && (isFaultActive()== true || (isButtonHeld() == true || remoteButton == true))){ // goes into fault state

        stopMotorIdle();
        remoteButton = false;

    }
    else if(status == "opening" && isDoorFullyOpen() == true){  // open the door fully

        openFully();

    }
    else if(status == "closing" && isDoorFullyClosed() == true){ // closes the door fully

        closeFully();
    }
}
//THESE ARE OUR OWN FUNCTIONS

boolean isButtonHeld() {

    switch(buttonState) {

        case idle:
            if(isButtonPressed() == true) {
                previousTime = millis();
                temp = true;
                buttonState = pressed;
            }
            break;

        case pressed:
            if(isButtonPressed() != true) {
                buttonRelease = true;
                buttonState = idle;
                break;
            }

            if(((millis() - previousTime) > MIN_BUTTON_PRESS_TIME) && isButtonPressed() == true && temp == true) {
                temp = false;
                buttonRelease = false;
                faultTime = millis();
                return true;
            }
            break;
    }
    return false;
}

void openFully() {
    Particle.publish("garageOpened", "garageOpened");
    status = "opened";

    stopMotor();

    previousLight = millis();
    while(millis() - previousLight < lightTime) {
        setLight(true);
    }
    setLight(false);
}

void motorIsOpening() {
    status = "opening";
    Particle.publish("motorOpening", "motorOpening");

    setLight(true);
    startMotorOpening();
}


void closeFully() {
    Particle.publish("garageClosed", "garageClosed");
    status = "closed";

    stopMotor();

    previousLight = millis();
    while(millis() - previousLight < lightTime) {
        setLight(true);
    }
    setLight(false);
}

void motorIsClosing() {
    status = "closing";
    Particle.publish("motorClosing", "motorClosing");

    setLight(true);
    startMotorClosing();
}

void stopMotorIdle() {

    stopMotor();

    previousLight = millis();
    while(millis() - previousLight < lightTime) {
        setLight(true);
    }
    setLight(false);

    if(status == "opening"){
            status = "faultOpen";
            Particle.publish("faultOpen", "faultOpen");
    }
    else if(status == "closing"){
            status = "faultClose";
            Particle.publish("faultClose", "faultClose");
    }
