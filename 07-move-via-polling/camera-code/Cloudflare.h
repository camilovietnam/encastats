#ifndef _CLOUDFLARE_H
#define _CLOUDFLARE_H

#include <HTTPClient.h>
#include <sstream>
#include <ArduinoJson.h>
#include <vector>
#include <string>

#include "credentials.h"

class Cloudflare {
  public:
    void Ping(void);
    std::vector<String> Poll(void);
    void SendTurnOnNotification(void);
};
#endif
