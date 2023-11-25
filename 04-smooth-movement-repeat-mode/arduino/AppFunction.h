//
// Created by Long  on 18/11/2023.
//

#ifndef _APP_FUNCTION_H
#define _APP_FUNCTION_H
#include <arduino.h>

enum ButtonCommands
{ 
    btnForward = 1,   //(1)
    btnBackward,      //(2)
    btnLeft,          //(3)
    btnRight,         //(4)
    btnStop,           //(9)
    btnModeFree,   //(5)
    bntModeLearn,  //(6)
    btnModeRepeat,  //(7)
    Mode4, //(8)
};

class AppFunction {
    public:
        void Init(void);
        uint8_t ReceiveCommandFromController(void);
        void MoveTheCar(uint8_t movement);
        void StopTheCar(void);
};

extern AppFunction MyApp;
#endif //_APP_FUNCTION_H
