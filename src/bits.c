//
// Created by Michael Lipinski on 25/01/2018.
//

#include <stdint.h>
#include "./bits.h"
#ifdef LOGGED_BITS
unsigned int bits_log_index = 0;

void bitLogClear() {
    bits_log_index = 0;
    for (int i = 0; i < BITS_LOG_SIZE; ++i) {
        bits_log[i] = (BitLogItem){};
    }
}

void bitSetWithSave(uint8_t *p, unsigned char m, enum BitLogType type) {
    if (bits_log_index >= BITS_LOG_SIZE) {
        return;
    }
    bits_log[bits_log_index++] = (BitLogItem){p, m, type};
    switch (type) {
        case bl_set:
            _bit_set(*p, m);
            break;
        case bl_clear:
            _bit_clear(*p, m);
            break;
        case bl_flip:
            _bit_flip(*p, m);
            break;
    }
}

#endif