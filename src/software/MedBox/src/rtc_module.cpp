#include "rtc_module.h"

RTCModule myRtc;

bool RTCModule::begin() {
    // 1. Inicjalizacja EEPROM MUSI być na samym początku!
    EEPROM.begin(512);

    // 2. Próbujemy połączyć się z RTC
    if (!rtc.begin()) {
        isRtcConnected = false; // Zaznaczamy, że modułu nie ma
        return false;
    }

    isRtcConnected = true; // Moduł jest obecny!

    // if (rtc.lostPower()) {
    //     Serial.println("RTC zgubił zasilanie! Ustawiam czas kompilacji...");
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // }

    uint32_t savedUnix;
    EEPROM.get(eepromAddress, savedUnix);

    DateTime now = rtc.now();
    uint32_t currentUnix = now.unixtime();

    if (savedUnix > 0 && savedUnix <= currentUnix) {
        offlineSeconds = currentUnix - savedUnix;
        Serial.print("Czas bez zasilania: ");
        Serial.print(offlineSeconds);
        Serial.println(" sekund.");
    } else {
        offlineSeconds = 0; 
    }

    return true;
}

DateTime RTCModule::getCurrentTime() {
    // Zabezpieczenie na wypadek wywołania bez podłączonego RTC
    if (!isRtcConnected) {
        return DateTime(2024, 1, 1, 12, 0, 0); // Zwracamy sztuczną datę
    }
    return rtc.now();
}

void RTCModule::printTime() {
    if (!isRtcConnected) return;
    
    DateTime now = rtc.now();
    Serial.printf("%04d-%02d-%02d %02d:%02d:%02d\n", 
                  now.year(), now.month(), now.day(), 
                  now.hour(), now.minute(), now.second());
}

void RTCModule::saveHeartbeat() {
    // ZABEZPIECZENIE: Jeśli nie ma modułu RTC, nie próbuj zapisywać czasu do EEPROM!
    if (!isRtcConnected) return; 

    DateTime now = rtc.now();
    uint32_t currentUnix = now.unixtime();
    
    if (currentUnix - lastSavedTime >= 60) {
        EEPROM.put(eepromAddress, currentUnix);
        EEPROM.commit(); 
        lastSavedTime = currentUnix;
    }
}

String RTCModule::getOfflineDuration() {
    if (!isRtcConnected) return "Moduł RTC niepodłączony";
    if (offlineSeconds == 0) return "Brak przerw w zasilaniu";
    
    uint32_t days = offlineSeconds / 86400;
    uint32_t hours = (offlineSeconds % 86400) / 3600;
    uint32_t mins = (offlineSeconds % 3600) / 60;
    
    String result = "";
    if (days > 0) result += String(days) + " dni, ";
    if (hours > 0) result += String(hours) + " godz, ";
    result += String(mins) + " min";
    
    return result;
}