#pragma once

#include <serialization/BasicTypes.hpp>
#include <serialization/Serializer.hpp>
#include <serialization/Deserializer.hpp>

#include <vector>

namespace serialization
{

    // serialize schema custom types
    Serializer &operator<<(Serializer &serializer, const Schema& schema);
    Serializer &operator<<(Serializer &serializer, const FieldType& fieldType);
    Serializer &operator<<(Serializer &serializer, const Field& field);
    Serializer &operator<<(Serializer &serializer, const CustomType& customType);
    Serializer &operator<<(Serializer &serializer, const PrimitiveType& primitiveType);

    // deserialize schema custom types
    Deserializer &operator>>(Deserializer &deserializer, Schema& schema);
    Deserializer &operator>>(Deserializer &deserializer, FieldType& fieldType);
    Deserializer &operator>>(Deserializer &deserializer, Field& field);
    Deserializer &operator>>(Deserializer &deserializer, CustomType& customType);
    Deserializer &operator>>(Deserializer &deserializer, PrimitiveType& primitiveType);

    // serialize pointers
//    template<class T>
//    Deserializer& operator>>(Serializer &serializer, std::vector<T*> &vec)
//    {

//        serializer << (uint32) vec.size();
//        for (T* obj: vec)
//            serializer << obj;
//        return serializer;
//    }


}
