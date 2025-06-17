// Programm:        Messstation_Wifi_vIT23_2_clean 
// Version:         v2 
// Letzte Änderung: 07.04.2025
// Anmerkung:       Übung 1 zu LS 7.4 - Anbindung Wifi 


#include "DHT.h"                // Bibliothek für DHT-Temperatur-/Feuchtigkeitssensor
#include <WiFi.h>               // WLAN-Funkion für ESP32
#include "heltec.h"             // OLED-Display und Board-spezifische Funktionen
extern "C" {
  #include "freertos/FreeRTOS.h"
  #include "freertos/timers.h"  // Timer-Funktion aus FREERTOS für Wiederverbindung
}
// #include <AsyncMqttClient.h>


// WLAN-Zugangsdaten
#define WIFI_SSID "AloWiFreeUnterricht"
#define WIFI_PASSWORD "DuKommstRein"

// Konfiguration des DHT-Sensors
#define DHTPIN 13     // GPIO-Pin für den DHT-Sensor
#define DHTTYPE DHT11 // Sensortyp: DHT11 

DHT dht(DHTPIN, DHTTYPE); // Initialisierung des Sensors

float temp; // Variable für Temperaturwert
float hum;  // Variable für Luftfeuchtigkeit

TimerHandle_t wifiReconnectTimer; // FreeRTOS-Timer zur Wiederverbindung bei WLAN-Verlust

unsigned long previousMillis = 0; // Zeitstempel für Zeitmessung in loop()
const long interval = 1000;       // Messinterval in Milisekunden

// Anzeige der Sensorwerte und WLAN-Daten auf dem OLED-Display
void displayReadingsOnOled() {
 String temperatureDisplay = "Temperature: " + (String)temp + "°C";
 String humidityDisplay =    "Humidity:    " + (String)hum + "%";
 
 Heltec.display->clear();
 Heltec.display->drawString(0, 36, temperatureDisplay);
 Heltec.display->drawString(0, 48, humidityDisplay);
 Heltec.display->drawString(0, 0, "WiFi connected: ");
 Heltec.display->drawString(0, 12, String(WIFI_SSID));
 Heltec.display->drawString(0, 24, "IP: " + WiFi.localIP().toString());
 Heltec.display->display();
}

// Verbindungsaufbau zum WLAN (Wird beim Start und bei Verbindungsverlust verwendet)
void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

// Ereignisbehandlung für WLAN-Statusänderung
void WiFiEvent(WiFiEvent_t event) {
  Serial.printf("[WiFi-event] event: %d\n", event);
  switch(event) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      // Verbindung erfolgreich hergestellt
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      Heltec.display->clear();
      Heltec.display->drawString(0, 0, "WiFi connected: ");
      Heltec.display->drawString(0, 12, String(WIFI_SSID));
      Heltec.display->drawString(0, 24, "IP: " + WiFi.localIP().toString());
      Heltec.display->display();
      break;
    case  ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      // WLAN-Verbindung verloren - Ausgabe und automatischer Wiederverbindungsversuch
      Serial.println("WiFi lost connection");
      Heltec.display->clear();
      Heltec.display->drawString(0, 0, "WiFi lost connection");
      Heltec.display->display();
      xTimerStart(wifiReconnectTimer, 0); // Startet den Reconnect-Timer
      break;
  }
}

// Initialisierung des Systems
void setup() {
  Serial.begin(115200);
  Serial.println();
  
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
    
  dht.begin(); // Start des DHT-Sensors
  
  // Reconnect-Timer konfigurieren (einmaliger Versuch nach 2 Sekunden)
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));
  
  // Ereignis-Callback für WLAN registrieren
  WiFi.onEvent(WiFiEvent);

  // Verbindungsversuch beim Start
  connectToWifi();  
}

void loop() {
  unsigned long currentMillis = millis();

  // Führt alle 1000 Millisekunden eine Messung durch
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      hum = dht.readHumidity();     // Luftfeuchtigkeit messen
      temp = dht.readTemperature(); // Temperatur messen

      Serial.print("Temperature: ");
      Serial.println(temp);
      Serial.print("humidity: ");
      Serial.println(hum);

      displayReadingsOnOled();      // Werte auf dem Display anzeigen
    
      // Fehlerbehandlung bei fehlgeschlagener Sensorabfrage
      if (isnan(temp) || isnan(hum)) {
        Serial.println(F("Failed to read from DHT!"));
        Heltec.display->drawString(0, 48, "Failed to read from DHT!");
        Heltec.display->display();
        return;
       }
   }
}
