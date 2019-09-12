#include <Arduino.h>
#include <SPI.h>
#include "ArduinoLongFi.h"

// set OUI and device_id to work with LongFi routing
const uint32_t oui = 1234;
const uint16_t device_id = 99;

// provide chip select, DIO0/INT, and RESET for any board
const uint8_t RFM95_CS = 8;
const uint8_t RFM95_INT = 3;
const uint8_t RFM95_RST = 4;

LongFi LongFi(LongFi::RadioType::SX1276, RFM95_RST, RFM95_CS, RFM95_INT);

void setup() {
  Serial.begin(9600);
  LongFi.init(oui, device_id);
}

uint8_t data[42] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42};

void loop() {
  Serial.print("sending: ");
  Serial.println(data[0]);
  // send blocks until complete
  LongFi.send(data, sizeof(data));
  data[0]++;
}