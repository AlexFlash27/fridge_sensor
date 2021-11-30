//idSensor 1
//fridgeName горячий цех-шкаф холодильный
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <Wire.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t
//dht22
#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"
#define DHTPIN 7
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //Newer Ethernet shields have a MAC address printed on a sticker on the shield
IPAddress server(192, 168, 7, 1); //IPv4 address
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);

//const int led = 7;
EthernetClient client;
void MorningAlarm();
void EveningAlarm();
void digitalClockDisplay();

void setup() {
  // prepare pin as output
  //pinMode(led, OUTPUT);
  //digitalWrite(led, LOW);

  Serial.begin(9600);
  Ethernet.begin(mac);
  dht.begin();
  // wait for Arduino Serial Monitor
  while (!Serial) ; 
  
  // get and set the time from the RTC
  setSyncProvider(RTC.get);   
  if (timeStatus() != timeSet) 
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");     
  
  // to test your project, you can set the time manually 
  //setTime(8,29,0,28,12,20); // set time to Saturday 8:29:00am Jan 1 2011

  // create the alarms, to trigger functions at specific times
  Alarm.alarmRepeat(11,0,0,MorningAlarm);  // 9:00am every day
  Alarm.alarmRepeat(11,5,0,EveningAlarm);  // 19:00 -> 7:00pm every day
}

void loop() {
  int idSensor = 1;
  String fridgeName = "горячий%20цех-шкаф%20холодильный";
  float hum = dht.readHumidity(); //Reading the humidity and storing in hum
  float temp = dht.readTemperature(); //Reading the temperature as Celsius and storing in temp
  float fah = dht.readTemperature(true); //reading the temperature in Fahrenheit
  float heat_index = dht.computeHeatIndex(fah, hum); //Reading the heat index in Fahrenheit
  float heat_indexC = dht.convertFtoC(heat_index); //Converting the heat index in Celsius
  
  digitalClockDisplay();
  // wait one second between each clock display in serial monitor
  Alarm.delay(1000); 
}

// functions to be called when an alarm triggers
void MorningAlarm() {
  if (client.connect(server, 80)) { //Connecting at the IP address and port we saved before
    Serial.println("connected");
    //client.print("GET /data.php?temperature=25&humidity=27&heat_index=24"); //Connecting and Sending values to database
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
void EveningAlarm() {
  // write here the task to perform every evening
  if (client.connect(server, 80)) { //Connecting at the IP address and port we saved before
    Serial.println("connected");
    //client.print("GET /data.php?temperature=25&humidity=27&heat_index=24"); //Connecting and Sending values to database
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

//void digitalClockDisplay() {
  // digital clock display of the time
  //Serial.print(hour());
  //printDigits(minute());
  //printDigits(second());
  //Serial.println();
//}
//void printDigits(int digits) {
  //Serial.print(":");
  //if (digits < 10)
    //Serial.print('0');
  //Serial.print(digits);
//}
