#include "misc.h"

static const uint32_t bit_band_reg_beg = 0x40000000;
static const uint32_t bit_band_reg_end = 0x400FFFFF;
static const uint32_t bit_band_base = 0x42000000;

static inline uint32_t getBitBandAddress(uint32_t address, uint8_t bit)
{
    uint32_t address_offset = address - bit_band_reg_beg;
    uint32_t bit_addr = bit_band_base + address_offset * 32 + bit * 4;
    return bit_addr;
}

void setBit(uint32_t address, uint8_t bit, bool value)
{
    // assert bit-band region
    assert_param(address >= bit_band_reg_beg || address <= bit_band_reg_end);

    uint32_t bit_addr = getBitBandAddress(address, bit);
    *(uint32_t *)(bit_addr) = value;
}

bool getBit(uint32_t address, uint8_t bit)
{
    // assert bit-band region
    assert_param(address >= bit_band_reg_beg || address <= bit_band_reg_end);

    uint32_t bit_addr = getBitBandAddress(address, bit);
    return *(uint32_t *)(bit_addr);
}

// infinite loop when assert failed
#ifdef USE_FULL_ASSERT
void assert_failed(const char *file, uint32_t line)
{
    while (1)
        ;
}
#endif
