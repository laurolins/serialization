#include <serialization/PrimitiveType.hpp>

namespace serialization
{

    //---------------
    // PrimitiveType
    //---------------

    PrimitiveType::PrimitiveType():
        Type(), size(-1)
    {}

    PrimitiveType::PrimitiveType(Schema *schema, TypeID typeId, string name, int size):
        Type(schema, typeId, name), size(size)
    {}

    int PrimitiveType::getSize() const
    {
        return size;
    }

    PrimitiveType *PrimitiveType::asPrimitiveType()
    {
        return this;
    }

}
