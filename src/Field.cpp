#include <serialization/Field.hpp>

namespace serialization
{

    //-------
    // Field
    //-------

    Field::Field():
        type(nullptr), index(-1), fieldType(FieldType()), name("")
    {}

    Field::Field(CustomType *type, int index, FieldType fieldType, string name):
        type(type), index(index), fieldType(fieldType), name(name)
    {}

    uint32 Field::getIndex() const
    {
        return index;
    }

    FieldType Field::getFieldType() const
    {
        return fieldType;
    }

    const string& Field::getName() const
    {
        return name;
    }

    CustomType* Field::getType() const
    {
        return type;
    }


}
