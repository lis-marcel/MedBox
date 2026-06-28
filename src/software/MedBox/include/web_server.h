#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h> // DODANE
#include "config.h"
#include "peripherials.h"

// Zaczynamy zapisywać dane WiFi od adresu 100 (omijamy dane RTC)
#define EEPROM_WIFI_ADDR 100 

// Struktura reprezentująca nasze dane w pamięci
struct WiFiCredentials {
    char magic[4];    // Flaga sprawdzająca czy to są nasze dane (np. wpiszemy "CFG")
    char ssid[33];    // Max 32 znaki dla nazwy WiFi
    char password[65];// Max 64 znaki dla hasła
};

class WebInterface {
public:
    void begin();
    void handleClient(); 

private:
    void handleRoot();
    void handleSaveConfig();
    String generateHtml(); 
};

extern WebInterface webUI;

#endif