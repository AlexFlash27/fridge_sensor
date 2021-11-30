//idSensor 1
//fridgeName горячий цех-шкаф холодильный
#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <Wire.h>
#include <DS1307RTC.h>
#define DHTPIN 7
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // newer Ethernet shields have a MAC address printed on a sticker on the shield
IPAddress server(192, 168, 7, 1); // IPv4 address
// set the static IP address to use if the DHCP fails to assign
//IPAddress ip(192, 168, 0, 177);
//IPAddress myDns(192, 168, 0, 1);
EthernetClient client;

IPAddress ip1(192, 168, 1, 99);
EthernetServer server1(80);

void setup() {
  Serial.begin(9600);
  // wait for Arduino Serial Monitor
  while (!Serial) ;

  // get and set the time from the RTC
  setSyncProvider(RTC.get);
  if (timeStatus() != timeSet)
    Serial.println("Unable to sync with the RTC");
  else
    Serial.println("RTC has set the system time");

  // create the alarms, to trigger functions at specific times
  Alarm.alarmRepeat(10, 15, 0, MorningAlarm);
  Alarm.alarmRepeat(9, 19, 0, EveningAlarm);

  Ethernet.begin(mac);
  dht.begin();
  server1.begin();
}

void loop() {
  Ethernet.begin(mac, ip1);
  digitalClockDisplay();
  // wait one second between each clock display in serial monitor
  Alarm.delay(1000);
  float h = dht.readHumidity( );
  float t = dht.readTemperature( );
  EthernetClient client1 = server1.available();
  if (client1) {
    boolean currentLineIsBlank = true;
    while (client1.connected ( ) ) {
      if (client1.available ( ) ) {
        char character = client1.read ( );
        Serial.write(character);
        if (character == '\n' && currentLineIsBlank) {
          client1.println ("HTTP/1.1 200 OK");
          client1.println ("Content-Type: text/html");
          client1.println ("Connection: close");
          client1.println ("Refresh: 5");
          client1.println ( );
          client1.println ("<!DOCTYPE HTML>");
          client1.println ("<html>");
          client1.print ("<Title>Arduino Ethernet Webserver </Title>");
          client1.print ("<h1>Arduino Ethernet Shield Webserver </h1>");
          client1.print ("<h4>Temperature in C: ");
          client1.print (t); client1.print("C");
          client1.print ("</h4><h4>Humidity: ");
          client1.print (h); client1.print("%");
          client1.println ("<br />");
          client1.println ("</html>");
          break;
        }
        if ( character == '\n') {
          currentLineIsBlank = true;
        }
        else if (character != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client1.stop();
  }
}

//int idSensor = 1;
//String fridgeName = "горячий%20цех-шкаф%20холодильный";
//float hum = dht.readHumidity(); //reading the humidity and storing in hum
//float temp = dht.readTemperature(); //reading the temperature as Celsius and storing in temp
//float fah = dht.readTemperature(true); //reading the temperature in Fahrenheit
//float heat_index = dht.computeHeatIndex(fah, hum); //reading the heat index in Fahrenheit
//float heat_indexC = dht.convertFtoC(heat_index); //converting the heat index in Celsius

void AlaramFunc() {
  int idSensor = 1;
  String fridgeName = "горячий%20цех-шкаф%20холодильный";
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();
  float fah = dht.readTemperature(true);
  float heat_index = dht.computeHeatIndex(fah, hum);
  float heat_indexC = dht.convertFtoC(heat_index);

  if (client.connect(server, 80)) {
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
    client.stop(); //Closing the connection
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void MorningAlarm() {
  AlaramFunc();
}

void EveningAlarm() {
  AlaramFunc();
}

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println();
}
void printDigits(int digits) {
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
