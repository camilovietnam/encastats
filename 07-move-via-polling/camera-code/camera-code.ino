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
#include "Polling.h"
#include "Cloudflare.h"

// define the ports for the Serial port that connects to the UNO R3 board
#define RXD2 33
#define TXD2 4

CameraWebServer_AP CameraWebServerAP;
CommandWebServer CommandWebServer;
Polling polling;
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
unsigned long pingInterval = 2000; // How many milliseconds to wait for each ping

// Poll
unsigned long lastPollingTime = 0;
unsigned long pollInterval = 4000; // How many milliseconds to wait for each data poll

void loop()
{
  // check if we need to ping
  unsigned long currentTime = millis();

  if (currentTime - lastPingTime >= pingInterval) {
    // cloudflare.Ping();
    lastPingTime = millis();

    if (pingInterval < 10000) {
      pingInterval = pingInterval + 1000;
    }
  }

  // Check if we need to poll
  if (currentTime - lastPollingTime >= pollInterval) {
      std::vector<String> movements = cloudflare.Poll();
      for (const auto& movement : movements) {
        Serial.println(movement);
        Serial2.println(movement);
      }
      lastPollingTime = millis();
  }
}
