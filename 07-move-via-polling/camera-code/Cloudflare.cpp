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
std::vector<String> Cloudflare::Poll(void) {
  // Serial.println("polling worker");
  HTTPClient http;
  std::ostringstream requestBodyStream;
  std::vector<String> vectorMovements;

  requestBodyStream << "{\"message\":{\"chat\":{\"id\":" << TELEGRAM_CHAT_ID << "},\"text\":\"rawlist\"}}";

//  Serial.print("Calling URL: ");
//  Serial.println(CLOUDFLARE_WORKER_URL);

//  Serial.println(requestBodyStream.str().c_str());

  // Get the concatenated string
  std::string requestBody = requestBodyStream.str();

  http.begin(CLOUDFLARE_WORKER_URL);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(requestBody.c_str());

  if (httpResponseCode > 0) {
//    Serial.print("HTTP Response code: ");
//    Serial.println(httpResponseCode);

    String response = http.getString();
    http.end();

    response = response.substring(1, response.length() - 1); // remove double quotes

    const char *inputCharArray = response.c_str();
    char *token = strtok(const_cast<char*>(inputCharArray), ",");

    // if no movements, finish
    if (!token) {
      // Serial.println("No movements.");
      http.end();
      return vectorMovements;
    }

    // extract each token and send it via Serial2
    while (token != NULL) {
      vectorMovements.push_back(token);
//      Serial2.println(token);
      token = strtok(NULL, ",");
    }
  } else {
    Serial.print("HTTP Request failed. Error code: ");
    Serial.println(httpResponseCode);
  }

  return  vectorMovements;
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