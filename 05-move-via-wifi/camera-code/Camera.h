//
// Created by Long  on 05/12/2023.
//

#ifndef CAMERA_H
#define CAMERA_H

#define CAMERA_MODEL_M5STACK_WIDE

#include "esp_camera.h"
#include <WiFi.h>
#include "camera_pins.h"
#include "esp_system.h"

class Camera {
  public:
    static void Init(void);
    static void takePhoto(void);
  private:
    static camera_config_t loadConfig(void);
};

#endif //CAMERA_H
