#include <serialization/Header.hpp>

namespace serialization
{

    //--------
    // Header
    //--------

    Header::Header():
        typeIdAndFlags(0),
        oldAddr(nullptr)
    {}

    Header::Header(TypeID typeId, RawPointer oldAddr):
        oldAddr(oldAddr)
    {
        typeIdAndFlags  = 0xBA << 24;
        typeIdAndFlags |= (typeId & 0xFFFF) << 8;
        typeIdAndFlags |= 0xAB;
    }

    TypeID Header::getTypeId()
    {
        return ((typeIdAndFlags >> 8) & 0xFFFF);
    }

}
