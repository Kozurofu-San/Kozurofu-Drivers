#pragma once

#include "interface/Spi.h"
#include "interface/Gpio.h"
#include "interface/Timer.h"
#include "Nrf24Const.h"

#include <cstdint>

namespace driver
{

class Nrf24Driver
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

        _isInit = true;

        _ce.write(1);
        uint8_t status;

        status = readReg(Nrf24::STATUS);
        // writeReg(Nrf24::RF_CH, 0x55);
        // status = readReg(Nrf24::RF_CH);

        return _isInit;
    }

    void write(uint8_t *data, size_t len)
    {
    }

    void read (uint8_t *data, size_t len)
    {
    }

    bool isInit()// override
    {
        return _isInit;
    }

    private:

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
        // _timer.delay(1);
        _p.transfer(Nrf24::CmdReadRegister | reg);
        // _timer.delay(1);
        uint8_t ret = _p.transfer(0);
        _p.disable();
        return ret;
    }

    bool writeReg(uint8_t reg, uint8_t data)
    {
        _p.enable();
        _p.transfer(Nrf24::CmdWriteRegister | reg);
        _p.transfer(data);
        _p.disable();
        return true;
    }

    ISpi &_p;
    IGpio &_ce;
    IGpio &_irq;
    ITimer &_timer;

    uint8_t _buffer[20];

    static constexpr uint32_t MaxSpeed = 8'000'000; // Hz

    bool _isInit = false;
};

}