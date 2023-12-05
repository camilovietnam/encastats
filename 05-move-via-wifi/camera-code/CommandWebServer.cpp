/*
 * @Description:  A web server that accepts requests and notifies the Serial port 2
 * @version: 
 * @Author: long
 * @Date: 2023-12-04
 * @LastEditors: long
 * @LastEditTime: 2012-12-04
 */
#include "CommandWebServer.h"
#include <ESPAsyncWebSrv.h>

AsyncWebServer CommandWebServer::server(8080);

void CommandWebServer::Begin(void) {
  // generic http handler
  server.onNotFound(handleRequest);

  // Begin the server
  server.begin();
}

void CommandWebServer::handleRequest(AsyncWebServerRequest *request) {
  String path = request->url();
  path.replace("/", "");

  Serial.println(path);
  Serial2.println(path);

  AsyncWebServerResponse *response = request->beginResponse(200, "text/plain",  "Ok");
  handleCors(response);
  request->send(response);
}

void CommandWebServer::handleCors(AsyncWebServerResponse *response) {
  response->addHeader("Access-Control-Allow-Origin", "*");
  response->addHeader("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
  response->addHeader("Access-Control-Allow-Headers", "*");
}

