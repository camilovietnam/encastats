/*
 * @Description:  A web server that accepts requests and notifies the Serial port 2
 * @version: 
 * @Author: long
 * @Date: 2023-12-04
 * @LastEditors: long
 * @LastEditTime: 2012-12-04
 */
#ifndef _CommandWebServer_H
#define _CommandWebServer_H

#include <ESPAsyncWebSrv.h>
#include "Camera.h"

class CommandWebServer
{
public:
  static void Begin();
private:
  static AsyncWebServer server;
  static void handleRequest(AsyncWebServerRequest *request);
  static void handleCors(AsyncWebServerResponse *response);
  static void handleCapture(AsyncWebServerRequest *request);
};

#endif
