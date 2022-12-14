#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  //Serial.begin(9600); // For testing only. Not necessary
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float f = dht.readTemperature(true);

  /* // For printing to serial monitor for testing only. Not necessary
  Serial.print(" Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(f);
  Serial.println("°F");
  delay(2000);
  */
}