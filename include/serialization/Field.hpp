#pragma once

#include <serialization/FieldType.hpp>
#include <serialization/CustomType.hpp>

namespace serialization
{

    //-------
    // Field
    //-------

    class Field
    {
    public:
        Field();
        Field(CustomType *type, int index, FieldType fieldType, string name);

        uint32      getIndex() const;
        FieldType   getFieldType() const;
        const string& getName() const;

        CustomType *getType() const;

    private:
        CustomType *type;
        uint32      index; // index
        FieldType   fieldType;
        string      name;

        // give access to private memeber to the deserialization function
        friend Deserializer& operator>>(Deserializer& deserializer, Field &field);

    };

}
