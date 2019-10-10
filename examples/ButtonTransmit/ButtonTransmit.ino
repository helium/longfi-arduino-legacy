#include <Arduino.h>
#include <SPI.h>
#include "LongFi.h"

// set OUI and device_id to work with LongFi routing
const uint32_t oui = 1;
const uint16_t device_id = 11;

#ifdef _VARIANT_ARDUINO_STM32_
// Discovery L072CZ-LRWAN1
const uint8_t RADIO_RESET_PIN = RADIO_RESET_PORT;
const uint8_t RADIO_DIO_0_PIN = RADIO_DIO_0_PORT;
const uint8_t RADIO_MOSI_PIN  = RADIO_MOSI_PORT;
const uint8_t RADIO_MISO_PIN  = RADIO_MISO_PORT;
const uint8_t RADIO_SCLK_PIN  = RADIO_SCLK_PORT;
const uint8_t RADIO_SS_PIN    = RADIO_NSS_PORT;
#endif
#ifdef _VARIANT_ARDUINO_CATENA_461x_
// MCCI Catena 4610
const uint8_t RADIO_RESET_PIN = RADIO_RESET;
const uint8_t RADIO_TCXO_PIN  = 33;
const uint8_t RADIO_DIO_0_PIN = 25;
const uint8_t RADIO_MOSI_PIN  = RADIO_MOSI;
const uint8_t RADIO_MISO_PIN  = RADIO_MISO;
const uint8_t RADIO_SCLK_PIN  = RADIO_SCK;
const uint8_t RADIO_SS_PIN    = RADIO_SS;
#endif

const uint8_t buttonPin =USER_BTN;
static boolean button_pushed = false;

LongFi LongFi(LongFi::RadioType::SX1276, RADIO_RESET_PIN, RADIO_SS_PIN, RADIO_DIO_0_PIN);

void setup() {
  Serial.begin(9600);

  SPI.setMOSI(RADIO_MOSI_PIN);
  SPI.setMISO(RADIO_MISO_PIN);
  SPI.setSCLK(RADIO_SCLK_PIN);
  SPI.setSSEL(RADIO_SS_PIN);
  SPI.begin();

  LongFi.init(oui, device_id);
  Serial.println("Setup Start");

  #ifdef _VARIANT_ARDUINO_CATENA_461x_
  // Enable TCXO On Catena Boards 
  LongFi.enable_tcxo(RADIO_TCXO_PIN);
  #endif
  
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), push_button_ISR, HIGH);
  
  
}

void push_button_ISR(){
  button_pushed = true;
}

uint8_t buf[128];
uint8_t counter = 1;
String message = "hello";
void loop() {
  while(button_pushed) {

      String full_msg = "|" + String(counter) + ": " + message+ "|";
      uint8_t msg_len = sizeof(full_msg) - 1;
      full_msg.getBytes(buf, msg_len);
      Serial.println("Pressed buttons: "+String(counter));
      LongFi.send(buf, msg_len);
      noInterrupts();
      button_pushed = false;
      interrupts();
      counter++;
  }
}
