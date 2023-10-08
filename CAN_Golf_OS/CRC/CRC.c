
#include "CRC.h"
#include "stdint.h"
unsigned char table[256] = {0};

uint8_t crc_8(uint8_t* data, uint8_t len) {
  unsigned crc = 0;
  int i, j;
  for (j = len; j; j--, data++) {
    crc ^= (*data << 8);
    for(i = 8; i; i--) {
      if (crc & 0x8000)
        crc ^= (0x1070 << 3);
      crc <<= 1;
    }
  }
  return (uint8_t)(crc >> 8);
}
