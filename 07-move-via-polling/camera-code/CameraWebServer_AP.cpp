
// WARNING!!! Make sure that you have selected Board ---> ESP32 Dev Module
//            Partition Scheme ---> Huge APP (3MB No OTA/1MB SPIFFS)
//            PSRAM ---> enabled

#define CAMERA_MODEL_M5STACK_WIDE

#include "CameraWebServer_AP.h"
#include "credentials.h"

void startCameraServer();

/* * * * * * * * * * * * * * * * * * * * * * */
/* * *  Initialize the Camera Server * * * * */
/* * * * * * * * * * * * * * * * * * * * * * */
void CameraWebServer_AP::CameraWebServer_AP_Init(void)
{
  connectToWifi();
}

/* * * * * * * * * * * * * * * * * * * * * * */
/* * *  Connect to the wifi  * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * */
void CameraWebServer_AP::connectToWifi() {
  uint64_t chipid = ESP.getEfuseMac();
  char string[10];
  sprintf(string, "%04X", (uint16_t)(chipid >> 32));
  String mac0_default = String(string);
  sprintf(string, "%08X", (uint32_t)chipid);
  String mac1_default = String(string);
  String url = SSID + mac0_default + mac1_default;
  const char *mac_default = url.c_str();

  Serial.println(":----------------------------:");
  Serial.print("wifi_name:");
  Serial.println(mac_default);
  Serial.println(":----------------------------:");
  wifi_name = mac0_default + mac1_default;

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(666);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  startCameraServer();

  Serial.print("Camera server is ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

