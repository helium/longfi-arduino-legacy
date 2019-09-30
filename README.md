# LongFi Arduino

This arduino library supports radio communication with Semtech compatible radios using the Helium LongFi Protocol.

## Board Support

### B-L072Z-LRWAN1 - ST STM32L0 Discovery kit  
Install board support package, find instructions [here](https://github.com/stm32duino/Arduino_Core_STM32#getting-started).  

Arduino IDE:  
1. Select Tools -> Board: -> Discovery 
2. Select Tools -> Board part number: -> Discovery L072Z-LRWAN1 

[B-L072Z-LRWAN1 User Manual](https://www.st.com/content/ccc/resource/technical/document/user_manual/group0/ac/62/15/c7/60/ac/4e/9c/DM00329995/files/DM00329995.pdf/jcr:content/translations/en.DM00329995.pdf)  

### Catena 4610 - MCCI  
Install board support package, find instructions [here](https://github.com/mcci-catena/Arduino_Core_STM32#getting-started). 

Arduino IDE:  
Select Tools -> Board: -> MCCI Catena 4610.  

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

