#include <AdafruitIO.h>
#include <AdafruitIO_MQTT.h>
#include <AdafruitIO_WiFi.h>
#include <AdafruitIO_Feed.h>

#include <ArduinoHttpClient.h>
#include <DHT.h>          // Library for DHT22 Humidity/Temp sensor
#include <DHT_U.h>        // Another library for DHT22 sensor

#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include "config.h"

// Some code borrowed from Feather HUZZAH examples, some from ladyada

#define DHTPIN 12         // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22     // DHT 22  (AM2302), AM2321

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

// digital pin 5
#define LED_PIN 5

// set up the 'digital' feed
AdafruitIO_Feed *digital = io.feed("DigitalShower");

// set up the humidity feed
AdafruitIO_Feed *humidityFeed = io.feed("HuzzahHumidity");
// set up the fahrenheit feed
AdafruitIO_Feed *farEndHeightFeed = io.feed("FantasticFarenheit");

void setup() {
  Serial.begin(9600);     // Set up console logging  
  while(! Serial);        // Wait for serial monitor to open
  
  pinMode(0, OUTPUT);     // Pin 0 is red LED for heartbeat
  pinMode(LED_PIN, OUTPUT); // Pin 5 is the Shower On/Off indicator LED
  
  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // set up a message handler for the 'digital' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  digital->onMessage(handleMessage);

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  digital->get();
    
  // Start sensor
  Serial.println(F("DHT22 starting!"));
  dht.begin();
  
}

void loop() {
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  
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
  delay(3500);
}


// this function is called whenever an 'digital' feed message
// is received from Adafruit IO. it was attached to
// the 'digital' feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {

  Serial.print("Shower toggle received <- ");

  if(data->toPinLevel() == HIGH)
    Serial.println("HIGH");
  else
    Serial.println("LOW");


  digitalWrite(LED_PIN, data->toPinLevel());
}
