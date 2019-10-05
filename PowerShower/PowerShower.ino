#include <DHT.h>
#include <DHT_U.h>

// Some code borrowed from Feather HUZZAH examples, some from ladyada
//#include "DHT.h"

#define DHTPIN 12        // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  pinMode(0, OUTPUT); // Pin 0 is heartbeat

  Serial.begin(9600);  // Set up console logging
  
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
  // Read temperature as Celsius (the default)
  float celcius = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float fahrenheit = dht.readTemperature(true);  

   // Check if any reads failed and exit early (to try again).
  if (isnan(humidity)) {
    Serial.println(F("Failed to get humidity!"));
    return;
  } 

  if (isnan(celcius)) {
    Serial.println(F("Failed to get celcius!"));
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
  Serial.println(F("Temperature: "));
  Serial.print(celcius);
  Serial.println(F("°C "));
  Serial.println(fahrenheit);
  Serial.println();  

  // Log data to cloud -- TBD
}
