#include "keymatrix.h"
#include "timers.h"

gpio::Pin *KeyMatrix::out_pins = nullptr;
uint8_t KeyMatrix::out_pins_count = 0;

gpio::Pin *KeyMatrix::in_pins = nullptr;
uint8_t KeyMatrix::in_pins_count = 0;

uint32_t KeyMatrix::period_ms = 0;

bool *KeyMatrix::keys = nullptr;

uint8_t KeyMatrix::current_out_pin = 0;
KeyMatrixState KeyMatrix::state = KeyMatrixState::idle;

inline bool &KeyMatrix::key(uint8_t row, uint8_t col)
{
    return keys[row * in_pins_count + col];
}

bool KeyMatrix::getKey(uint8_t row, uint8_t col)
{
    return key(row, col);
}

KeyMatrixState KeyMatrix::getState()
{
    return state;
}


void KeyMatrix::init(gpio::Pin *_out_pins, uint8_t _out_pins_count,
                     gpio::Pin *_in_pins, uint8_t _in_pins_count,
                     uint32_t _period_ms)
{
    out_pins = _out_pins;
    out_pins_count = _out_pins_count;
    in_pins = _in_pins;
    in_pins_count = _in_pins_count;
    period_ms = _period_ms;
    keys = new bool[_out_pins_count * _in_pins_count];

    state = KeyMatrixState::idle;
    for (uint8_t i = 0; i < out_pins_count * in_pins_count; i++)
    {
        keys[i] = 0;
    }

    // set all out pins to high
    gpio::setMultiplePins(out_pins, out_pins_count, gpio::Level::High);
}

void KeyMatrix::startProcessing()
{
    state = KeyMatrixState::wait_next;

    // set the current out pin to low
    current_out_pin = 0;
    out_pins[current_out_pin].setOutput(gpio::Level::Low);

    // start processing the keymatrix with a systick interrupt
    systick::setCallback(waitNext_Callback);
    systick::waitMsInt(period_ms);
}

void KeyMatrix::waitNext_Callback()
{
    // set corresponding keys levels
    for (uint8_t col = 0; col < in_pins_count; col++)
    {
        key(current_out_pin, col) = (bool)in_pins[col].getInput();
    }

    // go to next row
    current_out_pin++;
    // set high level in all rows
    gpio::setMultiplePins(out_pins, 3, gpio::Level::High);

    // if last row, finish
    if (current_out_pin == out_pins_count)
    {
        state = KeyMatrixState::finished;
    }
    else
    {
        // set low level in current row
        out_pins[current_out_pin].setOutput(gpio::Level::Low);
        // wait next callback
        systick::waitMsInt(period_ms);
    }
}
