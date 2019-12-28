#include <Arduino.h>
#include <SPI.h>
#include <LongFi.h>

// set OUI and device_id to work with LongFi routing
const uint32_t oui = 1234;
const uint16_t device_id = 99;
const uint8_t preshared_key[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};


#if defined(ARDUINO_WIRELESS_STICK_LITE) || defined(ARDUINO_WIRELESS_STICK)
// Adding a single define for switching on board type
#define _VARIANT_ARDUINO_HELTEC_ESP32_
#endif

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
#elif defined(_VARIANT_ARDUINO_HELTEC_ESP32_)
// Heltec ESP32 (Wireless Stick Lite)
const uint8_t RADIO_RESET_PIN = RST_LoRa;
const uint8_t RADIO_DIO_0_PIN = DIO0;
const uint8_t RADIO_MOSI_PIN  = MOSI;
const uint8_t RADIO_MISO_PIN  = MISO;
const uint8_t RADIO_SCLK_PIN  = SCK;
const uint8_t RADIO_SS_PIN    = SS;
const uint8_t USER_BUTTON     = KEY_BUILTIN;
#endif

LongFi LongFi(LongFi::RadioType::SX1276, RADIO_RESET_PIN, RADIO_SS_PIN, RADIO_DIO_0_PIN);

static boolean volatile button_pushed = false;

// !! Important !!
// Be aware that endianness will
// depend on your target device and the 
// receiving device of this packet payload.
struct LongFiPayload_t{
  uint32_t value1;
  int16_t value2;
  float value3;
};

struct LongFiPayload_t lf_payload;
uint8_t lf_buf[sizeof(lf_payload)];
uint8_t counter = 1;

void setup() {
  Serial.begin(9600);
  Serial.println("Setup Start");
  pinMode(LED, OUTPUT);
#ifdef _VARIANT_ARDUINO_HELTEC_ESP32_
  // The Heltec ESP32 SPI library doesn't have set* functions
  SPI.begin(RADIO_SCLK_PIN,RADIO_MISO_PIN,RADIO_MOSI_PIN,RADIO_SS_PIN);
#else
  SPI.setMOSI(RADIO_MOSI_PIN);
  SPI.setMISO(RADIO_MISO_PIN);
  SPI.setSCLK(RADIO_SCLK_PIN);
  SPI.setSSEL(RADIO_SS_PIN);
  SPI.begin();
#endif  
  
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

void loop() {
  while(button_pushed) {
      // Load new payload values
      lf_payload.value1 = counter;
      lf_payload.value2 = random(-15000, 15000);
      lf_payload.value3 = random(0, 99)/100.0;

      // Copy payload struct to buf
      memcpy(lf_buf, &lf_payload, sizeof(lf_payload));

      // Debug print
      Serial.println("Button press #: "+String(counter));
      Serial.println("value1: " + String(lf_payload.value1) + " value2: " + String(lf_payload.value2) + " value3: " + String(lf_payload.value3));
      Serial.println("Buffer Size: " + String(sizeof(lf_buf)));
      for(int i = 0; i < sizeof(lf_buf); i++)
      {
        Serial.println("Byte [" + String(i) + "]: " + String(lf_buf[i]));
      }

      // Turn LED ON to indicate beginning of TX
      digitalWrite(LED, HIGH);
      // Send blocks until complete
      LongFi.send(lf_buf, sizeof(lf_buf));
      // Turn LED OFF to indicate completion of TX
      digitalWrite(LED, LOW);
      noInterrupts();
      button_pushed = false;
      interrupts();
      counter++;
  }
}

