#include <avr/wdt.h>
#include "Driver.h"
#include "AppFunction.h"

unsigned long startTime = 0;
const unsigned long stopDelayShort = 3000;  // 500 ms for spinning left or right
const unsigned long stopDelayLong = 3000;   // 700 ms for forward and backward
bool countingDown = false;
uint8_t lastButtonPressed;

enum Modes
{ 
    modeFree = 6,   //(1)
    modeLearn,      //(2)
    modeRepeat,     //(3)
};

uint8_t carMode = modeFree;    // we begin in FREE MODE

void setup() {
  MyApp.Init();
}

void loop() {
  switch (carMode) {
    case modeFree:
      loopModeFree();
    case modeLearn:
      loopModeLearn();
    case modeRepeat:
      loopModeRepeat();
  }  
}

void loopModeFree() {
  unsigned long currentTime = millis();

  if (!countingDown) {
    lastButtonPressed = MyApp.ReceiveCommandFromController(); 

    if (isModeChange(lastButtonPressed)) {
      MyApp.StopTheCar();
      carMode = lastButtonPressed;
      return;
    } else if (isValidButton(lastButtonPressed)) {
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

void loopModeLearn() {
  unsigned long currentTime = millis();

  if (!countingDown) {
    lastButtonPressed = MyApp.ReceiveCommandFromController(); 

    if (isModeChange(lastButtonPressed)) {
      MyApp.StopTheCar();
      carMode = lastButtonPressed;
      return;
    } else if (isValidButton(lastButtonPressed)) {
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

void loopModeRepeat() {
  unsigned long currentTime = millis();

  if (!countingDown) {
    lastButtonPressed = MyApp.ReceiveCommandFromController(); 

    if (isModeChange(lastButtonPressed)) {
      MyApp.StopTheCar();
      carMode = lastButtonPressed;
      return;
    } else if (isValidButton(lastButtonPressed)) {
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
  }

  return false;
}

bool isModeChange(uint8_t buttonPressed) {
  switch (buttonPressed) {
    case btnModeFree:
    case bntModeLearn:
    case btnModeRepeat:
      return true;
  }

  return false;
}