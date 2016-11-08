#define MY_RADIO_NRF24

#include <MySensors.h>
#include <SPI.h>

#define OPEN 1
#define CLOSE 0

MyMessage msg(NODE_ID, V_TRIPPED);

uint8_t value = OPEN;

void presentation() {
    present(NODE_ID, S_DOOR);
}

void loop() {
    value = value == OPEN ? CLOSE : OPEN;
    send(msg.set(value));
    sleep(10000);
}
