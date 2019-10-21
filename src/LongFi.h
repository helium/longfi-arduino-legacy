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
  private:
    int _cs;
    int _dio0;
    int _rst;
    Radio_t _radio;
    LongFi_t _handle;
};

#endif