
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
#define INA219_POWER_REG 0X03
#define INA219_CURRENT_REG 0X04
#define INA219_CALIBRATION_REG 0X05

#define NUM_SAMPLES 128
#define SAMPLES_DIV 7


Timer t;

static volatile bool sample_almost_ready = false;

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
  t.every(50, sample_almost_ready_ISR);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

  Serial.println("OLED begun");

  display.display();
  
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE);
}

void printformat(){
  display.setCursor(0,0);
  display.print("min");
  display.setCursor(0,10);
  display.print("avg");
  display.setCursor(0,20);
  display.print("max");

  display.setCursor(70,0);
  display.print("mW");
  display.setCursor(70,10);
  display.print("mW");
  display.setCursor(70,20);
  display.print("mW");
}
unsigned char MSB_V = 0;
unsigned char LSB_V = 0;

unsigned char MSB_C = 0;
unsigned char LSB_C = 0;

unsigned char MSB_P = 0;
unsigned char LSB_P = 0;

unsigned char counter = 0;
unsigned char pointer = 0;
int values [NUM_SAMPLES];
int sum = 0.0;
unsigned char maxN = 0;
unsigned char minN = 100000;
void loop() {
  // read from the sensor
  t.update();
  if (sample_almost_ready) {
    noInterrupts();
    sample_almost_ready = false;
    interrupts();
    
    counter++;
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

    Wire.beginTransmission(INA219_BASE_ADDR); // transmit to device
    Wire.write(INA219_POWER_REG); //Adress of the result register
    Wire.endTransmission(INA219_BASE_ADDR);  //stop transmiting
         
    Wire.requestFrom(INA219_BASE_ADDR, 2);
    MSB_P  = Wire.read();
    LSB_P  = Wire.read();

//    float voltage = (float)(((MSB_V << 8 | LSB_V) >> 3) * 16) / 4000.0;
//    
//    
//    int16_t currentRegister = (((MSB_C) << 8) | LSB_C) ;
//    
//    
//    float current =  currentRegister * 97.65;
    int power = ((MSB_P << 8 | LSB_P) )   ; //97.65uA * 20
    //int power = abs(voltage * current/1000);
    sum = sum - values [pointer];
    values [pointer] = power;
    sum = sum + values [pointer];
    int average = sum >> SAMPLES_DIV;
    minN = -1;
    maxN = 0;
    
    for (int i=0;i<NUM_SAMPLES-1;i++){  
      if (values [i] > maxN){
          maxN = values [i];
      }
      if (values [i] < minN){
          minN = values [i];
      }
    }
    
    if (counter == 10 ){
        counter = 0;
        String minN_String = String(minN * 193);
        String average_String = String(average* 193);
        String maxN_String = String(maxN* 193);
        
        display.clearDisplay();
        printformat();
        display.setTextSize(1);
        
        display.setCursor(30,0);
        int l = minN_String.length();
        if (minN==0){
          display.print(minN_String.substring(0,l)+ ".00" );
        }
        else{
          display.print(minN_String.substring(0,l-2)+ "." + minN_String.substring(l-2));
        }
        display.setCursor(30,10);
        l = average_String.length();
        if (average==0){
          display.print(average_String.substring(0,l)+ ".00" );
        }
        else{
          display.print(average_String.substring(0,l-2)+ "." + average_String.substring(l-2));
        }
        
        display.setCursor(30,20);
        l = maxN_String.length();
        if (maxN==0){
          display.print(maxN_String.substring(0,l)+ ".00" );
        }
        else{
          display.print(maxN_String.substring(0,l-2)+ "." + maxN_String.substring(l-2));
        }
        display.display();
    }
    if (pointer>=NUM_SAMPLES-1){
        pointer = 0;
    }
    else {
        pointer++;
    }
    
  }
  else if(!sample_almost_ready){
     digitalWrite(LED_BUILTIN,LOW);
      __WFI();  
  }
}
