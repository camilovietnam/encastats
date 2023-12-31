#ifndef _WIFI_H
#define _WIFI_H

#include <WiFi.h>
#include "credentials.h"

class WifiClient {
    public:
        static void connect(void);
};

#endif