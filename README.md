# LongFi Arduino  
[![Build Status](https://travis-ci.com/helium/longfi-arduino.svg?token=bzKc8EpW7xxqudyhDiz1&branch=master)](https://travis-ci.com/helium/longfi-arduino)

This arduino library supports radio communication with Semtech compatible radios using the Helium LongFi Protocol.

## Install Library with Arduino IDE
Arduino IDE:  
1. Sketch -> Include Libraries -> Manage Libraries
2. Search for `LongFi`
3. Install Latest version

## Install Library with Git
1. Clone this repo in the following directory:  
linux: /home/{user}/Arduino/libraries  
windows: Documents/Arduino/libraries  
mac os: Documents/Arduino/libraries  

2. Restart Arduino IDE

## Example Sketches

Arduino IDE:  
File -> Examples -> LongFi -> TransmitPacket  
File -> Examples -> LongFi -> ButtonTransmit (Only supported on B-L072Z-LRWAN1 and Heltec ESP32 boards) 

## Board Support

### B-L072Z-LRWAN1 - ST STM32L0 Discovery kit  
Install board support package, find instructions [here](https://github.com/stm32duino/Arduino_Core_STM32#getting-started).  

Arduino IDE:  
1. Select Tools -> Board: -> Discovery 
2. Select Tools -> Board part number: -> Discovery L072Z-LRWAN1 

[B-L072Z-LRWAN1 Product Page](https://www.st.com/en/evaluation-tools/b-l072z-lrwan1.html)  
[B-L072Z-LRWAN1 User Manual](https://www.st.com/content/ccc/resource/technical/document/user_manual/group0/ac/62/15/c7/60/ac/4e/9c/DM00329995/files/DM00329995.pdf/jcr:content/translations/en.DM00329995.pdf)  

### Catena 4610 - MCCI  
Install board support package, find instructions [here](https://github.com/mcci-catena/Arduino_Core_STM32#getting-started). 

Arduino IDE:  
Select Tools -> Board: -> MCCI Catena 4610.  

[Catena 4610 Product Page](https://store.mcci.com/collections/iot-building-blocks/products/mcci-catena-4610-integrated-node-for-lorawan-technology)  
[Catena 4610 Pin Mapping Diagram](https://github.com/mcci-catena/HW-Designs/blob/master/Boards/Catena-4610/Catena-4610-Pinmapping.png)

### Feather M0 RFM96 - Adafruit

1. Add Adafruit package index, instructions [here](https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module/setup).
2. Install both Arduino and Adafruit SAMD board support, instructions [here](https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module/using-with-arduino-ide#install-samd-support-6-5).
3. (Windows Only) Install drivers, instructions [here](https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module/using-with-arduino-ide#install-drivers-windows-7-and-8-only-6-11)

[Adafruit Feather M0 with RFM95 Product Page](https://www.adafruit.com/product/3178)  
[Adafruit Feather M0 with RFM95 Datasheets & Files](https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module/downloads)  

### Heltec ESP32 based LoRa boards
Install Heltec ESP32 board support package, instructions here [here](https://docs.heltec.cn/#/en/user_manual/how_to_install_esp32_Arduino?id=step1-download-arduino-esp32-support)

**Important**  
Next, until [this](https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series/issues/117) issue is resolved, follow [these](Heltec-ESP32-board-support-fix.md) instructions to enable support for ldflags.

Arduino IDE:
Select Tools -> Board: -> `ESP32 Arduino`

_`ESP32 Arduino` is currently one of_
- `Wireless_Stick`
- `Wireless_Stick_Lite`
- `WIFI_Lora_32`
- `WIFI_LoRa_32_V2`

[Heltec Wireless Stick Lite Product Page](https://heltec.org/project/wireless-stick-lite/)
[Heltec Wireless Stick Product Page](https://heltec.org/project/wireless-stick/)
[Heltec WiFi LoRa 32 (V2) Product Page](https://heltec.org/project/wifi-lora-32/)

## Programming(Upload Method):

### B-L072Z-LRWAN1 - ST STM32L0 Discovery kit 

#### STM32CubeProgrammer(SWD)
Will use onboard ST-Link(Flasher/Debugger) to upload sketch.  
Download and Install required utility from ST [here](https://www.st.com/en/development-tools/stm32cubeprog.html).  

Arduino IDE:  
Select Tools -> Upload Method -> STM32CubeProgrammer(SWD)


### Catena 4610 - MCCI 

#### DFU
Arduino IDE:  
Select Tools -> Upload Method -> DFU  

To put the board in DFU mode, hold down the boot button while pressing and releasing the reset button.

#### ST-Link
Arduino IDE:  
Select Tools -> Upload Method -> STLink

Requires an ST-Link debugger connected to SWCLK, SWDIO, Vref, and GND, refer to pin mapping diagram. 

### Heltec ESP32 based LoRa boards
No special instructions are needed, the defaults are fine.

