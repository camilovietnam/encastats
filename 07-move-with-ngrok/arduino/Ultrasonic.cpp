//
// Created by Long  on 05/12/2023.
//

#include "Ultrasonic.h"

void Ultrasonic::Init(void) {
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
}

uint16_t Ultrasonic::Get(void){
  unsigned int distance = 0;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned int microseconds = (unsigned int)pulseIn(ECHO_PIN, HIGH);
  
  distance = (microseconds / 58);
  Serial.println("microseconds: " + String(microseconds));
  Serial.println("distance: " + String(distance));

  return distance;
}