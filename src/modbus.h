#pragma once
#include "stm32f10x.h"
#include "usart.h"

struct Array
{
    uint8_t *arr;
    uint32_t size;
    uint32_t max_size;

    void push(uint8_t value);
    void push(uint8_t *values, uint32_t size);
};

enum class ModbusState : uint8_t
{
    idle,
    receiving,
    received,
    processing,
    processed,
    transmitting
};

class Modbus
{
private:
    static uint8_t address;

    static Array pack_in;
    static Array pack_out;

    static usart::Usart *usart;
    static ModbusState state;

    // receive new byte of packet
    static void receiveData(uint8_t data);
    // 3.5 symbols silence for Modbus RTU
    static void finishPacket();

public:
    static void init(uint8_t _address, usart::Usart *_usart,
                     uint32_t data_in_max_size, uint32_t data_out_max_size);

    // read the request and generate a response
    static void processPacket();
    // transmit the generated response
    static void transmitPacket();

    static void formError(uint8_t error_code);
    static void addCRC();

    // getters
    static inline ModbusState getState() { return state; }
    static inline uint8_t getAddress() { return address; }

private:
    Modbus() = default;
};
