#pragma once

#include "BasicTypes.hpp"

namespace serialization
{

    //-------
    // Type
    //-------

    class Type
    {
    public:
        Type();
        Type(Schema *schema, int typeId, string name);

        virtual ~Type() {}

        TypeID  getTypeID() const;
        string  getName() const;
        Schema *getSchema() const;

        virtual PrimitiveType *asPrimitiveType();
        virtual CustomType    *asCustomType();

    protected:
        Schema *schema;
        TypeID  typeId;
        string  name;
    };

}
