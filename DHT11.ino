#include "DHT.h"
#include "heltec.h"

#define DHTTYPE DHT11  
uint8_t DHTPin = 13; 
DHT dht(DHTPin, DHTTYPE);  

void setup() {
  dht.begin();  
  
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
 }

void loop() {
  float curTemperature = dht.readTemperature();
  float curHumidity = dht.readHumidity();
  if(isnan(curTemperature)|| isnan( curHumidity)){
    Heltec.display->clear(); 
    Heltec.display->drawString(0, 0, "Test Sensor: keine Daten");
    Heltec.display->drawString(0, 24, "Sensor überprüfen!");
    Heltec.display->display();  
  }
  else if((curTemperature != NAN) && (curHumidity != NAN)){
    Heltec.display->clear();
    Heltec.display->drawString(0, 0,  "Test Sensor: bestanden");
    Heltec.display->drawString(0, 24, "Sensor liefert Daten.");
    Heltec.display->display();
  }
  delay(1000);
}
