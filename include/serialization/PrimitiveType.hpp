#pragma once

#include <serialization/Type.hpp>
#include <serialization/BasicTypes.hpp>

namespace serialization
{

//---------------
// PrimitiveType
//---------------

class PrimitiveType: public Type
{
public:
    PrimitiveType();
    PrimitiveType(Schema *schema, TypeID typeId, string name, int size);

    int  getSize() const;
    PrimitiveType *asPrimitiveType();
private:
    int  size; // size in bytes

    // give access to private memeber to the deserialization function
    friend Deserializer& operator>>(Deserializer& deserializer, PrimitiveType &primitiveType);

};

}
