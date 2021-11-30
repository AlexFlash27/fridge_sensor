// idSensor 15
// fridgeName цех холод. закуски-стол холодильный
#include <SPI.h>
#include <WiFi.h>
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
const char* ssid = "School97";
const char* password = "20012020";

IPAddress server(192, 168, 7, 1); // IPv4 address

WiFiClient client;

void setup() {
  Serial.begin(9600); // setting the baud rate at 9600
  WiFi.begin(ssid, password);
  dht.begin();
}

void loop() {
  int idSensor = 100;
  String fridgeName = "цех%20холод.%20закуски-стол%20холодильный";
  float hum = dht.readHumidity(); // reading the humidity and storing in hum
  float temp = dht.readTemperature(); // reading the temperature as Celsius and storing in temp
  float fah = dht.readTemperature(true); // reading the temperature in Fahrenheit
  float heat_index = dht.computeHeatIndex(fah, hum); // reading the heat index in Fahrenheit
  float heat_indexC = dht.convertFtoC(heat_index); // converting the heat index in Celsius

  //if (isnan(hum) || isnan(temp)) {
  //Serial.println(F("Failed to read from DHT sensor!"));
  //return;
//}

  if (client.connect(server, 80)) { // connecting at the IP address and port we saved before
    Serial.println("connected");
    client.print("GET /data.php?");
    client.print("temperature=");
    client.print(temp);
    client.print("&humidity=");
    client.print(hum);
    client.print("&heat_index=");
    client.print(heat_indexC);
    client.print("&idSensor=");
    client.print(idSensor);
    client.print("&fridgeName=");
    client.print(fridgeName);
    client.println(" HTTP/1.1");
    client.println("Host: 192.168.7.1");
    client.println("Connection: close");
    client.println();

    Serial.print("GET /data.php?");
    Serial.print("temperature=");
    Serial.print(temp);
    Serial.print("&humidity=");
    Serial.print(hum);
    Serial.print("&heat_index=");
    Serial.print(heat_indexC);
    Serial.print("&idSensor=");
    Serial.print(idSensor);
    Serial.print("&fridgeName=");
    Serial.print(fridgeName);
    Serial.println(" HTTP/1.1");

    client.stop(); // losing the connection
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  delay(300000);//delay(5000);
}
