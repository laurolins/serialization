#pragma once

#include <serialization/BasicTypes.hpp>
#include <serialization/Schema.hpp>
#include <serialization/Deserializer.hpp>

namespace serialization
{

    //---------------
    // DefaultSchema
    //---------------

    /*!
     * \brief Initialize Basic Serialization Infrastructure that should not change.
     */
    class CoreSchema {
    public:

        static const int LAST_BUILTIN_TYPEID = 0x0FF; // 255
        static const int FIRST_USER_TYPEID   = 0x100; // 256
        static const int MAXIMUM_USER_TYPEID   = 0xFFFF; // 65535

        enum ID {
            ID_char,            // 0
            ID_int8,            // 1
            ID_int16,           // 2
            ID_int32,           // 3
            ID_int64,           // 4
            ID_uint8,           // 5
            ID_uint16,          // 6
            ID_uint32,          // 7
            ID_uint64,          // 8
            ID_float32,         // 9
            ID_float64,         // 10
            ID_pointer,         // 11
            ID_tagged_pointer,  // 12
            ID_FieldType,       // 13
            ID_Field,           // 14
            ID_PrimitiveType,   // 15
            ID_CustomType,      // 16
            ID_Schema           // 17
        };

        static void initCoreSchema(Schema &schema);

        static void initCoreDeserializer(Deserializer &deserializer);

        // static void initializeDefaultPrimitiveTypes(Schema &schema);

    };


}
