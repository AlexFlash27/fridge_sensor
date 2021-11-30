#include <SPI.h>
#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
const char* ssid = "School97";
const char* password = "20012020";
#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

IPAddress server(192, 168, 7, 1); // IPv4 address

WiFiClient client;

void setup() {
  WiFi.begin(ssid, password);
  sensors.begin();
  Serial.begin(9600); // setting the baud rate at 9600
}

void loop() {
  int idSensor = 5;
  String fridgeName = "овощной%20цех-шкаф%20холодильный";
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);

  if (client.connect(server, 80)) { // connecting at the IP address and port we saved before
    Serial.println("connected");
    client.print("GET /data.php?");
    client.print("temperature=");
    client.print(temp);
    //client.print("&humidity=");
    //client.print(hum);
    //client.print("&heat_index=");
    //client.print(heat_indexC);
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
    //Serial.print("&humidity=");
    //Serial.print(hum);
    //Serial.print("&heat_index=");
    //Serial.print(heat_indexC);
    Serial.print("&idSensor=");
    Serial.print(idSensor);
    Serial.print("&fridgeName=");
    Serial.print(fridgeName);
    Serial.println(" HTTP/1.1");

    client.stop(); // losing the connection
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
    WiFi.reconnect();
  }
  delay(300000);//delay(5000);
  //ESP.restart();
}
