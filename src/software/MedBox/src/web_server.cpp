#include "web_server.h"
#include "rtc_module.h"

// Tworzymy serwer nasłuchujący na standardowym porcie HTTP 80
ESP8266WebServer server(80);
WebInterface webUI;

// Zmienne globalne do przechowywania obecnych nazw (do wyświetlenia na stronie)
String currentSSID = "MedBox_Konfiguracja";
String currentPASS = "12345678";

void WebInterface::begin() {
    // 0. TWARDY RESET RADIA I CZYSZCZENIE CACHE'U
    WiFi.persistent(false); // Blokuje ESP przed ukrytym zapisywaniem konfiguracji WiFi
    WiFi.disconnect(true);
    WiFi.softAPdisconnect(true);
    delay(200); // Dajemy układowi radiowemu chwilę na oddech

    // 1. Odczyt danych WiFi z EEPROM
    WiFiCredentials creds;
    EEPROM.get(EEPROM_WIFI_ADDR, creds);

    // Sprawdzamy, czy w ogóle mamy zapisane jakieś własne dane (słowo "CFG")
    // Oraz upewniamy się, że pierwszy znak SSID to nie jest pusty znak.
    if (strncmp(creds.magic, "CFG", 3) == 0 && creds.ssid[0] != '\0') {
        currentSSID = String(creds.ssid);
        currentPASS = String(creds.password);
        Serial.println("\nZnaleziono własną konfigurację sieci AP w EEPROM:");
        Serial.println("SSID: " + currentSSID);
    } else {
        Serial.println("\nBrak poprawnych danych w EEPROM. Uruchamiam sieć domyślną.");
        currentSSID = "MedBox_Konfiguracja";
        currentPASS = "12345678";
    }

    // 2. Uruchamiamy ESP TYLKO jako własną sieć (Access Point)
    WiFi.mode(WIFI_AP);
    
    // Sztywne przypisanie adresu IP zapobiega błędom DHCP
    IPAddress apIP(192, 168, 4, 1);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    
    // 3. Rozgłaszanie sieci z odpowiednimi zabezpieczeniami
    bool apStatus = false;
    
    // WPA2 wymaga min. 8 znaków. Jeśli hasło jest krótsze (np. puste), robimy sieć otwartą
    if (currentPASS.length() >= 8) {
        apStatus = WiFi.softAP(currentSSID.c_str(), currentPASS.c_str());
    } else {
        Serial.println("UWAGA: Hasło za krótkie lub puste. Uruchamiam sieć otwartą!");
        apStatus = WiFi.softAP(currentSSID.c_str());
    }
    
    if (apStatus) {
        Serial.print("SUKCES: Uruchomiono sieć: ");
        Serial.println(currentSSID);
        Serial.print("Adres panelu administracyjnego: ");
        Serial.println(WiFi.softAPIP());
    } else {
        Serial.println("BŁĄD KRYTYCZNY: Nie udało się uruchomić Access Pointa!");
    }

    // 4. Routing podstron
    server.on("/", std::bind(&WebInterface::handleRoot, this));
    server.on("/save", HTTP_POST, std::bind(&WebInterface::handleSaveConfig, this));

    server.begin();
    Serial.println("Web Server uruchomiony!");
}

void WebInterface::handleClient() {
    server.handleClient();
}

void WebInterface::handleRoot() {
    server.send(200, "text/html", generateHtml());
}

void WebInterface::handleSaveConfig() {
    String newSSID = server.arg("ssid");
    String newPASS = server.arg("password");
    
    Serial.println("\nZapisuję nowe dane sieci AP do EEPROM...");
    
    // Pakujemy dane ze strony www do naszej struktury
    WiFiCredentials creds;
    strncpy(creds.magic, "CFG", 4);
    strncpy(creds.ssid, newSSID.c_str(), sizeof(creds.ssid) - 1);
    creds.ssid[sizeof(creds.ssid) - 1] = '\0'; // Zabezpieczenie przed przepełnieniem
    strncpy(creds.password, newPASS.c_str(), sizeof(creds.password) - 1);
    creds.password[sizeof(creds.password) - 1] = '\0';

    // Pchamy do pamięci trwałej!
    EEPROM.put(EEPROM_WIFI_ADDR, creds);
    EEPROM.commit();

    // Informacja zwrotna
    String response = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
    response += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    response += "<style>body{font-family:Arial; text-align:center; margin-top:50px; line-height:1.6;}</style></head><body>";
    response += "<h1>Zapisano pomyślnie!</h1>";
    response += "<p>MedBox restartuje się i za chwilę udostępni nową sieć o nazwie: <b>" + newSSID + "</b></p>";
    response += "<p style='color:red;'><b>UWAGA:</b> Twoje urządzenie straci teraz połączenie.<br>Musisz ręcznie połączyć się z nową siecią WiFi i odświeżyć tę stronę!</p>";
    response += "</body></html>";
    
    server.send(200, "text/html", response);
    
    // Dajemy ESP 1 sekundę na fizyczne wysłanie strony HTML
    delay(1000); 
    ESP.restart(); 
}

String WebInterface::generateHtml() {
    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>MedBox Admin</title>";
    html += "<style>body{font-family:Arial; margin:20px;} .box{padding:10px; margin:5px; border:1px solid #000; display:inline-block;} .taken{background-color:#c8e6c9;} .waiting{background-color:#ffcdd2;} .info{background-color:#e3f2fd; padding:15px; border-radius:5px; margin-bottom: 20px;} .net-status{color: #1565c0; font-weight: bold;}</style>";
    html += "</head><body>";
    
    html += "<h2>Panel Administratora MedBox</h2>";

    // Informacja o aktualnej sieci
    html += "<p class='net-status'>Aktywna sieć nadawana przez urządzenie: " + currentSSID + "</p>";

    // POBIERANIE CZASU Z RTC
    DateTime now = myRtc.getCurrentTime();
    char timeStr[30];
    sprintf(timeStr, "%04d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());

    html += "<div class='info'>";
    html += "<strong>Czas urządzenia:</strong> " + String(timeStr) + "<br><br>";
    html += "<strong>Ostatnia utrata zasilania trwała:</strong> " + myRtc.getOfflineDuration();
    html += "</div>";
    
    html += "<h3>Stan Skrytek:</h3>";
    for(int i = 0; i < NUM_BOXES; i++) {
        bool isWaiting = periph.mcp.digitalRead(i + NUM_BOXES); 
        if (isWaiting) {
            html += "<div class='box waiting'>Skrytka " + String(i) + ": LEKI CZEKAJĄ</div>";
        } else {
            html += "<div class='box taken'>Skrytka " + String(i) + ": POBRANO</div>";
        }
    }
    
    // FORMULARZ ZMIANY DANYCH AP
    html += "<hr><h3>Zmień nazwę i hasło tej sieci</h3>";
    html += "<p style='font-size: 0.9em; color: #555;'>Po zmianie danych, MedBox natychmiast zresetuje swoją sieć WiFi.</p>";
    html += "<form action='/save' method='POST'>";
    html += "Nowa nazwa sieci (SSID): <br><input type='text' name='ssid' value='" + currentSSID + "' required><br><br>";
    html += "Nowe hasło (min. 8 znaków, zostaw puste dla braku hasła): <br><input type='text' name='password' minlength='8' value='" + currentPASS + "'><br><br>";
    html += "<input type='submit' value='Zmień i Restartuj urządzenie'>";
    html += "</form>";
    
    html += "</body></html>";
    return html;
}