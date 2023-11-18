//
// Created by Long  on 18/11/2023.
//

#include <hardwareSerial.h>
#include <stdio.h>
#include <string.h>
#include "AppFunction.h"
#include "Driver.h"

// #include "ArduinoJson-v6.11.1.h" //ArduinoJson

#define _is_print 1
#define _Test_print 0

Driver MyDriver;

// never used anymore
// enum CarModes {
//     Mode_1,
//     Mode_2,
//     Mode_3,
//     Mode_4,
//     Mode_5,
//     Mode_6,
// };

// struct Application {
//     CarModes Car_Mode;
// };

// Application ElegooCar;

void AppFunction::Init(void) {
    bool res_error = true;
    Serial.begin(9600);
    MyDriver.Init();
}

void AppFunction::Receive(void) {
    uint8_t Button_pressed;
    static bool data_received = false;

    if (MyDriver.Get(&Button_pressed)) {
        data_received = true;
    }

    if (true == data_received) {
        switch (Button_pressed) {
            case 1:
                Serial.println("Up");
                break;
            case 2:
                Serial.println("Down");
                break;
            case 3:
                Serial.println("Left");
                break;
            case 4:
                Serial.println("Right");
                break;
            case 5:
                Serial.println("Ok");
                break;
            case 6:
                Serial.println("Mode 1");
                break;
            case 7:
                Serial.println("Mode 2");
                break;
            case 8:
                Serial.println("Mode 3");
                break;
            case 9:
                Serial.println("Mode 4");
                break;
            case 10:
                Serial.println("Mode 5");
                break;
        }
    }
}




