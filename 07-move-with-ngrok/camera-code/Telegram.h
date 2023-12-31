#ifndef _TELEGRAM_H
#define _TELEGRAM_H

#include "esp_camera.h"
#include <string>
#include <HTTPClient.h>
#include "credentials.h"
#include <sstream>

class Telegram {
  public:
    bool SendPhoto(camera_fb_t*);
};
#endif