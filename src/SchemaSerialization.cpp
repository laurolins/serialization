#include <iostream>
#include <serialization/SchemaLog.hpp>

#include <serialization/SchemaSerialization.hpp>

#include <serialization/Serializer.hpp>
#include <serialization/Deserializer.hpp>

#include <serialization/Field.hpp>
#include <serialization/FieldType.hpp>
#include <serialization/PrimitiveType.hpp>
#include <serialization/CustomType.hpp>
#include <serialization/Schema.hpp>
#include <serialization/CoreSchema.hpp>

namespace serialization
{

    //-----------------------------------
    // serialize schema custom types
    //-----------------------------------


    typedef SequenceReader<std::string, char> StrSeq;

    Serializer &operator<<(Serializer &serializer, const FieldType& fieldType)
    {
//        std::ostream &ostream = serializer.getStream();
        serializer << fieldType.typeId << fieldType.kind << fieldType.repeat;
        return serializer;
    }


    Serializer &operator<<(Serializer &serializer, const Field& field)
    {
        serializer << Pointer<CustomType>(field.getType())
                   << field.getIndex()
                   << field.getFieldType()
                   << StrSeq(field.getName());

        return serializer;
    }

    Serializer &operator<<(Serializer &serializer, const PrimitiveType& primtype)
    {
        serializer << Pointer<Schema>(primtype.getSchema())
                   << primtype.getTypeID()
                   << StrSeq(primtype.getName())
                   << primtype.getSize();

        return serializer;
    }

    Serializer &operator<<(Serializer &serializer, const CustomType& customType)
    {
        serializer << Pointer<Schema>(customType.getSchema())
                   << customType.getTypeID()
                   << StrSeq(customType.getName());

        const vector<Field> &fields = customType.getFields();
        serializer << (uint32) fields.size();
        for (auto &f: fields)
            serializer << f;

        return serializer;
    }

    Serializer& operator<<(Serializer& serializer, const Schema& schema)
    {
        serializer << schema.freeTypeID;
        serializer << schema.size();
        for (Type* t: schema.getTypes())
        {
            if (t->asPrimitiveType())
                serializer << Pointer<PrimitiveType>(reinterpret_cast<PrimitiveType*>(t));
            else
                serializer << Pointer<CustomType>(reinterpret_cast<CustomType*>(t));
        }
        return serializer;
    }

    //-----------------------------------
    // deserialize primitive types
    //-----------------------------------

    typedef SequenceWriter<std::string, char> StrSeqW;

    Deserializer &operator>>(Deserializer &deserializer, FieldType& fieldType)
    {
        deserializer >> fieldType.typeId >> fieldType.kind >> fieldType.repeat;
        return deserializer;
    }

    Deserializer &operator>>(Deserializer &deserializer, Field& field)
    {
        deserializer >> PointerRef<CustomType>(field.type)
                     >> field.index
                     >> field.fieldType
                     >> StrSeqW(field.name);

        // std::clog << field << std::endl;
        return deserializer;
    }

    Deserializer &operator>>(Deserializer &deserializer, PrimitiveType& primitiveType)
    {
        // schema
        deserializer >> PointerRef<Schema>(primitiveType.schema)
                     >> primitiveType.typeId
                     >> StrSeqW(primitiveType.name)
                     >> primitiveType.size;

        // std::clog << "[PrimitiveType Deserialization] " << primitiveType << std::endl;

        return deserializer;
    }

    Deserializer &operator>>(Deserializer &deserializer, CustomType& customType)
    {
        // schema
        deserializer >> PointerRef<Schema>(customType.schema) // write on ptr
                     >> customType.typeId                     // write on typeId
                     >> StrSeqW(customType.name);             // write on name

        // fields
        uint32 num_fields;
        deserializer >> num_fields;
        customType.fields.resize(num_fields);

        for (uint32 i=0;i<num_fields;i++)
            deserializer >> customType.fields[i];

        // std::clog << "[CustomType Deserialization] " << customType << std::endl;

        return deserializer;
    }

    Deserializer &operator>>(Deserializer &deserializer, Schema& schema)
    {
        // schema
        deserializer >> schema.freeTypeID;  // write on ptr

        //
        typedef RefSequenceWriter<std::vector<Type*>, Type*> RefSeqTypes;
        deserializer >> RefSeqTypes(schema.types);

//        uint32 length;
//        deserializer >> length;  // write on ptr
//        schema.types.resize(length, nullptr);
//        for (uint32 i=0;i<length;i++)
//        {
//            deserializer >> schema.types[i];
//        }

        // std::clog << "[Schema Deserialization] num types: " << schema.size() << std::endl;

        return deserializer;
    }

}
