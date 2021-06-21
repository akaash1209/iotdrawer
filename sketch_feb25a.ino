#include <ESP8266WiFi.h>//Wi-Fi routines that we are calling to connect to the network
#include "Adafruit_MQTT.h" 
#include "Adafruit_MQTT_Client.h"
const char *ssid =  "k";     // Enter your WiFi Name
const char *pass =  "********"; // Enter your WiFi Password
WiFiClient client;
#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "akashsinghh" 
#define MQTT_PASS "***********" // Enter the API key that you copied from your adafrui IO account
#define relay D5
#define buzzer D6
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);
//Set up the feed you're subscribing to
Adafruit_MQTT_Subscribe Lock = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/Lock");
void setup()
{
  Serial.begin(115200);
  delay(10);
  mqtt.subscribe(&Lock);
  pinMode(relay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(relay, LOW); // keep motor off initally 
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");              // print ... till not connected
  }
  Serial.println("");
  Serial.println("WiFi connected");
} 
void loop()
{
   MQTT_connect();  
Adafruit_MQTT_Subscribe * subscription;
while ((subscription = mqtt.readSubscription(5000)))
     {   
   if (subscription == &Lock)
     {
      //Print the new value to the serial monitor
      Serial.println((char*) Lock.lastread);     
   if (!strcmp((char*) Lock.lastread, "Close"))
      {
        digitalWrite(relay, LOW);//function is used to write a HIGH or a LOW value to a digital pin
        Serial.print("Door Unlocked");//PRINT
        digitalWrite(buzzer, HIGH);//function is used to write a HIGH or a LOW value to a digital pin
        delay(2000);// It accepts a single integer (or number) argument. This number represents the time (measured in milliseconds). The program 
        //should wait until moving on to the next line of code when it encounters this function.
        digitalWrite(buzzer, LOW);
    }
    if (!strcmp((char*) Lock.lastread, "Open"))
      {
        digitalWrite(relay, HIGH);
        Serial.print("Door Closed");
        digitalWrite(buzzer, HIGH);
        delay(2000);
        digitalWrite(buzzer, LOW);
    }
 }  
     } 
}
void MQTT_connect() 
{
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) 
  {
    return;
  }
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  {       
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) 
       {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
}
