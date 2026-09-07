#pragma once

#include "Ads1115Const.h"

#include "interface/I2c.h"
#include "interface/Adc.h"
#include "interface/Gpio.h"
#include "interface/Timer.h"

#include <cstdint>

/* // Temperature / Pressure sensor

#include "device/DHT22/Dht22.h"

    I2cController i2c2 {I2C2};
    I2cDriver i2c_tempPress {i2c2};
    Bmp280Driver<II2c> tempPress {i2c_tempPress};
    
    // I2C
    GpioDriver::remap(AFIO_MAPR_I2C1_REMAP, false);
    GpioDriver::mode(GPIOB, 11, GpioDriver::Mode::AlternateOpendrain);   // SDA
    GpioDriver::mode(GPIOB, 10, GpioDriver::Mode::AlternateOpendrain);   // SCL
    CHECK(p.i2c2.init(400'000));

    // Temperature / Pressure sensor
    p.i2c2.check(II2c::Address::BMP280 + 1);
    p.i2c_tempPress.init(II2c::Address::BMP280 + 1);
    p.tempPress.init();
*/

namespace driver
{

class Ads1115Driver : IAdc
{
    public:

    Ads1115Driver(II2c &p)
        : _p(p)
    {
    }

    bool init()
    {
        // Init check
        if (!_p.isInit())
        {
            return false;
        }

        _isInit = true;
        
        return _isInit;
    }

    bool start() override
    {
        return true;
    }

    uint16_t getRawValue() override
    {
        return 0;
    }
    
    uint32_t getVoltage() override
    {
        return 0;
    }

    bool isInit() override
    {
        return _isInit;
    }
    
    private:

    II2c &_p;
    
    bool _isInit = false;

    static const size_t Timeout = 10;

    bool write(uint8_t addr, uint8_t data)
    {
        _p.start();
        _p.address(II2c::Write);
        _p.write(addr);
        _p.write(data);
        _p.stop();
        return true;
    }
    
    uint16_t read(uint8_t addr)
    {
        uint16_t ret;
        uint16_t *ptr = &ret;
        _p.start();
        _p.address(II2c::Write);
        _p.write(addr);
        _p.start();
        _p.address(II2c::Read);
        *ptr++ = _p.read();
        *ptr = _p.read(true);
        _p.stop();
        return true;
    }
    
};
}
