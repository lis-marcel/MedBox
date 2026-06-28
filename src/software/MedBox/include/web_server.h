#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "config.h"
#include "peripherials.h"

class WebInterface {
public:
    void begin();
    void handleClient(); // Musi być wywoływane w loop()

private:
    // Metody obsługujące konkretne adresy URL w przeglądarce
    void handleRoot();
    void handleSaveConfig();
    
    String generateHtml(); // Generuje wygląd strony
};

extern WebInterface webUI;

#endif