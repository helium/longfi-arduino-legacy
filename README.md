# LongFi

This arduino library supports radio communication with Semtech compatible radios using the Helium LongFi Protocol.

## Board Support
The example sketch currently only works with the MCCI Catena 4610 board. 
It currently requires MCCI's Catena Board support package. Please
follow instructions to install [here](https://github.com/mcci-catena/Arduino_Core_STM32#getting-started). 

Next select "MCCI Catena 4610" from Tools -> Boards -> MCCI Catena 4610.

[Catena 4610 Pin Mapping Diagram](https://github.com/mcci-catena/HW-Designs/blob/master/Boards/Catena-4610/Catena-4610-Pinmapping.png)

## Install Library
This arduino library needs to be installed manullay as it has not been added to the Arduino Library registry yet.

1. Clone this repo in the following directory:  
linux: /home/{user}/Arduino/libraries  
windows: Documents/Arduino/libraries  
mac os: Documents/Arduino/libraries  

2. Restart Arduino IDE

## Example Sketch

Arduino IDE:  
File -> Examples -> LongFi -> TransmitPacket  

## Programming

### DFU (Flashing Only)
Arduino IDE:  
Select Tools -> Upload Method -> DFU  

To put the board in DFU mode, hold down the boot button while pressing and releasing the reset button.

### ST-Link (Flash & Debug)
Arduino IDE:  
Select Tools -> Upload Method -> STLink

Requires an ST-Link debugger connected to SWCLK, SWDIO, Vref, and GND, refer to pin mapping diagram. 

