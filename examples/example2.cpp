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

struct Graph;
struct Node;

//------
// Node
//------

struct Node
{
    Node();
    Node(Graph *graph, string name, float  fp32, double fp64);

    void addNeighbor(Node *node);

public: // data
    Graph*        graph;

    std::string   name;
    float         fp32;
    double        fp64;

    std::vector<Node*> neighbors;
};

Node::Node():
    graph(nullptr), name(""), fp32(0.0), fp64(0.0)
{}

Node::Node(Graph *graph, string name, float  fp32, double fp64):
    graph(graph), name(name), fp32(fp32), fp64(fp64)
{}

void Node::addNeighbor(Node *node)
{
    neighbors.push_back(node);
}



//-------
// Graph
//-------

struct Graph
{
    virtual ~Graph();
    Node& addNode(string name, float fp32, float fp64);

    std::vector<Node*> nodes;
};

Graph::~Graph()
{
    for (auto a: nodes)
        delete a;
}

Node& Graph::addNode(string name, float fp32, float fp64)
{
    Node *node = new Node(this, name, fp32, fp64);
    nodes.push_back(node);
    return *node;
}





//---------------
// Connect Nodes
//---------------

Node &operator>>(Node &a, Node &b)
{
    a.addNeighbor(&b);
    return b;
}

Node &operator<<(Node &a, Node &b)
{
    b.addNeighbor(&a);
    return b;
}




//-----
// Log
//-----

ostream &operator<<(ostream &o, Node &node)
{
    char st[1000];
    sprintf(st,"addr: %-10p  name: %-4s  fp32: %5.1f  fp64: %5.1f  neighbors:",
            &node, node.name.c_str(), node.fp32, node.fp64);
    o << st;
    for (auto neighbor: node.neighbors)
        o << " " << neighbor->name;
    o << endl;
    return o;
}

ostream &operator<<(ostream &o, Graph &graph)
{
    for (auto node: graph.nodes)
        o << *node;
    return o;
}

using namespace serialization;



//-------------
// Description
//-------------

Schema &operator<<(Schema &schema, const Graph &graph)
{
    CustomType *graphType = retrieveOrCreateCustomType<Graph>(schema,"Graph");
    assert(graphType->getNumFields() == 0);
    CustomType *nodeType = retrieveOrCreateCustomType<Node>(schema,"Node");
    graphType->addField(
                FieldType(nodeType->getTypeID(), FieldType::REPEATED_POINTER),
                "nodes");
    return schema;
}

Schema &operator<<(Schema &schema, const Node &node)
{
    CustomType *graphType = retrieveOrCreateCustomType<Graph>(schema,"Graph");
    CustomType *nodeType  = retrieveOrCreateCustomType<Node>(schema,"Node");
    assert(nodeType->getNumFields() == 0);

    nodeType->addField(FieldType(graphType->getTypeID(), FieldType::POINTER),"graph");
    nodeType->addField(FieldType(CoreSchema::ID_char,  FieldType::REPEATED),"name");
    nodeType->addField(FieldType(CoreSchema::ID_float32, FieldType::NORMAL),"fp32");
    nodeType->addField(FieldType(CoreSchema::ID_float64, FieldType::NORMAL),"fp64");

    nodeType->addField(FieldType(nodeType->getTypeID(),  FieldType::REPEATED_POINTER),"neighbors");

    return schema;
}





//-----------
// Serialize
//-----------

// must be initialized
static TypeID ID_Node  = 0;
static TypeID ID_Graph = 0;


Serializer &operator<<(Serializer &serializer, const Graph &graph)
{
    serializer << graph.nodes;
    return serializer;
}

Serializer &operator<<(Serializer &serializer, const Node &node)
{
    serializer << node.graph
               << node.name
               << node.fp32
               << node.fp64
               << node.neighbors;
    return serializer;
}



//-----------
// Deserialize
//-----------

Deserializer &operator>>(Deserializer &deserializer, Graph &graph)
{
    deserializer >> graph.nodes;
    return deserializer;
}

Deserializer &operator>>(Deserializer &deserializer, Node &node)
{
    deserializer >> node.graph
                 >> node.name
                 >> node.fp32
                 >> node.fp64
                 >> node.neighbors;
    return deserializer;
}






//-------
// Main
//-------

int main()
{
    Graph graph;
    Node &A = graph.addNode("A",1.1,1.1);
    Node &B = graph.addNode("B",2.2,2.2);
    Node &C = graph.addNode("C",3.3,3.3);
    Node &D = graph.addNode("D",4.4,4.4);
    Node &E = graph.addNode("E",5.5,5.5);

    // add connections
    A >> B >> A >> E >> C >> B;
    A >> D >> B;
    D >> E;

    //
    clog << graph;


#if 1
    try {

        Serializer serializer("graph.bin");

        Schema &schema = serializer.getSchema();
        schema << Graph();
        schema << Node();

//        ID_Graph = schema.getTypeID<Graph>();
//        ID_Node  = schema.getTypeID<Node>();

        // serialize schema
        serializer.scheduleSerialization(&schema);
        serializer.run();

        // serialize user content
        serializer.scheduleSerialization(&graph);
        serializer.run();

    }
    catch (std::exception &e)
    {
        std::clog << e.what() << std::endl;
    }
#endif


#if 1
    try {

        Deserializer deserializer("graph.bin");

        //
        // deserialize schema
        //
        deserializer.run(0,CoreSchema::FIRST_USER_TYPEID-1);

        // get schema
        vector<RawPointer>& schemas = deserializer.getDeserializedObjectsByType(CoreSchema::ID_Schema);
        assert (schemas.size() > 0);
        Schema &schema = *reinterpret_cast<Schema*>(schemas[0].asVoidPtr());
        clog << "Retrieved Schema:" << endl;
        clog << schema << endl;

        // for every type in the schema we have to add an object creator
        ID_Graph = schema.getTypeByName("Graph")->getTypeID();
        ID_Node  = schema.getTypeByName("Node")->getTypeID();
        deserializer.addObjectFactory(ID_Graph, new ObjectFactory<Graph>());
        deserializer.addObjectFactory(ID_Node,  new ObjectFactory<Node>());


        //
        // deserialize user content (a graph)
        //
        deserializer.run(CoreSchema::FIRST_USER_TYPEID,CoreSchema::FIRST_USER_TYPEID+1000);
        vector<RawPointer>& graphs = deserializer.getDeserializedObjectsByType(ID_Graph);
        if (graphs.size() > 0)
        {
            Graph &graph = *reinterpret_cast<Graph*>(graphs[0].asVoidPtr());
            clog << "Retrieved Graph:" << endl;
            clog << graph << endl;
        }

    }
    catch (std::exception &e)
    {
        std::clog << e.what() << std::endl;
    }
#endif




























    //
//    try
//    {
//        Serializer serializer("/tmp/schema.bin");

//        // add types of blocks that will be pushed into
//        // the serializer (schema)
//        Schema &schema = serializer.getSchema();

//        // push schema
//        pushSerializableItemIfNecessary<Schema>( serializer,
//                                                &schema,
//                                                 schema.getTypeByName("Schema")->getTypeID());

//        serializer.process();

//        std::clog << "=====> number of serialized blocks: "
//                  << serializer.countSerializedObjects
//                  << std::endl;

//    }
//    catch (std::exception &e)
//    {
//        std::clog << e.what() << std::endl;
//    }

//    try
//    {
//        Deserializer deserializer("/tmp/schema.bin");

//        // register class factories for the types of the schema
//        CoreSchema::initCoreDeserializer(deserializer);

//        // deserialize file
//        deserializer.run();

//        std::clog << "=====> number of deserialized blocks: "
//                  << deserializer.deserializedObjects.size()
//                  << std::endl;

//        //
//        Schema* schema = reinterpret_cast<Schema*>(deserializer.deserializedObjects[0]);

//        std::clog << *schema;

//    }
//    catch (std::exception &e)
//    {
//        std::clog << e.what() << std::endl;
//    }

}
