#include <iostream>

#include <stdio.h>

#include <cassert>

#include <serialization/BasicTypes.hpp>
#include <serialization/CoreSchema.hpp>
#include <serialization/Deserializer.hpp>
#include <serialization/FieldType.hpp>
#include <serialization/Schema.hpp>
#include <serialization/Schema.hpp>
#include <serialization/SchemaLog.hpp>
#include <serialization/SchemaSerialization.hpp>
#include <serialization/Serializer.hpp>
#include <serialization/Type.hpp>

using namespace std;
using namespace serialization;

//-------
// Main
//-------

int main(int argc, char** argv)
{

    if (argc <= 1)
    {
        cout << "Usage: " << argv[0] << "<binary_file>" << endl;
        return 0;
    }

    string filename(argv[1]); // = "/Users/lauro/projects/serialize/examples/graph.bin";

    // the way to describe custom types should be invariant
    Deserializer deserializer(filename);
    CoreSchema::initCoreDeserializer(deserializer);

    // deserialize the schema
    deserializer.run(0,CoreSchema::FIRST_USER_TYPEID-1);

    // get schema
    vector<Schema*> schemas;
    deserializer.collectDeserializedObjectsByType(CoreSchema::ID_Schema, schemas);

    assert(schemas.size() > 0);

    Schema &schema = *schemas[0];

    clog << "Retrieved Schema:" << endl;
    clog << schema << endl;

    deserializer.log(schema, std::cout);


//    //
//    clog << drawings;


//#if 1
//    try {

//        Serializer serializer("drawings.bin");

//        Schema &schema = serializer.getSchema();
//        registerTypeToSchema<DrawingList>(schema);
//        registerTypeToSchema<Rectangle>(schema);
//        registerTypeToSchema<Circle>(schema);

////        ID_Graph = schema.getTypeID<Graph>();
////        ID_Node  = schema.getTypeID<Node>();

//        // serialize schema
//        serializer.scheduleSerialization(&schema);
//        serializer.run();

//        // serialize user content
//        serializer.scheduleSerialization(&drawings);
//        serializer.run();

//    }
//    catch (std::exception &e)
//    {
//        std::clog << e.what() << std::endl;
//    }
//#endif


//#if 1
//    try {

//        Deserializer deserializer("drawings.bin");

//        //
//        // deserialize schema
//        //
//        deserializer.run(0,CoreSchema::FIRST_USER_TYPEID-1);

//        // get schema
//        vector<RawPointer>& schemas = deserializer.getDeserializedObjectsByType(CoreSchema::ID_Schema);
//        assert (schemas.size() > 0);
//        Schema &schema = *reinterpret_cast<Schema*>(schemas[0].asVoidPtr());
//        clog << "Retrieved Schema:" << endl;
//        clog << schema << endl;

//        // for every type in the schema we have to add an object creator
//        TypeID ID_Circle       = schema.getTypeByName("Circle")->getTypeID();
//        TypeID ID_Rectangle    = schema.getTypeByName("Rectangle")->getTypeID();
//        TypeID ID_DrawingList  = schema.getTypeByName("DrawingList")->getTypeID();

//        // deserializer
//        deserializer.addClassFactory(ID_Circle,      new ClassFactory<Circle>());
//        deserializer.addClassFactory(ID_Rectangle,   new ClassFactory<Rectangle>());
//        deserializer.addClassFactory(ID_DrawingList, new ClassFactory<DrawingList>());

//        //
//        // deserialize user content (a graph)
//        //
//        deserializer.run(CoreSchema::FIRST_USER_TYPEID,CoreSchema::FIRST_USER_TYPEID+1000);
//        vector<RawPointer>& lists = deserializer.getDeserializedObjectsByType(ID_DrawingList);
//        if (lists.size() > 0)
//        {
//            DrawingList &drawingList = *reinterpret_cast<DrawingList*>(lists[0].asVoidPtr());
//            clog << "Retrieved Drawing List:" << endl;
//            clog << drawingList << endl;
//        }

//    }
//    catch (std::exception &e)
//    {
//        std::clog << e.what() << std::endl;
//    }
//#endif

}
