/*
 * @Description:
 * @version: 
 * @Author: u1062049, based on original code by Elegoo
 * @Date: 2023-11-18 23:53:27
 * @LastEditors: u1062049 (based on original code by Changhua)
 * @LastEditTime: 2023-11-18
 */

// WARNING!!! Make sure that you have selected Board ---> ESP32 Dev Module
//            Partition Scheme ---> Huge APP (3MB No OTA/1MB SPIFFS)
//            PSRAM ---> enabled

#define CAMERA_MODEL_M5STACK_WIDE

#include "CommandWebServer.h"
#include "Camera.h"
#include "esp_camera.h"
#include "Cloudflare.h"
#include "Telegram.h"
#include "WifiClient.h"

// define the ports for the Serial port that connects to the UNO R3 board
#define RXD2 33
#define TXD2 4

CommandWebServer commandWebServer;
Cloudflare cloudflare;
Camera camera;
Telegram telegram;
WifiClient wifiClient;

// bool WA_en = false;
#define CAMERA_MODEL_M5STACK_WIDE

// Ping-related timestamps
unsigned long lastPingTime = 0;
unsigned long pingInterval = 5000; // How many milliseconds to wait for each ping

void startCameraServer();

void setup()
{
  // Setup the serial communication
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // connect to the WiFi network and setup the camera server
  wifiClient.connect();
  startCameraServer();

  // unknown
  // pinMode(13, OUTPUT);
  // digitalWrite(13, HIGH);

  // initialize camera
  camera.Init();

  // initialize the web server that listens to movement commands
  commandWebServer.Begin();

  // Let the Worker know that we are on
  cloudflare.SendTurnOnNotification();
}

void loop()
{
  unsigned long currentTime = millis();

  // do we need to ping?
  if (currentTime - lastPingTime >= pingInterval) {
    cloudflare.Ping();
    lastPingTime = millis();

    if (pingInterval < 10000) {
      pingInterval = pingInterval + 1000;
    }
  }
}
