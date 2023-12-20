/*
 * @Description:
 * @version: 
 * @Author: u1062049, based on original code by Elegoo
 * @Date: 2023-11-18 23:53:27
 * @LastEditors: u1062049 (based on original code by Changhua)
 * @LastEditTime: 2023-11-18
 */

#define CAMERA_MODEL_M5STACK_WIDE

#include "CameraWebServer_AP.h"
#include "CommandWebServer.h"
#include "Camera.h"
#include "esp_camera.h"
#include "Cloudflare.h"

// define the ports for the Serial port that connects to the UNO R3 board
#define RXD2 33
#define TXD2 4

CameraWebServer_AP CameraWebServerAP;
CommandWebServer CommandWebServer;
Cloudflare cloudflare;

// bool WA_en = false;

void setup()
{
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  CameraWebServerAP.CameraWebServer_AP_Init();

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  CommandWebServer.Begin();

  // give time to the program to finish before starting the ping
  cloudflare.SendTurnOnNotification();

//   delay(500);
}

// Ping
unsigned long lastPingTime = 0;
unsigned long pingInterval = 5000; // How many milliseconds to wait for each ping

// Poll
unsigned long lastPollingTime = 0;
unsigned long pollInterval = 500; // How many milliseconds to wait for each data poll

void loop()
{
  // check if we need to ping
  unsigned long currentTime = millis();

  if (currentTime - lastPingTime >= pingInterval) {
    cloudflare.Ping();
    lastPingTime = millis();

    if (pingInterval < 10000) {
      pingInterval = pingInterval + 1000;
    }
  }

  // Check if we need to poll
  if (currentTime - lastPollingTime >= pollInterval) {
      String movement = cloudflare.Poll();
      if (!movement.isEmpty()) {
        Serial2.println(movement);
      }
      lastPollingTime = millis();
  }
}
