#include "web_server.h"

// Tworzymy serwer nasłuchujący na standardowym porcie HTTP 80
ESP8266WebServer server(80);
WebInterface webUI;

void WebInterface::begin() {
    // 1. Uruchamiamy ESP jako Access Point (Własna sieć WiFi)
    Serial.println("\nUruchamianie sieci WiFi (Tryb AP)...");
    WiFi.softAP("MedBox_Konfiguracja", "12345678"); // SSID i Hasło dla komisji
    
    Serial.print("Połącz się z WiFi 'MedBox_Konfiguracja' i wejdź na adres: ");
    Serial.println(WiFi.softAPIP());

    // 2. Konfiguracja "ścieżek" (routingu) na naszej stronie
    server.on("/", std::bind(&WebInterface::handleRoot, this));
    server.on("/save", HTTP_POST, std::bind(&WebInterface::handleSaveConfig, this));

    // 3. Start serwera
    server.begin();
    Serial.println("Web Server uruchomiony!");
}

void WebInterface::handleClient() {
    server.handleClient(); // Nasłuchiwanie na zapytania od przeglądarki
}

void WebInterface::handleRoot() {
    // Wysyłamy wygenerowany kod HTML do przeglądarki klienta
    server.send(200, "text/html", generateHtml());
}

void WebInterface::handleSaveConfig() {
    // Odbieramy dane z formularza
    String newSSID = server.arg("ssid");
    String newPASS = server.arg("password");
    
    Serial.println("Otrzymano nowe dane logowania:");
    Serial.println("SSID: " + newSSID);
    Serial.println("PASS: " + newPASS);
    
    // Na razie tylko wyświetlamy je w HTML, zapis do EEPROM dodamy później!
    String response = "<html><body><h1>Zapisano pomyslnie!</h1>";
    response += "<p>Nowe SSID: " + newSSID + "</p>";
    response += "<a href='/'>Wroc</a></body></html>";
    
    server.send(200, "text/html", response);
}

String WebInterface::generateHtml() {
    // Prosty interfejs HTML + odrobina CSS dla wyglądu
    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>MedBox Admin</title>";
    html += "<style>body{font-family:Arial; margin:20px;} .box{padding:10px; margin:5px; border:1px solid #000; display:inline-block;} .taken{background-color:#c8e6c9;} .waiting{background-color:#ffcdd2;}</style>";
    html += "</head><body>";
    
    html += "<h2>Panel Administratora MedBox</h2>";
    
    // SEKCJA 1: Stan przegródek
    html += "<h3>Stan Skrytek:</h3>";
    for(int i = 0; i < NUM_BOXES; i++) {
        // Sprawdzamy czy dioda świeci (czeka na leki)
        // Zakładamy, że diody są na porcie B (piny 7-13) zgodnie z Twoim kodem
        bool isWaiting = periph.mcp.digitalRead(i + NUM_BOXES); 
        
        if (isWaiting) {
            html += "<div class='box waiting'>Skrytka " + String(i) + ": LEKI CZEKAJĄ</div>";
        } else {
            html += "<div class='box taken'>Skrytka " + String(i) + ": POBRANO</div>";
        }
    }
    
    // SEKCJA 2: Formularz zmiany WiFi
    html += "<hr><h3>Konfiguracja WiFi Docelowego</h3>";
    html += "<form action='/save' method='POST'>";
    html += "SSID: <input type='text' name='ssid'><br><br>";
    html += "Haslo: <input type='password' name='password'><br><br>";
    html += "<input type='submit' value='Zapisz i Restartuj'>";
    html += "</form>";
    
    html += "</body></html>";
    return html;
}