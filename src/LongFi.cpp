#include "LongFi.h"
#include "Arduino.h"
#include "SPI.h"
#include "longfi-device/board.h"
#include "longfi-device/longfi.h"
#include "longfi-device/radio/sx1276/sx1276.h"
#include "longfi-device/radio/sx1272/sx1272.h"
#include "longfi-device/radio/sx126x/sx126x.h"

#define RAND_MIN 0
#define RAND_MAX 2147483647 

static int NSS_PIN;
static int DIO0_PIN;
static int TCXO_PIN;
static int RESET_PIN;

extern "C" {

void BoardReset(bool enable)
{
  //Radio Reset
  digitalWrite(RESET_PIN, LOW);

  delay(1);

  digitalWrite(RESET_PIN, HIGH); 

  delay(6);

  digitalWrite(RESET_PIN, LOW);

  delay(1);

  digitalWrite(RESET_PIN, HIGH);
}

uint8_t BoardSpiInOut(LF_Spi_t *s, uint8_t outData){
    return SPI.transfer(outData);
}

void BoardDelayMs(uint32_t ms){
    delay(ms);
}

void BoardSpiNss(bool sel){
    if (sel) {
        digitalWrite(NSS_PIN, HIGH);

    } else {
        digitalWrite(NSS_PIN, LOW);
    }
}

uint32_t BoardGetRandomBits(uint8_t r_param)
{
   return random(RAND_MIN, RAND_MAX); 
}

bool BoardBusyPinStatus(void)
{
    return true;
}

uint8_t BoardReducePower(uint8_t amount)
{
    return 0;
}

uint8_t BoardSetBoardTcxo(bool enable)
{
    if(enable)
    {
        digitalWrite(TCXO_PIN, HIGH);
    }
    else
    {
        digitalWrite(TCXO_PIN, LOW);
    }
    return 6;
}

void BoardSetAntennaPins(AntPinsMode_t mode, uint8_t power)
{
}
}

static BoardBindings_t BoardBindings = {
    .spi_in_out = BoardSpiInOut,
    .spi_nss = BoardSpiNss,
    .reset = BoardReset,
    .delay_ms = BoardDelayMs,
    .get_random_bits = BoardGetRandomBits,
    .busy_pin_status = NULL,
    .reduce_power = NULL, 
    .set_board_tcxo = NULL,
    .set_antenna_pins = NULL,
};

uint8_t preshared_key[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
uint8_t *GetPresharedKey(){
  return preshared_key;
}

static volatile bool DIO0FIRED = false;

LongFi::LongFi(RadioType radio, int reset, int nss, int dio0)
    : _nss(nss)
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

LongFi::LongFi(RadioType radio, int reset, int nss, int dio0, int tcxo)
    : _nss(nss)
    , _dio0(dio0)
    , _rst(reset)
    , _tcxo(tcxo)
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
        // TCXO Exist
        BoardBindings.set_board_tcxo = BoardSetBoardTcxo;
    };

void dio0_callback(){
    noInterrupts();
    DIO0FIRED = true;
    interrupts();
}

void LongFi::init(uint32_t oui, uint16_t device_id){

    union LongFiAuthCallbacks auth_cb = {.preshared_key = preshared_key};

    LongFiConfig_t lf_config = {
        .oui = oui,
        .device_id = device_id,
        .auth_mode = PresharedKey128, 
    };

    _handle = longfi_new_handle(&BoardBindings, &this->_radio, lf_config, auth_cb);

    NSS_PIN = this->_nss;
    DIO0_PIN = this->_dio0;
    TCXO_PIN = this->_tcxo;
    RESET_PIN = this->_rst;
    pinMode(this->_nss, OUTPUT);
    pinMode(this->_dio0, INPUT);
    pinMode(this->_tcxo, OUTPUT);
    pinMode(this->_rst, OUTPUT);
    attachInterrupt(DIO0_PIN, dio0_callback, RISING);

    // Seed RNG with A0 analog read value
    randomSeed(analogRead(A0));

    longfi_init(&_handle);
};

//blocks until done
void LongFi::send(const uint8_t * data, size_t len){   
    noInterrupts();
    DIO0FIRED = false;
    interrupts();
    longfi_send(&this->_handle, data, len);

    ClientEvent_t event = ClientEvent_None;
    while( event!=ClientEvent_TxDone ){
        while(!DIO0FIRED){};
        noInterrupts();
        DIO0FIRED = false;
        interrupts();
        event = longfi_handle_event(&this->_handle, DIO0);
    }
};
