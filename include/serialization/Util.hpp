#pragma once

#include <serialization/Serializer.hpp>
#include <serialization/Deserializer.hpp>
#include <serialization/Schema.hpp>

#include <cxxabi.h>

namespace serialization {

namespace util {

template <typename T>
std::string getTypeName()
{
char   buffer[10000];
size_t length = 10000;
int    status;
abi::__cxa_demangle(typeid(T).name(),
                    buffer,
                    &length,
                    &status);
buffer[length] = 0;
return std::string(buffer);
}


    template <typename T>
    void addObjectFactory(serialization::Schema &schema, serialization::Deserializer &deserializer)
    {
        deserializer.addObjectFactory(schema.getTypeByName(getTypeName<T>())->getTypeID(), new ObjectFactory<T>());
    }


}

}
