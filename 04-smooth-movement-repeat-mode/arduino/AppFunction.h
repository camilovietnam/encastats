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
    LeftForward,   //(5)
    LeftBackward,  //(6)
    RightForward,  //(7)
    RightBackward, //(8)
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
