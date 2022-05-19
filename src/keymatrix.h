#pragma once
#include "stm32f10x.h"
#include "gpio.h"

enum class KeyMatrixState : uint8_t
{
    idle,
    wait_next,
    finished
};

class KeyMatrix
{
private:
    // in rows
    static gpio::Pin *out_pins;
    static uint8_t out_pins_count;

    // in columns
    static gpio::Pin *in_pins;
    static uint8_t in_pins_count;

    // array of keys
    static uint8_t *keys;

    static uint8_t current_out_pin;
    static KeyMatrixState state;

    static inline uint8_t &key(uint8_t i, uint8_t j);

public:
    static uint8_t getKey(uint8_t i, uint8_t j);

    static void init(gpio::Pin *_out_pins, uint8_t _out_pins_count,
                     gpio::Pin *_in_pins, uint8_t _in_pins_count,
                     uint8_t *_keys_array);

private:
    KeyMatrix() = default;
};
