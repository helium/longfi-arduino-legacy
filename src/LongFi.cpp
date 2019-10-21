#include "LongFi.h"
#include "Arduino.h"
//#include "SPI.h"
#include "longfi-device/board.h"
#include "longfi-device/longfi.h"
#include "longfi-device/radio/sx1276/sx1276.h"
#include "longfi-device/radio/sx1272/sx1272.h"
#include "longfi-device/radio/sx126x/sx126x.h"

static int CS_PIN;
static int DIO0_PIN;

extern "C" {
}

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