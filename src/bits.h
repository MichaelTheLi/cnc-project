//
// Created by Michael Lipinski on 25/01/2018.
//

#ifndef CLION_AVR_BITS_H
#define CLION_AVR_BITS_H

#define _bit_set(p,m) ((p) |= (m))
#define _bit_clear(p,m) ((p) &= ~(m))
#define _bit_flip(p,m) ((p) ^= (m))

enum BitLogType {bl_set, bl_clear, bl_flip};
typedef struct {
    uint8_t *addr;
    unsigned char bit;
    enum BitLogType type;
} BitLogItem;

BitLogItem bits_log[1024];

void bitSetWithSave(uint8_t *p, unsigned char m, enum BitLogType type);
void bitLogClear();

#ifdef LOGGED_BITS

#define bit_set(p,m) (bitSetWithSave)(&(p), m, bl_set)
#define bit_clear(p,m) (bitSetWithSave)(&(p), m, bl_clear)
#define bit_flip(p,m) (bitSetWithSave)(&(p), m, bl_flip)

#else

#define bit_set(p,m) _bit_set(p, m)
#define bit_clear(p,m) _bit_clear(p, m)
#define bit_flip(p,m) _bit_flip(p, m)

#endif


#define BIT(x) (0x01 << (x))
#define LONGBIT(x) ((unsigned long)0x00000001 << (x))

#define bit_get(p,m) ((p) & (m))
#define bit_write(c,p,m) ((c) ? bit_set(p,m) : bit_clear(p,m))


#endif //CLION_AVR_BITS_H
