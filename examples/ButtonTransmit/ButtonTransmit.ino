#include <Arduino.h>
#include <SPI.h>
#include "LongFi.h"

// set OUI and device_id to work with LongFi routing
const uint32_t oui = 1234;
const uint16_t device_id = 99;
const uint8_t preshared_key[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

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
  // Configure Debug Print and LED
  Serial.begin(9600);
  Serial.println("Setup Start");
  pinMode(LED, OUTPUT);
  // Assign Radio SPI Pins
  SPI.setMOSI(RADIO_MOSI_PIN);
  SPI.setMISO(RADIO_MISO_PIN);
  SPI.setSCLK(RADIO_SCLK_PIN);
  SPI.setSSEL(RADIO_SS_PIN);
  SPI.begin();
  // Init LongFi
  LongFi.init(oui, device_id, preshared_key);
  // Configure User Button 
  pinMode(USER_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(USER_BUTTON), push_button_ISR, HIGH);
  Serial.println("Setup Complete");
}

void push_button_ISR(){
  button_pushed = true;
}

uint8_t buf[128];
uint8_t counter = 1;
String message1 = "Hello";
String message2 = "World!";

void loop() {
  while(button_pushed) {
      // Prepare Payload Buffer
      String full_msg = "{\"value1\":" + String(counter) + ",\"value2\":\"" + message1 + "\",\"value3\":\"" + message2 + "\"}";
      uint8_t msg_len = full_msg.length();
      memcpy(buf, full_msg.c_str(), msg_len);
      // Debug Prints
      Serial.println("Payload: " + full_msg);
      Serial.println("Length: " + String(msg_len));
      Serial.println("Button press #: " + String(counter));
      // Turn LED ON to indicate beginning of TX
      digitalWrite(LED, HIGH);
      // Send Payload Buffer 
      LongFi.send(buf, msg_len);
      // Turn LED OFF to indicate completion of TX
      digitalWrite(LED, LOW);
      // Reset Button Flag
      noInterrupts();
      button_pushed = false;
      interrupts();
      // Increment Counter
      counter++;
  }
}