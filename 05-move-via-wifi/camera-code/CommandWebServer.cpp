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
#include "Camera.h"

#include "img_converters.h"

Camera camera;

AsyncWebServer CommandWebServer::server(8080);

void CommandWebServer::Begin() {
  // generic http handler
  server.on("/takePhoto", HTTP_GET, handleCapture);
  server.onNotFound(handleRequest);

  // Initialize the camera
  camera.Init();

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

void CommandWebServer::handleCapture(AsyncWebServerRequest *request) {
    Serial.println("handleCapture");
    AsyncWebServerResponse *response;
    camera_fb_t *fb = NULL;

    fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("error: not fb");
        response = request->beginResponse(500, "text/plain",  "Error");
        handleCors(response);
        request->send(response);

        return;
    }

    request->send_P(200, "image/jpeg", fb->buf, fb->len);
    esp_camera_fb_return(fb);
}