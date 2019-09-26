#include <Arduino.h>
#include <SPI.h>
#include "LongFi.h"

// set OUI and device_id to work with LongFi routing
const uint32_t oui = 1234;
const uint16_t device_id = 99;

// Discovery L072CZ-LRWAN1
// const uint8_t RADIO_RESET_PIN = RADIO_RESET_PORT;
// const uint8_t RADIO_TCXO_PIN  = RADIO_TCXO_PORT; // Do not enable
// const uint8_t RADIO_DIO_0_PIN = RADIO_DIO_0_PORT;
// const uint8_t RADIO_MOSI_PIN  = RADIO_MOSI_PORT;
// const uint8_t RADIO_MISO_PIN  = RADIO_MISO_PORT;
// const uint8_t RADIO_SS_PIN    = RADIO_NSS_PORT;

// MCCI Catena 4610
const uint8_t RADIO_RESET_PIN = RADIO_RESET;
const uint8_t RADIO_TCXO_PIN  = RADIO_TCXO;
const uint8_t RADIO_DIO_0_PIN = RADIO_DIO_0;
const uint8_t RADIO_MOSI_PIN  = RADIO_MOSI;
const uint8_t RADIO_MISO_PIN  = RADIO_MISO;
const uint8_t RADIO_SS_PIN    = RADIO_SS;

LongFi LongFi(LongFi::RadioType::SX1276, RADIO_RESET_PIN, RADIO_SS_PIN, RADIO_DIO_0_PIN);

void setup() {
  Serial.begin(9600);
  LongFi.init(oui, device_id);
  Serial.println("Setup Start");

  // Setup Pins for TCXO Enable
  pinMode(RADIO_TCXO_PIN, OUTPUT);
  pinMode(RADIO_RESET_PIN, OUTPUT);

  // Enable TCXO  
  digitalWrite(RADIO_TCXO_PIN, HIGH);
  digitalWrite(RADIO_RESET_PIN, LOW);
  delay(1);
  digitalWrite(RADIO_RESET_PIN, HIGH);
  delay(6);
  LongFi.enable_tcxo();
  digitalWrite(RADIO_RESET_PIN, LOW);
  delay(1);
  digitalWrite(RADIO_RESET_PIN, HIGH);
}

uint8_t data[8] = {1, 2, 3, 4, 5, 6, 7, 8};

void loop() {
  Serial.print("sending: ");
  Serial.println(data[0]);
  // send blocks until complete
  LongFi.send(data, sizeof(data));
  data[0]++;
}
