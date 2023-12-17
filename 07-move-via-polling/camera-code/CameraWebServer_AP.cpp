
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
  // readRemoteSettings();
}

/* * * * * * * * * * * * * * * * * * * * * * */
/* * *  Send HTTP Turn On    * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * */
void CameraWebServer_AP::sendTurnOnRequest() {
  HTTPClient http;
  std::ostringstream requestBodyStream;

  requestBodyStream << "{\"message\":{\"chat\":{\"id\":" << TELEGRAM_CHAT_ID << "},\"text\":\"turn on\", \"info\":\"Camera server: " << WiFi.localIP().toString().c_str() <<  "\"}}";

  http.begin(CLOUDFLARE_WORKER_URL);
  http.addHeader("Content-Type", "application/json");

  std::string requestBody = requestBodyStream.str();
  int httpResponseCode = http.POST(requestBody.c_str());

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.print("HTTP Request failed. Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

/* * * * * * * * * * * * * * * * * * * * * * */
/* * *  Ping the worker  * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * */
void CameraWebServer_AP::PingWorker(void) {
  HTTPClient http;
  std::ostringstream requestBodyStream;

  requestBodyStream << "{\"message\":{\"chat\":{\"id\":" << TELEGRAM_CHAT_ID << "},\"text\":\"ping\"}}";

  Serial.print("Calling URL: ");
  Serial.println(CLOUDFLARE_WORKER_URL);
  
  Serial.print(requestBodyStream.str().c_str());

  // Get the concatenated string
  std::string requestBody = requestBodyStream.str();

  http.begin(CLOUDFLARE_WORKER_URL);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(requestBody.c_str());

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.print("HTTP Request failed. Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
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

  sendTurnOnRequest();
  startCameraServer();

  Serial.print("Camera server is ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

