#pragma once

#include <serialization/BasicTypes.hpp>

namespace serialization {

    //--------
    // Header
    //--------

    struct Header
    {
        Header();
        Header(TypeID typeId, RawPointer oldAddr);

    public:
        TypeID getTypeId();

    public: // attributes
        uint32     typeIdAndFlags;
        RawPointer oldAddr;
    };

}
