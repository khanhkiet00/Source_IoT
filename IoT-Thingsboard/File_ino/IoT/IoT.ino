#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>

#define DHTPIN D4 // Pin của DHT11
#define DHTTYPE DHT11

const char* ssid = "Hoang Tan";
const char* password = "15041974";
const char* thingsboardUrl = "http://192.168.1.249:8080/api/v1/VYONqrE0rADIBpAxQPjI/telemetry"; 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  dht.begin();
}

void loop() {
  float humidity = random(10, 50);
  float temperature =  random(10, 50);
  // float humidity = dht.readHumidity();
  // float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  String jsonData = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + "}";

  WiFiClient client;
  HTTPClient http;
  http.begin(client, thingsboardUrl);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(jsonData);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(httpResponseCode);
    Serial.println(response);
  } else {
    Serial.println("Error on sending POST: " + String(httpResponseCode));
  }

  http.end();
  delay(5000); // Gửi dữ liệu mỗi 5s
}
