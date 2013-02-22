#include <iostream>

#include <serialization/BasicTypes.hpp>
#include <serialization/Serializer.hpp>
#include <serialization/Schema.hpp>
#include <serialization/Type.hpp>
#include <serialization/SchemaSerialization.hpp>
#include <serialization/CoreSchema.hpp>
#include <serialization/SchemaLog.hpp>


using namespace std;
using namespace serialization;

int main()
{

    try
    {
        Serializer serializer("/tmp/schema.bin");

        // add types of blocks that will be pushed into
        // the serializer (schema)
        Schema &schema = serializer.getSchema();


        // add to pending list
        // uint32 _uint32 = 0x12345678;
        // serializer.scheduleSerialization(&_uint32);

        serializer.scheduleSerialization(&schema);


        serializer.run();

        std::clog << "=====> number of serialized blocks: "
                  << serializer.countSerializedObjects
                  << std::endl;

    }
    catch (std::exception &e)
    {
        std::clog << e.what() << std::endl;
    }

#if 1
    try
    {
        Deserializer deserializer("/tmp/schema.bin");

        // deserialize file
        deserializer.run(0,CoreSchema::FIRST_USER_TYPEID+1000);

//        std::clog << "=====> number of deserialized blocks: "
//                  << deserializer.deserializedObjects.size()
//                  << std::endl;

        //
        vector<RawPointer>& schemas = deserializer.getDeserializedObjectsByType(CoreSchema::ID_Schema);
        if (schemas.size() > 0)
        {
            Schema &schema = *reinterpret_cast<Schema*>(schemas[0].asVoidPtr());
            clog << "Retrieved Schema:" << endl;
            clog << schema << endl;
        }

    }
    catch (std::exception &e)
    {
        std::clog << e.what() << std::endl;
    }
#endif

}
