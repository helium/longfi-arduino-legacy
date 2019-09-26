#include "LongFi.h"
#include "Arduino.h"
#include "SPI.h"
#include "longfi-device/board.h"
#include "longfi-device/longfi.h"
#include "longfi-device/radio/sx1276/sx1276.h"
#include "longfi-device/radio/sx1272/sx1272.h"
#include "longfi-device/radio/sx126x/sx126x.h"

static int CS_PIN;
static int DIO0_PIN;
extern "C" {
uint16_t ArduinoSpiInOut(LF_Spi_t *s, uint16_t outData){
    return SPI.transfer(outData);
}

void ArduinoDelayMs(uint32_t ms){
    delay(ms);
}

void ArduinoGpioInit(LF_Gpio_t *obj,
              PinNames pin,
              PinModes mode,
              PinConfigs config,
              PinTypes pin_type,
              uint32_t val){}



void ArduinoGpioWrite(LF_Gpio_t *obj, uint32_t val){
    if (val == 0) {
        digitalWrite(CS_PIN, LOW);
    } else {
        digitalWrite(CS_PIN, HIGH);
    }
}

uint32_t ArduinoGpioRead(LF_Gpio_t *obj){
    return 0;
}


void ArduinoGpioSetInterrupt(LF_Gpio_t *obj, IrqModes irqMode, IrqPriorities irqPriority, GpioIrqHandler *irqHandler){
}
}

static BoardBindings_t ArduinoBindings  = {
    .spi_in_out = &ArduinoSpiInOut,
    .gpio_init = &ArduinoGpioInit,
    .gpio_write = &ArduinoGpioWrite,
    .gpio_read = &ArduinoGpioRead,
    .gpio_set_interrupt = &ArduinoGpioSetInterrupt,
    .delay_ms = &ArduinoDelayMs,
};

static volatile bool DIO0FIRED = false;

LongFi::LongFi(RadioType radio, int reset, int cs, int dio0)
    : _cs(cs)
    , _dio0(dio0)
    , _rst(reset)
    {
        switch (radio)
        {   
            case SX126x:
            _radio = SX126xRadioNew();
            break;
            case SX1276:
            _radio = SX1276RadioNew();
            break;
            case SX1272:
            _radio = SX1272RadioNew();
            break;
        }

    };

void dio0_callback(){
    noInterrupts();
    DIO0FIRED = true;
    interrupts();
}

void LongFi::init(uint32_t oui, uint16_t device_id){
    // start the SPI library
    SPI.begin();

    digitalWrite(this->_rst, LOW);
    delay(1000);
    digitalWrite(this->_rst, HIGH);
    delay(1000);

    RfConfig_t config = {
        .oui = oui,
        .device_id = device_id,
    };
    _handle = longfi_new_handle(&ArduinoBindings, &this->_radio, config);

    CS_PIN = this->_cs;
    DIO0_PIN = this->_dio0;
    pinMode(this->_cs, OUTPUT);
    pinMode(this->_dio0, INPUT);
    attachInterrupt(DIO0_PIN, dio0_callback, RISING);
    longfi_init(&_handle);

};

void LongFi::enable_tcxo(void){
    longfi_enable_tcxo(&_handle);
}

//blocks until done
void LongFi::send(const uint8_t * data, size_t len){   
    noInterrupts();
    DIO0FIRED = false;
    interrupts();
    longfi_send(&this->_handle, LONGFI_QOS_0, data, len);

    ClientEvent event = ClientEvent_None;
    while( event!=ClientEvent_TxDone ){
        while(!DIO0FIRED){};
        noInterrupts();
        DIO0FIRED = false;
        interrupts();
        event = longfi_handle_event(&this->_handle, DIO0);
    }

};

void LongFi::set_buffer(uint8_t * buf, size_t len){
    longfi_set_buf(&this->_handle, buf, len);
};

uint32_t LongFi::get_random(){
    return longfi_get_random(&this->_handle);
};
