#include <serialization/Type.hpp>

namespace serialization
{

    //------
    // Type
    //------

    Type::Type():
        schema(nullptr),
        typeId(-1),
        name("")
    {}

    Type::Type(Schema *schema, int typeId, string name):
        schema(schema),
        typeId(typeId),
        name(name)
    {}

    TypeID Type::getTypeID() const
    {
        return typeId;
    }

    string Type::getName() const
    {
        return name;
    }

    Schema *Type::getSchema() const
    {
        return schema;
    }

    PrimitiveType *Type::asPrimitiveType()
    {
        return nullptr;
    }

    CustomType *Type::asCustomType()
    {
        return nullptr;
    }

}
