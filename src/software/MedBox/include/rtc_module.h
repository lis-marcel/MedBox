#ifndef RTC_MODULE_H
#define RTC_MODULE_H

#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <EEPROM.h>

class RTCModule {
public:
    bool begin();
    DateTime getCurrentTime();
    void printTime(); 
    
    void saveHeartbeat(); 
    String getOfflineDuration(); 

private:
    RTC_DS3231 rtc;
    
    bool isRtcConnected = false; // DODANE: Flaga bezpieczeństwa
    uint32_t lastSavedTime = 0; 
    uint32_t offlineSeconds = 0; 
    const int eepromAddress = 0; 
};

extern RTCModule myRtc; 

#endif