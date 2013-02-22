#pragma once

#include <serialization/Type.hpp>
#include <serialization/Field.hpp>
#include <serialization/FieldType.hpp>

namespace serialization
{

    //------------
    // CustomType
    //------------

    class CustomType: public Type
    {
    public:
        CustomType();
        CustomType(Schema *schema, TypeID typeId, string name);

        void addField(FieldType fieldType, string name);

        int  getNumFields();

        CustomType *asCustomType();

        const vector<Field>& getFields() const;

    private:
        std::vector<Field> fields;


        // give access to private memeber to the deserialization function
        friend Deserializer& operator>>(Deserializer& deserializer, CustomType &customType);

    };

}
