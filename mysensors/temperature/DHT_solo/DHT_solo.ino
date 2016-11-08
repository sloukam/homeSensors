#include "DHT.h"

#define DHT22_PIN 3

DHT dht;

unsigned int minimumSamplingPeriod = 5000;

void setup() {
  Serial.begin(115200);
  dht.setup(DHT22_PIN, dht.DHT22); // data pin 2
  minimumSamplingPeriod = dht.getMinimumSamplingPeriod();

  Serial.println("Status\tHumidity (%)\tTemperature (C)");

}

void loop() {
  delay(minimumSamplingPeriod);

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.println(temperature, 1);

}

