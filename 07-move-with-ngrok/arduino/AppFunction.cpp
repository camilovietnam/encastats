//
// Created by Long  on 18/11/2023.
//

#include <stdio.h>
#include <string.h>
#include "AppFunction.h"
#include "Driver.h"
#include "Motor.h"
#include "Ultrasonic.h"

uint8_t speed = 60;            // motor speed
uint16_t minDistance = 10;    // Min distance forward to obstacle

Driver MyDriver;

/* * * * * * * * * * * * * * * * * */
/* Setup the car app * * * * * * * */
/* * * * * * * * * * * * * * * * * */
void AppFunction::Init(void) {
    Serial.begin(9600);
    MyDriver.Init();
    ultrasonic.Init();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * */
/* Move the car in a given direction * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * */
void AppFunction::MoveTheCar(uint8_t movement){
    switch (movement) {
        case btnForward:
            // do not move forward unless there are no obstacles
            if (hasObstacles()) {
                Serial.println("no movement will occur due to obstacles");
                return;
            }

            AppMotor.GoForward(speed);
            break;
        case btnBackward:
            AppMotor.GoBackward(speed);
            break;
        case btnLeft:
            AppMotor.TurnLeft(speed);
            break;
        case btnRight:
            AppMotor.TurnRight(speed);
            break;
        case btnStop:
            AppMotor.Stop();
            break;
        case btnModeFree:
            Serial.println("Mode 1");
            break;
        case bntModeLearn:
            Serial.println("Mode 2");
            break;
        case btnModeRepeat:
            Serial.println("Mode 3");
            break;
        case Mode4:
            Serial.println("Mode 4");
            break;
        case 10:
            Serial.println("Mode 5");
            break;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * */
/* Read a button from the controller * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * */
uint8_t AppFunction::ReceiveCommandFromController(void) {
    uint8_t Button_pressed = 100;

    static bool data_received = false;

    if (MyDriver.Get(&Button_pressed)) {
        data_received = true;
        // Serial.println(Button_pressed);
    }

    if (true == data_received) {
        MoveTheCar(Button_pressed);
    }

    return Button_pressed;
}


/* * * * * * * * * * * * * * * */
/* Stop the car  * * * * * * * */
/* * * * * * * * * * * * * * * */
void AppFunction::StopTheCar(void) {
    AppMotor.Stop();
}


/* * * * * * * * * * * * * * * * * * * * * * */
/* Check for obstacles with ultrasound   * * */
/* * * * * * * * * * * * * * * * * * * * * * */
bool AppFunction::hasObstacles(void) {
    uint16_t distance = ultrasonic.Get();
    if (distance < minDistance) {
        Serial.println("there is an obstacle ahead. Min distance is: " + String(minDistance) + ", current: " + String(distance));
        return true;
    }

    return distance < minDistance;
}

void AppFunction::ChangeSpeed(uint8_t newSpeed) {
  speed = newSpeed;
  Serial.println("speed was set to: " + String(newSpeed));
}