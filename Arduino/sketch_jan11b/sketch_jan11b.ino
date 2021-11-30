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
IPAddress server(192, 168, 7, 1); //IPv4 address
// set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);
EthernetClient client;

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

  // to test your project, you can set the time manually
  //setTime(8,29,0,1,1,11); // set time to Saturday 8:29:00am Jan 1 2011

  // create the alarms, to trigger functions at specific times
  Alarm.alarmRepeat(10, 25, 0, MorningAlarm);
  Alarm.alarmRepeat(10, 25, 30, EveningAlarm);

  Ethernet.begin(mac);
  dht.begin();
}

void loop() {
  digitalClockDisplay();
  // wait one second between each clock display in serial monitor
  Alarm.delay(1000);
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
    //if you didn't get a connection to the server:
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
