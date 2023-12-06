//
// Created by Long  on 05/12/2023.
//

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>

class Ultrasonic {
  public:
    static void Init(void);
    static uint16_t Get(void);
  private:
    #define ECHO_PIN 12
    #define TRIG_PIN 13

    #define MAX_DISTANCE 200
};

extern Ultrasonic ultrasonic;
#endif //ULTRASONIC_H
