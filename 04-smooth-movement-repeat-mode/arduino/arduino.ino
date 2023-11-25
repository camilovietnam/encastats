#include <avr/wdt.h>
#include "Driver.h"
#include "AppFunction.h"
#include "Timeout.h"

// Variables for all Modes
bool modeLoopedAtLeastOnce = false;

// Variables for Free Mode
unsigned long startTime = 0;
const unsigned int delayForShortMovement = 350;  // 500 ms for spinning left or right
const unsigned int delayForLongMovement = 750;   // 700 ms for forward and backward
bool countingDown = false;
uint8_t lastButtonPressed;
Timeout beginningOfMovement;

// Variables for Learn Mode
const int maxHistorySize = 10;
uint8_t buttonHistory[maxHistorySize];
int historyIndex = 0;

enum Modes
{ 
    modeFree = 6,   //(1) Freedom of movement
    modeLearn,      //(2) Freedom of movement - but the movements are stored in an array
    modeRepeat,     //(3) The movements are repeated from the array
};

uint8_t carMode = modeFree;    // we begin in FREE MODE

void setup() {
  MyApp.Init();
  Serial.println(" > > > > > > > > > > > > > > > > > > > > >");
  Serial.println(" > > > > > > > > > > > > > > > > > > > > >");
  Serial.println(" > > > > ELEGOO CAR  > > > > > > > > > > >");
  Serial.println(" > > > > > > > > > > > > > > > > > > > > >");
  Serial.println(" > > > > > > > > > > > > > > > > > > > > >");

  Serial.println("The car will start in Free Mode.");
}

void loop() {
  switch (carMode) {
    case modeFree:
      loopModeFree();
      break;
    case modeLearn:
      loopModeLearn();
      break;
    case modeRepeat:
      loopModeRepeat();
      break;
  }  
}

void loopModeFree() {
  unsigned long currentTime = millis();

  if (!countingDown) {
    lastButtonPressed = MyApp.ReceiveCommandFromController(); 

    if (isModeChange(lastButtonPressed)) {
      MyApp.StopTheCar();
      carMode = lastButtonPressed;
      modeLoopedAtLeastOnce = false;
      return;
    } else if (isValidMovement(lastButtonPressed)) {
      startTime = millis();
      countingDown = true;
      beginningOfMovement.Reset();
    }
  }

  switch (lastButtonPressed) {
      case btnForward: 
      case btnBackward:
        if (beginningOfMovement.HasElapsed(delayForLongMovement)) {
          // stop the car
          countingDown = false;
          lastButtonPressed = 100;
          MyApp.StopTheCar();
        }
        break;
      case btnLeft:
      case btnRight:
        if (beginningOfMovement.HasElapsed(delayForShortMovement)) {
          // stop the car
          countingDown = false;
          lastButtonPressed = 100;
          MyApp.StopTheCar();
        }
        break;
      default:
        // todo: I think this is not ne
        countingDown = false;
        break;
  }  
}

void loopModeLearn() {
  if (!modeLoopedAtLeastOnce) {
    // This is the first loop. Reset the history
    modeLoopedAtLeastOnce = true;
    historyIndex = 0;
  }

  unsigned long currentTime = millis();

  if (!countingDown) {
    lastButtonPressed = MyApp.ReceiveCommandFromController(); 

    if (isModeChange(lastButtonPressed)) {
      Serial.print("Changed mode LEARN -> ");
      Serial.println(lastButtonPressed);
      MyApp.StopTheCar();
      carMode = lastButtonPressed;
      modeLoopedAtLeastOnce = false;
      return;
    } else if (isValidMovement(lastButtonPressed)) {
      // store the movement in the button history, if there is enough space
      if (historyIndex < maxHistorySize) {
        buttonHistory[historyIndex] = lastButtonPressed;
        historyIndex++;
        Serial.print("Saved movement: ");
        Serial.println(lastButtonPressed);
      } else {
        Serial.println("You used all the memory for buttons. Go to Repeat mode now.");
      }

      startTime = millis();
      countingDown = true;
    }
  }

  // stop after a given timeout for each type of movement
  switch (lastButtonPressed) {
      case btnForward: 
      case btnBackward:
        if (currentTime - startTime > delayForLongMovement) {
          // stop the car
          Serial.println("Learn Stop the car 1");
          countingDown = false;
          lastButtonPressed = 100;
          MyApp.StopTheCar();
        }
        break;
      case btnLeft:
      case btnRight:
        if (currentTime - startTime > delayForShortMovement) {
          // stop the car
          Serial.println("Learn Stop the car 2");
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
    // Allow reading a new button command
    lastButtonPressed = MyApp.ReceiveCommandFromController(); 

    if (isModeChange(lastButtonPressed)) {
      Serial.print("Changed mode REPEAT -> ");
      Serial.println(lastButtonPressed);
      MyApp.StopTheCar();
      carMode = lastButtonPressed;
      modeLoopedAtLeastOnce = false;
      return;
    }

  if (carMode == modeRepeat && !modeLoopedAtLeastOnce) {
    Serial.println("Enter loop repeat");
    Serial.print("Buttons to repeat: ");
    for (int i = 0; i < historyIndex; ++i) {
        Serial.print(buttonHistory[i]);
        Serial.print(" ");
    }
    Serial.println(); 

    // historyIndex = 0; This would enable printing only once
    
    modeLoopedAtLeastOnce = true;
  }
}


bool isValidMovement(uint8_t buttonPressed) {
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