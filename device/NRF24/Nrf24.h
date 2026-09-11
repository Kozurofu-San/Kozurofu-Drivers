#pragma once

#include "Nrf24Const.h"

#include "interface/Spi.h"
#include "interface/Serial.h"
#include "interface/Gpio.h"
#include "interface/Timer.h"

#include <cstdint>

namespace driver
{

class Nrf24Driver: public ISerial
{
    public:

    Nrf24Driver(ISpi &p, IGpio &ce, IGpio &irq, ITimer &timer)
        : _p(p), _ce(ce), _irq(irq), _timer(timer)
    {
    }
    ~Nrf24Driver() = default;

    bool init()
    {
        // Init check
        if (!_p.isInit() or !_timer.isInit())
        {
            return false;
        }

        // Speed check
        if (_p.getSpeed() > MaxSpeed or _p.getSpeed() == 0)
        {
            return false;
        }

        _ce.write(0);

        // Check if SPI works
        uint8_t status = readReg(Nrf24::STATUS);
        if (status = 0x0E)
        {
            _isInit = true;
        }

        return _isInit;
    }

    bool write(uint8_t *data, size_t len) override
    {
        // TODO: implement
        return false;
    }

    bool read (uint8_t *data, size_t len) override
    {
        // TODO: implement
        return false;
    }

    void setCallback(void (*cb)(uint32_t)) override
    {
        _cb = cb;
    }

    void setBuffer(uint8_t *buffer, size_t size) override
    {
        _buffer = buffer;
        _bufferSize = size;
    }

    // Callback when IRQ is set
    void interrupt()
    {
        _cb(0);
    }
    
    uint32_t getSpeed() const override
    {
        return _speed;
    }

    bool isInit()// override
    {
        return _isInit;
    }

    private:

    ISpi &_p;
    IGpio &_ce;
    IGpio &_irq;
    ITimer &_timer;

    void (*_cb)(uint32_t) = nullptr;
    uint8_t *_buffer = nullptr;
    size_t _bufferSize = 0;

    static constexpr uint32_t MaxSpeed = 8'000'000; // Hz

    uint32_t _speed; // Speed in Hz
    bool _isInit = false;

    uint8_t read(uint8_t reg, uint8_t *data, size_t len)
    {
        _p.enable();
        uint8_t status = _p.transfer(Nrf24::CmdReadRegister | reg);
        _p.read(data, len);
        _p.disable();
        return status;
    }

    uint8_t write(uint8_t reg, uint8_t *data, size_t len)
    {
        _p.enable();
        uint8_t status = _p.transfer(Nrf24::CmdWriteRegister | reg);
        _p.write(data, len);
        _p.disable();
        return status;
    }

    uint8_t readReg(uint8_t reg)
    {
        _p.enable();
        uint8_t status = _p.transfer(Nrf24::CmdReadRegister | reg);
        uint8_t ret = _p.transfer(0);
        _p.disable();
        return ret;
    }

    uint8_t writeReg(uint8_t reg, uint8_t data)
    {
        _p.enable();
        uint8_t status = _p.transfer(Nrf24::CmdWriteRegister | reg);
        _p.transfer(data);
        _p.disable();
        return status;
    }

};

}
