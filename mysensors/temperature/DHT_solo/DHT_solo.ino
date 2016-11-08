#include "DHT.h"

#define DHT22_PIN 3
#define MAX_TEMPERATURE_ATTEMPTS 3

DHT dht;

unsigned int minimumSamplingPeriod = 5000;

void setup() {
  Serial.begin(115200);
  dht.setup(DHT22_PIN, dht.DHT22); // data pin 2
  minimumSamplingPeriod = dht.getMinimumSamplingPeriod();

  Serial.println("Status\tHumidity (%)\tTemperature (C)");

}

void loop() {
  sendTemperatureAndHumidity();
}

void sendTemperatureAndHumidity() {
  delay(minimumSamplingPeriod);
  float temper = getTemperature(MAX_TEMPERATURE_ATTEMPTS);
  Serial.print(temper, 1);

  delay(minimumSamplingPeriod);
  float humid = getHumidity(MAX_TEMPERATURE_ATTEMPTS);
  Serial.println(humid, 1);

}


/**
   read and return temperatur. If error raised than wait required time and try it again.
   There is max amoutn of attempt to fetch temperature.
*/
float getTemperature(int maxNoOfAttempt) {

  for (int i = 0; i < maxNoOfAttempt; i++) {
    float temp = dht.getTemperature();

    //    Serial.print("status - ");
    //    Serial.println(dht.getStatusString());

    if ("OK" == dht.getStatusString()) {
      return temp;
    }

    Serial.println("something went wrong during getting temperature...");
    delay(minimumSamplingPeriod);
  }

  return -100;
}
/**
   read and return temperatur. If error raised than wait required time and try it again.
   There is max amoutn of attempt to fetch temperature.
*/
float getHumidity(int maxNoOfAttempt) {

  for (int i = 0; i < maxNoOfAttempt; i++) {
    float humid = dht.getHumidity();

    //    Serial.print("status - ");
    //    Serial.println(dht.getStatusString());

    if ("OK" == dht.getStatusString()) {
      return humid;
    }

    Serial.println("something went wrong during getting humidity...");
    delay(minimumSamplingPeriod);
  }

  return -100;
}

