#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN D4
#define DHTTYPE    DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);
WiFiClient wifiClient;
 
const char* ssid = "ssid";
const char* password = "password";
void setup () {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  dht.begin();
 sensor_t sensor; 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting..");
  }
}
 
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    double t = event.temperature;
    dht.humidity().getEvent(&event);
    double h = event.relative_humidity;

    String url = "http://192.168.43.148/mine/dht.php?t="+String(t)+"&h="+String(h);
    http.begin(wifiClient,url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
      if(payload=="panas"){
        Serial.println("keputusan jika panas");
      }else{
        Serial.println("keputusan jika tidak panas");
      }
    }
    http.end();
  }
  delay(5000);
}
