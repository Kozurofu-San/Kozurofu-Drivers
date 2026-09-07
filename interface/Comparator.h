#pragma once

#include <cstdint>

namespace driver
{

class IComparator
{
    public:

    virtual ~IComparator() = default;

    virtual bool start() = 0;

    virtual uint32_t isHigher() = 0;
    virtual uint32_t isLower() = 0;
    
    virtual bool isInit() = 0;
};

}