#include "Cloudflare.h"

/* * * * * * * * * * * * * * * * * * * * * * */
/* * *  Ping the worker  * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * */
void Cloudflare::Ping(void) {
  HTTPClient http;
  std::ostringstream requestBodyStream;

  requestBodyStream << "{\"message\":{\"chat\":{\"id\":" << TELEGRAM_CHAT_ID << "},\"text\":\"ping\"}}";

//  Serial.print("Calling URL: ");
//  Serial.println(CLOUDFLARE_WORKER_URL);

  // Serial.println(requestBodyStream.str().c_str());

  // Get the concatenated string
  std::string requestBody = requestBodyStream.str();

  http.begin(CLOUDFLARE_WORKER_URL);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(requestBody.c_str());

  if (httpResponseCode > 0) {
    // Serial.println("HTTP Response code: ");
    // Serial.println(httpResponseCode);

    // String response = http.getString();
    // Serial.println(response);
  } else {
    Serial.print("HTTP Request failed. Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}


/* * * * * * * * * * * * * * * * * * * * * * */
/* * *  Poll the worker  * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * */
String Cloudflare::Poll(void) {
  // Serial.println("polling worker");
  HTTPClient http;
  std::ostringstream requestBodyStream;
  String movement;

  requestBodyStream << "{\"message\":{\"chat\":{\"id\":" << TELEGRAM_CHAT_ID << "},\"text\":\"poll\"}}";

//  Serial.print("Calling URL: ");
//  Serial.println(CLOUDFLARE_WORKER_URL);

//  Serial.println(requestBodyStream.str().c_str());

  // Get the concatenated string
  std::string requestBody = requestBodyStream.str();

  unsigned long start = millis();
  http.begin(CLOUDFLARE_WORKER_URL);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(requestBody.c_str());

  if (httpResponseCode > 0) {
    movement = http.getString();
    movement = movement.substring(1, movement.length() - 1); // remove double quotes
  } else {
    Serial.print("HTTP Request failed. Error code: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();

  if (movement == "photo") {

  }

  return movement;
}

/* * * * * * * * * * * * * * * * * * * * * * */
/* * *  Send HTTP Turn On    * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * */
void Cloudflare::SendTurnOnNotification(void) {
  HTTPClient http;
  std::ostringstream requestBodyStream;

  requestBodyStream << "{\"message\":{\"chat\":{\"id\":" << TELEGRAM_CHAT_ID << "},\"text\":\"turn on\", \"info\":\"Camera server: " << WiFi.localIP().toString().c_str() <<  "\"}}";

  http.begin(CLOUDFLARE_WORKER_URL);
  http.addHeader("Content-Type", "application/json");

  std::string requestBody = requestBodyStream.str();
  int httpResponseCode = http.POST(requestBody.c_str());

  if (httpResponseCode > 0) {
//    Serial.print("HTTP Response code: ");
//    Serial.println(httpResponseCode);

    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.print("HTTP Request failed. Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}