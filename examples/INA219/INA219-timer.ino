#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Timer.h"

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

#define INA219_CONF_REG 0x00
#define INA219_BASE_ADDR  0x40
#define INA219_SHUNT_VOLT_REG 0X01
#define INA219_BUS_VOLT_REG 0X02
#define INA219_CURRENT_REG 0X03
#define INA219_CURRENT_REG 0X04
#define INA219_CALIBRATION_REG 0X05

Timer t;

static volatile bool sample_almost_ready = false;
static volatile bool screen_refresh = false;
static bool state = false ; //false-voltage true-current
void sample_almost_ready_ISR() {
  sample_almost_ready = true;
}



void enter_Sleep( void )
{
    FLASH->ACR &= ~FLASH_ACR_SLEEP_PD;
    __WFI();  // enter low-power mode
}


void setup() {
  Wire.setSDA(PB9);
  Wire.setSCL(PB8);
  Wire.begin();        // join i2c bus (address optional for master)

  pinMode(LED_BUILTIN, OUTPUT);
  
  //delay(500);
  //Writing the configuration register
  Wire.beginTransmission(INA219_BASE_ADDR); // transmit to device
  Wire.write(INA219_CONF_REG); //Address of the configuration register
  Wire.write(0b00011100); //LSB-MSB Reset = 0 ; BRNG = 0 (16 v); pg = 11 (/8) BADC=1001 (9); 
  Wire.write(0b11000111); //LSB-MSB SADC = 1010 (2.13 ms conversion time) Mode = 111 (Shunt and bus, continuous)
  Wire.endTransmission(INA219_BASE_ADDR); // stop transmitting

  
  Wire.beginTransmission(INA219_BASE_ADDR); // transmit to device
  Wire.write(INA219_CALIBRATION_REG); //Address of the configuration register
  Wire.write(0b10000); //Configuration byte MSB
  Wire.write(0b01000100);//Configuration byte LSB //01010100
  Wire.endTransmission(INA219_BASE_ADDR); // stop transmitting

  Serial.begin(9600);  // start serial for output
  Serial.println("Setup ready");

  //Timers configuration
  //t.oscillate(pin, 3, LOW);
  t.every(200, sample_almost_ready_ISR);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

  Serial.println("OLED begun");

  display.display();
  
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE);
}

unsigned char MSB_V = 0;
unsigned char LSB_V = 0;

unsigned char MSB_C = 0;
unsigned char LSB_C = 0;

unsigned char counter = 0;
void loop() {
  // read from the sensor
  t.update();
  if (sample_almost_ready) {
    
    noInterrupts();
    sample_almost_ready = false;
    interrupts();
    display.clearDisplay();
    digitalWrite(LED_BUILTIN,HIGH);

    Wire.beginTransmission(INA219_BASE_ADDR); // transmit to device
    Wire.write(INA219_BUS_VOLT_REG); //Adress of the result register
    Wire.endTransmission(INA219_BASE_ADDR);  //stop transmiting
    Wire.requestFrom(INA219_BASE_ADDR, 2);
    MSB_V  = Wire.read();
    LSB_V  = Wire.read();
    
    Wire.beginTransmission(INA219_BASE_ADDR); // transmit to device
    Wire.write(INA219_CURRENT_REG); //Adress of the result register
    Wire.endTransmission(INA219_BASE_ADDR);  //stop transmiting
         
    Wire.requestFrom(INA219_BASE_ADDR, 2);
    MSB_C  = Wire.read();
    LSB_C  = Wire.read();
    
    noInterrupts();
    screen_refresh = false;
    interrupts();

    float voltage = (float)(((MSB_V << 8 | LSB_V) >> 3) * 16) / 4000.0;
    display.setTextSize(1.5);
    display.setCursor(0,0);
    display.print(String(voltage) + "v");
    
    float current = (float)(((MSB_C & 0b01111111) << 8) | LSB_C) * 97.65;
    display.setTextSize(1);
    display.setCursor(0,20);
    if (current>3199209){
      display.print(String(0) + "uA");
    }
    else{
      if ((MSB_C >> 7) == 1) {
        display.print(String(current) + "uA");
      }
      else {
        display.print("-" + String(current) + "uA");
      }
    }
    
    display.display();
  }
  else if(!sample_almost_ready){
     digitalWrite(LED_BUILTIN,LOW);
      __WFI();  
  }
  

}
