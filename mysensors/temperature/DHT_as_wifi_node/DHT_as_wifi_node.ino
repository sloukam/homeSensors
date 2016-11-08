/**
   The MySensors Arduino library handles the wireless radio link and protocol
   between your home built sensors/actuators and HA controller of choice.
   The sensors forms a self healing radio network with optional repeaters. Each
   repeater and gateway builds a routing tables in EEPROM which keeps track of the
   network topology allowing messages to be routed to nodes.

   Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
   Copyright (C) 2013-2015 Sensnology AB
   Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors

   Documentation: http://www.mysensors.org
   Support Forum: http://forum.mysensors.org

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   version 2 as published by the Free Software Foundation.

 *******************************

   REVISION HISTORY
   Version 1.0: Henrik EKblad
   Version 1.1 - 2016-07-20: Converted to MySensors v2.0 and added various improvements - Torben Woltjen (mozzbozz)

   DESCRIPTION
   This sketch provides an example of how to implement a humidity/temperature
   sensor using a DHT11/DHT-22.

   For more information, please visit:
   http://www.mysensors.org/build/humidity

*/

// Enable debug prints
#define MY_DEBUG

#define MY_NODE_ID                  66
//#define MY_PARENT_NODE_ID           0
//#define MY_PARENT_NODE_IS_STATIC

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69
//#define MY_RS485

#include <SPI.h>
#include <MySensors.h>
#include <DHT.h>

// Set this to the pin you connected the DHT's data pin to
#define DHT22_PIN 3

#define MAX_TEMPERATURE_ATTEMPTS 3

// Set this offset if the sensor has a permanent small offset to the real temperatures
#define SENSOR_TEMP_OFFSET 0

// Force sending an update of the temperature after n sensor reads, so a controller showing the
// timestamp of the last update doesn't show something like 3 hours in the unlikely case, that
// the value didn't change since;
// i.e. the sensor would force sending an update every UPDATE_INTERVAL*FORCE_UPDATE_N_READS [ms]
static const uint8_t FORCE_UPDATE_N_READS = 10;

#define CHILD_ID_HUM  0
#define CHILD_ID_TEMP 1


// Sleep time between sensor updates (in milliseconds)
// Must be >1000ms for DHT22 and >2000ms for DHT11
//static const uint64_t UPDATE_INTERVAL = 5000;
unsigned int minimumSamplingPeriod = 3000;
static const unsigned int TRANSMITION_INTERVAL = 20000;


//float lastTemp;
//float lastHum;
//uint8_t nNoUpdatesTemp;
//uint8_t nNoUpdatesHum;
bool metric = true;

MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);

DHT dht;

void presentation()
{
  // Send the sketch version information to the gateway
  sendSketchInfo("TemperatureAndHumidity", "1.1");

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_HUM, S_HUM);
  present(CHILD_ID_TEMP, S_TEMP);

  //  metric = getConfig().isMetric;
}


void setup() {

  Serial.begin(115200);
  dht.setup(DHT22_PIN, dht.DHT22); // data pin 2
  if (dht.getMinimumSamplingPeriod() > minimumSamplingPeriod) {
    minimumSamplingPeriod = dht.getMinimumSamplingPeriod();
  }

  Serial.println("Humidity and temperature node starting...");

  //  sleep(minimumSamplingPeriod);
}

void loop() {
  sendTemperatureAndHumidity();
  delay(TRANSMITION_INTERVAL);
}

void sendTemperatureAndHumidity() {

  delay(minimumSamplingPeriod);
  // Force reading sensor, so it works also after sleep()
  dht.readSensor(true);

  // Get temperature from DHT library
  float temperature = getTemperature(MAX_TEMPERATURE_ATTEMPTS);
  if (isnan(temperature)) {
    Serial.println("Failed reading temperature from DHT!");
  } else {
    temperature += SENSOR_TEMP_OFFSET;
    send(msgTemp.set(temperature, 1));

#ifdef MY_DEBUG
    Serial.print("Temperature: ");
    Serial.println(temperature);
#endif
  }


  delay(minimumSamplingPeriod);

  // Get humidity from DHT library
  float humidity = dht.getHumidity();
  if (isnan(humidity)) {
    Serial.println("Failed reading humidity from DHT");
  } else {
    send(msgHum.set(humidity, 1));

#ifdef MY_DEBUG
    Serial.print("Humidity: ");
    Serial.println(humidity);
#endif
  }

}



//void sendTemperatureAndHumidity() {
//  delay(minimumSamplingPeriod);
//  float temper = getTemperature(MAX_TEMPERATURE_ATTEMPTS);
//  Serial.print(temper, 1);
//
//  delay(minimumSamplingPeriod);
//  float humid = getHumidity(MAX_TEMPERATURE_ATTEMPTS);
//  Serial.println(humid, 1);
//}


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



