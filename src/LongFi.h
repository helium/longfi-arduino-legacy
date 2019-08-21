#ifndef LongFi_h
#define LongFi_h

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "longfi-device/longfi.h"

class LongFi
{
  public:
    enum RadioType 
    {
      SX126x,
      SX1276,
      SX1272,
    };

    LongFi(RadioType radio, int reset, int cs, int dio0);
    void init(uint32_t oui, uint16_t device_id);
    void send(const uint8_t * data, size_t len);
    void set_buffer(uint8_t * buf, size_t len);
    uint32_t get_random();
  private:
    int _cs;
    int _dio0;
    int _rst;
    Radio_t _radio;
    LongFi_t _handle;
};

#endif