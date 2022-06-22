#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN D4
#define DHTTYPE    DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);
 
const char* ssid = "ssid";
const char* password =  "password";
const char* mqttServer = "192.168.43.148";
const int mqttPort = 12948;
const char* mqttUser = "";
const char* mqttPassword = "";
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
 
  Serial.begin(115200);
 dht.begin();
 sensor_t sensor;
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
  client.subscribe("langganan"); //masukkan subscribe disini
 
}
 
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
}
 
void loop() {
  client.loop();
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  double t = event.temperature;
  dht.humidity().getEvent(&event);
  double h = event.relative_humidity;
  char* tc = reinterpret_cast<char*>(&t);
  char* hc = reinterpret_cast<char*>(&h);
  client.publish("suhu",tc);
  client.publish("kelembaban",hc);
}
