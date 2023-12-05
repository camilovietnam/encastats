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

class CommandWebServer
{
public:
  static void Begin(void);
private:
  static AsyncWebServer server;
  static void handleRequest(AsyncWebServerRequest *request);
  static void handleCors(AsyncWebServerResponse *response);
  static void handleMovement(AsyncWebServerRequest *response, const char * movement);
};

#endif
