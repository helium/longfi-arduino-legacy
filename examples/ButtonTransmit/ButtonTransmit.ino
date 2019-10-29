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
const uint8_t USER_BUTTON     = USER_BTN;
const uint8_t LED             = LED_BLUE;
LongFi LongFi(LongFi::RadioType::SX1276, RADIO_RESET_PIN, RADIO_SS_PIN, RADIO_DIO_0_PIN);
#endif

static boolean volatile button_pushed = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Setup Start");
  pinMode(LED, OUTPUT);

  SPI.setMOSI(RADIO_MOSI_PIN);
  SPI.setMISO(RADIO_MISO_PIN);
  SPI.setSCLK(RADIO_SCLK_PIN);
  SPI.setSSEL(RADIO_SS_PIN);
  SPI.begin();

  LongFi.init(oui, device_id);
  Serial.println("Setup Complete");

  pinMode(USER_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(USER_BUTTON), push_button_ISR, HIGH);
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
      Serial.println("Button press #: "+String(counter));
      // Turn LED ON to indicate beginning of TX
      digitalWrite(LED, HIGH);
      // send blocks until complete
      LongFi.send(buf, msg_len);
      // Turn LED OFF to indicate completion of TX
      digitalWrite(LED, LOW);
      noInterrupts();
      button_pushed = false;
      interrupts();
      counter++;
  }
}
