#include "modbus.h"
#include "misc.h"
#include "keymatrix.h"
#include "timers.h"

uint8_t Modbus::address = 0x00;
Array Modbus::pack_in = {nullptr, 0, 0};
Array Modbus::pack_out = {nullptr, 0, 0};
usart::Usart *Modbus::usart = nullptr;
ModbusState Modbus::state = ModbusState::idle;

// Array methods
void Array::push(uint8_t value)
{
    assert_param(size + 1 <= max_size);
    if (size + 1 > max_size)
    {
        return;
    }

    arr[size] = value;
    size++;
}

void Array::push(uint8_t *values, uint32_t len)
{
    assert_param(size + len <= max_size);
    if (size + len > max_size)
    {
        return;
    }

    for (uint32_t i = 0; i < len; i++)
    {
        arr[size] = values[i];
        size++;
    }
}

static uint16_t CRC16(const uint8_t *buf, uint32_t len)
{
    uint16_t crc = 0xFFFF;
    uint32_t i = 0;
    char bit = 0;

    for (i = 0; i < len; i++)
    {
        crc ^= buf[i];

        for (bit = 0; bit < 8; bit++)
        {
            if (crc & 0x0001)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc;
}

void Modbus::init(uint8_t _address, usart::Usart *_usart,
                  uint32_t data_in_max_size, uint32_t data_out_max_size)
{
    address = _address;
    usart = _usart;

    pack_in.max_size = data_in_max_size;
    pack_in.size = 0;
    pack_in.arr = new uint8_t[data_in_max_size];

    pack_out.max_size = data_out_max_size;
    pack_out.size = 0;
    pack_out.arr = new uint8_t[data_out_max_size];

    usart->setReceiveCallback(&Modbus::receiveData);
    usart->enableReceiver();
    usart->enableTransmitter();
}

void Modbus::receiveData(uint8_t data)
{
    if (state == ModbusState::idle)
    {
        state = ModbusState::receiving;
        pack_in.size = 0;
        // 3.5+1 symbols - separate time
        static uint32_t separate_time_ms = 45000 / usart->getBaudrate();
        // set callback for systick to separate packets by timer
        systick::setCallback(&finishPacket);
        systick::waitMsInt(separate_time_ms, false);
    }
    // reset timer that separates packets
    systick::resetCounter();
    // add received byte to packet
    pack_in.push(data);
}

void Modbus::finishPacket()
{
    state = ModbusState::received;
    systick::disableTimer();
}

void Modbus::formError(uint8_t error_code)
{
    pack_out.size = 0;
    pack_out.push(address);
    pack_out.push(pack_in.arr[1] | 0x80); // add error bit
    pack_out.push(error_code);            // send error code
    addCRC();
    state = ModbusState::processed;
}

void Modbus::addCRC()
{
    uint16_t crc = CRC16(pack_out.arr, pack_out.size);
    pack_out.push(crc & 0x00FF);
    pack_out.push((crc & 0xFF00) >> 8);
}

void Modbus::processPacket()
{
    state = ModbusState::processing;

    // ignore packets that are not for us
    if (pack_in.arr[0] != address)
    {
        state = ModbusState::idle;
        return;
    }

    // ignore empty packets
    if (pack_in.size <= 4)
    {
        state = ModbusState::idle;
        return;
    }

    // check crc, ignore if it's not valid
    uint16_t crc = CRC16(pack_in.arr, pack_in.size - 2);
    if ((pack_in.arr[6] | (pack_in.arr[7] << 8)) != crc)
    {
        state = ModbusState::idle;
        return;
    }

    // check function code (only 0x02 allowed)
    if (pack_in.arr[1] != 0x02)
    {
        formError(0x01); // error code 0x01 - invalid function code
        return;
    }

    // size of packet for 0x02 function code must be 6+2
    if (pack_in.size != 8)
    {
        formError(0x03); // error code 0x03 - invalid data
        return;
    }

    uint16_t start_address = (uint16_t)pack_in.arr[3] |
                             (uint16_t)pack_in.arr[2] << 8u;

    uint16_t registers_count = ((uint16_t)pack_in.arr[5]) |
                               ((uint16_t)pack_in.arr[4] << 8u);

    uint8_t key_matrix_rows = KeyMatrix::getRowCount();
    uint8_t key_matrix_cols = KeyMatrix::getColCount();

    // check address and count of requested registers
    if (start_address + registers_count > key_matrix_cols * key_matrix_rows ||
        registers_count == 0)
    {
        formError(0x03); // error code 0x02 - invalid address or registers count
        return;
    }

    // start processing the keymatrix, wait for finish
    KeyMatrix::startProcessing();
    while (KeyMatrix::getState() != KeyMatrixState::finished)
        ;

    // start to form the packet: address, function code
    pack_out.size = 0;
    pack_out.push(address);
    pack_out.push(pack_in.arr[1]); // function code

    uint16_t start_byte = start_address / 8u;
    uint16_t end_byte = (start_address + registers_count - 1u) / 8u;
    uint8_t bytes_count = end_byte - start_byte + 1u;

    // add bytes count to the packet
    pack_out.push(bytes_count);

    // pick out only requested registers
    uint16_t curr_byte_num = start_byte;
    uint8_t curr_byte = 0x00u;
    for (uint16_t reg = start_address; reg < start_address + registers_count; reg++)
    {
        if (reg / 8 > curr_byte_num)
        {
            pack_out.push(curr_byte);
            curr_byte_num++;
            curr_byte = 0x00;
        }
        // curr_byte |= (uint16_t)KeyMatrix::getKey(reg / key_matrix_rows, reg % key_matrix_rows) << (reg % 8u);
        curr_byte |= (uint16_t)KeyMatrix::getKey(0, reg) << (reg % 8u);
    }
    pack_out.push(curr_byte);

    addCRC();
    state = ModbusState::processed;
}

void Modbus::transmitPacket()
{
    usart->transmit(pack_out.arr, pack_out.size);
    state = ModbusState::idle;
}
