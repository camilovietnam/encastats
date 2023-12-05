/*
 * @Descripttion: 
 * @version: 
 * @Author: Elegoo
 * @Date: 2020-06-04 11:42:27
 * @LastEditors: Changhua
 * @LastEditTime: 2020-07-23 14:21:48
 */

#ifndef _CameraWebServer_AP_H
#define _CameraWebServer_AP_H

#include "camera_pins.h"
#include "esp_system.h"
#include "esp_camera.h"

#include <WiFi.h>
#include <HTTPClient.h>

#include <iostream>
#include <sstream>
#include <string>

class CameraWebServer_AP
{

public:
  void CameraWebServer_AP_Init(void);
  void PingWorker(void);
  String wifi_name;  
private:
  void connectToWifi();
};

#endif
