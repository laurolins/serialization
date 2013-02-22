#include <serialization/CoreSchema.hpp>
#include <serialization/Field.hpp>
#include <serialization/FieldType.hpp>
#include <serialization/CustomType.hpp>
#include <serialization/PrimitiveType.hpp>
#include <serialization/SchemaSerialization.hpp>
#include <serialization/SchemaDescription.hpp>

#include <cassert>

namespace serialization
{

    void CoreSchema::initCoreSchema(Schema &schema)
    {
        assert (schema.size() == 0);

        schema.addPrimitiveType<char>("char");                 // 0
        schema.addPrimitiveType<int8>("int8");                 // 1
        schema.addPrimitiveType<int16>("int16");               // 2
        schema.addPrimitiveType<int32>("int32");               // 3
        schema.addPrimitiveType<int64>("int64");               // 4
        schema.addPrimitiveType<uint8>("uint8");               // 5
        schema.addPrimitiveType<uint16>("uint16");             // 6
        schema.addPrimitiveType<uint32>("uint32");             // 7
        schema.addPrimitiveType<uint64>("uint64");             // 8
        schema.addPrimitiveType<float32>("float32");           // 9
        schema.addPrimitiveType<float64>("float64");           // 10
        schema.addPrimitiveType<RawPointer>("pointer");        // 11
        schema.addPrimitiveType<RawPointer>("tagged_pointer"); // 12
        schema << FieldType();                                 // 13
        schema << Field();                                     // 14
        schema << PrimitiveType();                             // 15
        schema << CustomType();                                // 16
        schema << Schema();                                    // 17

        // FIRST_USER_TYPEID is 256
        schema.setFreeTypeID(FIRST_USER_TYPEID);    // from 0x000d to 0x00ff, the types are reserved
                                                    // custom types should start from 0x0100

    }

    void CoreSchema::initCoreDeserializer(Deserializer &deserializer)
    {
//        deserializer.addObjectFactory(ID_int8,          new ObjectFactory<int8>());
//        deserializer.addObjectFactory(ID_int16,         new ObjectFactory<int16>());
//        deserializer.addObjectFactory(ID_int32,         new ObjectFactory<int32>());
//        deserializer.addObjectFactory(ID_int64,         new ObjectFactory<int64>());
//        deserializer.addObjectFactory(ID_uint8,         new ObjectFactory<uint8>());
//        deserializer.addObjectFactory(ID_uint16,        new ObjectFactory<uint16>());
//        deserializer.addObjectFactory(ID_uint32,        new ObjectFactory<uint32>());
//        deserializer.addObjectFactory(ID_uint64,        new ObjectFactory<uint64>());
//        deserializer.addObjectFactory(ID_float32,       new ObjectFactory<float32>());
//        deserializer.addObjectFactory(ID_float64,       new ObjectFactory<float64>());
//        deserializer.addObjectFactory(ID_char,          new ObjectFactory<char>());
//        deserializer.addObjectFactory(ID_pointer,       new ObjectFactory<RawPointer>());
//        deserializer.addObjectFactory(ID_tagged_pointer,new ObjectFactory<TaggedPointer>());


        deserializer.addObjectFactory(ID_FieldType,     new ObjectFactory<FieldType>());
        deserializer.addObjectFactory(ID_Field,         new ObjectFactory<Field>());
        deserializer.addObjectFactory(ID_PrimitiveType, new ObjectFactory<PrimitiveType>());
        deserializer.addObjectFactory(ID_CustomType,    new ObjectFactory<CustomType>());
        deserializer.addObjectFactory(ID_Schema,        new ObjectFactory<Schema>());
    }

}
