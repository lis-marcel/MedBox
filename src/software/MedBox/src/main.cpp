#include <Arduino.h>

// Definiujemy piny dla naszych diod testowych
#define LED1_PIN 12
#define LED2_PIN 13

void setup() {
  // Uruchamiamy port szeregowy, żeby widzieć komunikaty w terminalu PlatformIO
  Serial.begin(115200);
  Serial.println("\nStart testu diod na ESP8266!");

  // Konfigurujemy piny jako wyjścia
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // Ustawiamy stan początkowy (LED1 włączony, LED2 wyłączony)
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, LOW);
}

void loop() {
  // Przełączamy stany diod na przeciwne
  digitalWrite(LED1_PIN, !digitalRead(LED1_PIN));
  digitalWrite(LED2_PIN, !digitalRead(LED2_PIN));

  // Wypisujemy kontrolnie kropkę w terminalu
  Serial.print(".");

  // Czekamy 500 milisekund (pół sekundy)
  delay(500);
}