#include <serialization/CustomType.hpp>

namespace serialization
{

    //---------------
    // CustomType
    //---------------

    CustomType::CustomType():
        Type()
    {}

    CustomType::CustomType(Schema *schema, TypeID typeId, string name):
        Type(schema, typeId, name)
    {}

    void CustomType::addField(FieldType fieldType, string name)
    {
        fields.push_back(Field(this,fields.size(),fieldType,name));
    }

    int CustomType::getNumFields()
    {
        return fields.size();
    }

    CustomType *CustomType::asCustomType()
    {
        return this;
    }

    const vector<Field> &CustomType::getFields() const
    {
        return fields;
    }

}
