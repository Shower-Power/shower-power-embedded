#include <AdafruitIO.h>
#include <AdafruitIO_MQTT.h>
#include <AdafruitIO_WiFi.h>
#include <AdafruitIO_Feed.h>

#include <ArduinoHttpClient.h>
#include <DHT.h>          // Library for DHT22 Humidity/Temp sensor
#include <DHT_U.h>        // Another library for DHT22 sensor
//#include <ESP8266WiFi.h>  // Wifi library for Feather HUZZAH

#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include "config.h"

//#define IO_USERNAME  "valefleur"
//#define IO_KEY       "306f566fba684e78b9bf082961c92bfa"

// Some code borrowed from Feather HUZZAH examples, some from ladyada

#define DHTPIN 12         // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22     // DHT 22  (AM2302), AM2321

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

// set up the humidity feed
AdafruitIO_Feed *humidityFeed = io.feed("HuzzahHumidity");
// set up the farenheit feed
AdafruitIO_Feed *farEndHeightFeed = io.feed("FantasticFarenheit");

// Set up wifi data
//const char* ssid     = "WWCode";
//const char* password = "Hopper Lovelace Borg";
//const char* host = "wifitest.adafruit.com";

void setup() {
  Serial.begin(9600);     // Set up console logging  
  while(! Serial);        // Wait for serial monitor to open
  
  pinMode(0, OUTPUT);     // Pin 0 is red LED for heartbeat

  // Connect to wifi
//  Serial.print("Connecting to ");
//  Serial.println(ssid);
  
//  WiFi.begin(ssid, password);
  
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }

//  Serial.println("");
//  Serial.println("WiFi connected");  
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  
  // Start sensor
  Serial.println(F("DHT22 starting!"));
  dht.begin();
}

void loop() {
  // Beat the heart, then take a sensor reading

  //Heartbeat
  digitalWrite(0, HIGH);  // heartbeat on
  delay(500);             // wait 500 ms
  digitalWrite(0, LOW);   // heartbeat off
  delay(500);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humidity = dht.readHumidity();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float fahrenheit = dht.readTemperature(true);  

   // Check if any reads failed and exit early (to try again).
  if (isnan(humidity)) {
    Serial.println(F("Failed to get humidity!"));
    return;
  } 

  if (isnan(fahrenheit)) {
    Serial.println(F("Failed to get fahrenheit!"));
    return;
  } 

  // Log data to console
  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.println(F("%"));
  Serial.print(F("Temperature: "));
  Serial.println(fahrenheit);  

  // Log data to cloud
  Serial.print("sending humidity -> ");
  Serial.println(humidity);
  humidityFeed->save(humidity);

  Serial.print("sending FarEndHeight -> ");
  Serial.println(fahrenheit);
  farEndHeightFeed->save(fahrenheit);
  Serial.println();

  // Don't overload the cloud; avoid being throttled
  delay(3000);
}
