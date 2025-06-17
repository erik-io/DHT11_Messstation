
# IoT-Wetterstation mit ESP32 und DHT11

Dieses Projekt wurde im Rahmen meiner Ausbildung zum Fachinformatiker für Anwendungsentwicklung realisiert. Ziel war es, eine fehlerhafte C++ Applikation für einen ESP32 zu reparieren und zu einer voll funktionsfähigen IoT-Wetterstation auszubauen.

Das Projekt demonstriert Kernkompetenzen in der Embedded-Entwicklung, im systematischen Debugging und in der Problemlösung nach der **PRIMM-Methode** (Predict, Run, Investigate, Modify, Make).

### **Architektur & Features**

Die Wetterstation basiert auf einem **Heltec ESP32 Board**, das über einen **DHT11-Sensor** kontinuierlich die Umgebungstemperatur und Luftfeuchtigkeit misst.

**Kernfunktionen:**

-   **Echtzeit-Datenerfassung:** Auslesen von Temperatur und Luftfeuchtigkeit im Sekundentakt.
    
-   **WLAN-Konnektivität:** Das Gerät verbindet sich selbstständig mit einem vordefinierten WLAN-Netzwerk. Der Verbindungsstatus sowie die zugewiesene IP-Adresse werden auf dem Display angezeigt.
    
-   **Automatischer Reconnect:** Bei einem Verbindungsabbruch sorgt ein **FreeRTOS-Timer** dafür, dass der ESP32 nach kurzer Zeit selbstständig versucht, die WLAN-Verbindung wiederherzustellen.
    
-   **Datenvisualisierung:** Alle relevanten Informationen (Sensorwerte, WLAN-Status, IP-Adresse) werden übersichtlich auf dem integrierten OLED-Display dargestellt.
    
-   **Serial Debugging:** Zur Überwachung und Fehleranalyse werden alle Messwerte und System-Events parallel auf dem seriellen Monitor ausgegeben.
    

### **Technische Umsetzung & Lernerfahrungen**

Die größte Herausforderung bestand darin, den initial bereitgestellten, nicht funktionierenden Code zu verstehen und zu korrigieren. Die Aufgabenstellung umfasste konkret die Implementierung der Sensorinitialisierung, des WLAN-Verbindungsaufbaus und der Anzeige der IP-Adresse.

Durch die strukturierte Analyse konnte ich Fehler in der Ansteuerung der Hardware und der Netzwerkkonfiguration identifizieren und beheben. Ein wichtiger Lernschritt war die Implementierung der ereignisbasierten Behandlung von WLAN-Statusänderungen (`WiFiEvent`), um robust auf Verbindungsverluste reagieren zu können.
