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
    // it's rows
    static gpio::Pin *out_pins;
    static uint8_t out_pins_count;

    // it's columns
    static gpio::Pin *in_pins;
    static uint8_t in_pins_count;

    static uint32_t period_ms;

    // array of keys
    static bool *keys;

    static uint8_t current_out_pin;
    static KeyMatrixState state;

    static inline bool &key(uint8_t row, uint8_t col);

    static void waitNext_Callback();

public:
    static bool getKey(uint8_t row, uint8_t col);
    static KeyMatrixState getState();

    static void init(gpio::Pin *_out_pins, uint8_t _out_pins_count,
                     gpio::Pin *_in_pins, uint8_t _in_pins_count,
                     uint32_t _period_ms);

    static void startProcessing();

private:
    KeyMatrix() = default;
};
