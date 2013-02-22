#include <cassert>

#include <serialization/SchemaDescription.hpp>
#include <serialization/CoreSchema.hpp>
#include <serialization/Schema.hpp>

#include <typeindex>

namespace serialization
{

    //---------------------
    // Add Types to Schema
    //---------------------

    Schema &operator<<(Schema &schema, const FieldType& )
    {
        CustomType *t = retrieveOrCreateCustomType<FieldType>(schema,"FieldType");
        assert(t->getNumFields() == 0);

        t->addField(FieldType(CoreSchema::ID_uint16, FieldType::VALUE, FieldType::NO_REPEAT),"typeId");
        t->addField(FieldType(CoreSchema::ID_uint8 , FieldType::VALUE, FieldType::NO_REPEAT),"kind"  );
        t->addField(FieldType(CoreSchema::ID_uint8 , FieldType::VALUE, FieldType::NO_REPEAT),"repeat");
        return schema;
    }

    Schema &operator<<(Schema &schema, const Field& )
    {
        CustomType *t = retrieveOrCreateCustomType<Field>(schema,"Field");

        assert(t->getNumFields() == 0);

        t->addField(FieldType(CoreSchema::ID_CustomType, FieldType::POINTER, FieldType::NO_REPEAT), "customType");
        t->addField(FieldType(CoreSchema::ID_uint32,     FieldType::VALUE,   FieldType::NO_REPEAT), "index");
        t->addField(FieldType(CoreSchema::ID_FieldType,  FieldType::VALUE,   FieldType::NO_REPEAT), "fieldType");
        t->addField(FieldType(CoreSchema::ID_char,       FieldType::VALUE,   FieldType::REPEAT_32), "name");

        return schema;
    }

    Schema &operator<<(Schema &schema, const PrimitiveType& )
    {
        CustomType *t = retrieveOrCreateCustomType<PrimitiveType>(schema,"PrimitiveType");

        assert(t->getNumFields() == 0);

        t->addField(FieldType(CoreSchema::ID_Schema, FieldType::POINTER, FieldType::NO_REPEAT), "schema");
        t->addField(FieldType(CoreSchema::ID_uint32, FieldType::VALUE,   FieldType::NO_REPEAT), "typeId");
        t->addField(FieldType(CoreSchema::ID_char,   FieldType::VALUE,   FieldType::REPEAT_32), "name");
        t->addField(FieldType(CoreSchema::ID_uint32, FieldType::VALUE,   FieldType::NO_REPEAT), "size");

        return schema;
    }

    Schema &operator<<(Schema &schema, const CustomType& )
    {
        CustomType *t = retrieveOrCreateCustomType<CustomType>(schema,"CustomType");
        assert(t->getNumFields() == 0);

        t->addField(FieldType(CoreSchema::ID_Schema, FieldType::POINTER, FieldType::NO_REPEAT), "schema");
        t->addField(FieldType(CoreSchema::ID_uint32, FieldType::VALUE,   FieldType::NO_REPEAT), "typeId");
        t->addField(FieldType(CoreSchema::ID_char,   FieldType::VALUE,   FieldType::REPEAT_32), "name");
        t->addField(FieldType(CoreSchema::ID_Field,  FieldType::VALUE,   FieldType::REPEAT_32), "fields");

        return schema;
    }


    Schema &operator<<(Schema &schema, const Schema& )
    {
        CustomType *t = retrieveOrCreateCustomType<Schema>(schema,"Schema");
        assert(t->getNumFields() == 0);

        t->addField(FieldType(CoreSchema::ID_uint32,  FieldType::VALUE, FieldType::NO_REPEAT), "freeTypeID");
        t->addField(FieldType(CoreSchema::ID_pointer, FieldType::VALUE, FieldType::REPEAT_32), "types");

        return schema;
    }

}
