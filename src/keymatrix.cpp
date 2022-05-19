#include "keymatrix.h"

gpio::Pin *KeyMatrix::out_pins = nullptr;
uint8_t KeyMatrix::out_pins_count = 0;

gpio::Pin *KeyMatrix::in_pins = nullptr;
uint8_t KeyMatrix::in_pins_count = 0;

uint8_t *KeyMatrix::keys = nullptr;

uint8_t KeyMatrix::current_out_pin = 0;
KeyMatrixState KeyMatrix::state = KeyMatrixState::idle;

inline uint8_t &KeyMatrix::key(uint8_t i, uint8_t j)
{
    return keys[i * out_pins_count + j];
}

uint8_t KeyMatrix::getKey(uint8_t i, uint8_t j)
{
    return key(i, j);
}

void KeyMatrix::init(gpio::Pin *_out_pins, uint8_t _out_pins_count,
                     gpio::Pin *_in_pins, uint8_t _in_pins_count)
{
    out_pins = _out_pins;
    out_pins_count = _out_pins_count;
    in_pins = _in_pins;
    in_pins_count = _in_pins_count;
    keys = new uint8_t[_out_pins_count * _in_pins_count];

    current_out_pin = 0;
    state = KeyMatrixState::idle;
    for (uint8_t i = 0; i < out_pins_count * in_pins_count; i++)
    {
        keys[i] = 0;
    }

    // set all out pins high
    gpio::setMultiplePins(out_pins, out_pins_count, gpio::Level::High);
}
