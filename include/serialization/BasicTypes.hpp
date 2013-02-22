#pragma once

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <stdint.h>
#include <ostream>
#include <istream>
#include <fstream>
#include <stack>
#include <queue>
#include <typeindex>


#include <serialization/Pointer.hpp>

namespace serialization
{

    typedef std::type_index type_index;
    typedef std::string string;
    using   std::vector;
    using   std::unordered_map;
    using   std::unordered_set;
    using   std::stack;
    using   std::queue;

    typedef uint32_t TypeID;        // type identifier is a 32 bit unsiged integer
    typedef uint32_t RepeatCounter; // type identifier is a 32 bit unsiged integer

    typedef int8_t   int8;
    typedef uint8_t  uint8;
    typedef int16_t  int16;
    typedef uint16_t uint16;
    typedef int32_t  int32;
    typedef int64_t  int64;
    typedef uint32_t uint32;
    typedef uint64_t uint64;
    typedef float    float32;
    typedef double   float64;

    typedef void*    voidptr; // memory address is a 64 bit unsigned integer

    class FieldType;
    class Field;
    class Type;
    class PrimitiveType;
    class CustomType;
    class Schema;
    class CoreSchema;

    class Serializer;
    class Deserializer;

}
