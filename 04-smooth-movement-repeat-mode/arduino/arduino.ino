#include <avr/wdt.h>
#include "Driver.h"
#include "AppFunction.h"

unsigned long startTime = 0;
const unsigned long stopDelayShort = 500;  // 500 ms for spinning left or right
const unsigned long stopDelayLong = 700;   // 700 ms for forward and backward
bool countingDown = false;
uint8_t lastButtonPressed;

void setup() {
  MyApp.Init();
}

void loop() {
  unsigned long currentTime = millis();

  if (!countingDown) {
    lastButtonPressed = MyApp.ReceiveCommandFromController(); 
    if (isValidButton(lastButtonPressed)) {
      startTime = millis();
      countingDown = true;
    }
  }

  switch (lastButtonPressed) {
      case btnForward: 
      case btnBackward:
        if (currentTime - startTime > stopDelayLong) {
          // stop the car
          countingDown = false;
          lastButtonPressed = 100;
          MyApp.StopTheCar();
        }
        break;
      case btnLeft:
      case btnRight:
        if (currentTime - startTime > stopDelayShort) {
          // stop the car
          countingDown = false;
          lastButtonPressed = 100;
          MyApp.StopTheCar();
        }
        break;
      default:
        countingDown = false;
        break;
  }  
}

bool isValidButton(uint8_t buttonPressed) {
  switch (buttonPressed) {
    case btnForward:
    case btnBackward:
    case btnLeft:
    case btnRight:
    case btnStop:
      return true;
      break;
  }

  return false;
}