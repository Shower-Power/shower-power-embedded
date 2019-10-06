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

// Some code borrowed from Feather HUZZAH examples, some from ladyada

#define DHTPIN 12         // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22     // DHT 22  (AM2302), AM2321

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

// set up the humidity feed
AdafruitIO_Feed *humidityFeed = io.feed("HuzzahHumidity");
// set up the fahrenheit feed
AdafruitIO_Feed *farEndHeightFeed = io.feed("FantasticFarenheit");

void setup() {
  Serial.begin(9600);     // Set up console logging  
  while(! Serial);        // Wait for serial monitor to open
  
  pinMode(0, OUTPUT);     // Pin 0 is red LED for heartbeat

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
  // Turn on heartbeat LED when collecting sensor data
  digitalWrite(0, HIGH);  // heartbeat on
  delay(500);             // wait 500 ms

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

  // Turn off heartbeat LED when sending data to cloud
  digitalWrite(0, LOW);   // heartbeat off
  delay(500);

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
