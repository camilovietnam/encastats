#include "Telegram.h"

bool Telegram::SendPhoto(camera_fb_t* photo) {
  // Crear la URL del servicio de Telegram para enviar fotos
  //  String url = "https://api.telegram.org/bot" + String(BOT_TOKEN) + "/sendPhoto?chat_id=" + String(TELEGRAM_CHAT_ID);
  // std::ostringstream requestBodyStream;
  // requestBodyStream << "https://api.telegram.org/bot" << BOT_TOKEN << "/sendPhoto?chat_id=" << TELEGRAM_CHAT_ID;

  // Configurar el cliente HTTP
  HTTPClient http;
  // String url = String(requestBodyStream.str());
  http.begin("fake");
  http.addHeader("Content-Type", "image/jpeg");

  // Enviar la foto
  int httpResponseCode = http.POST((uint8_t*)photo->buf, photo->len);

  // Cerrar la conexión HTTP
  http.end();

  String response = http.getString();
  Serial.print("HTTP response from the bot: ");
  Serial.println(response);

  Serial.print("Response code: ");
  Serial.println(httpResponseCode);

  // Verificar si la foto se envió correctamente (código de respuesta HTTP 200)
  return (httpResponseCode == 200);
}