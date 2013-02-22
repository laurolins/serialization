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
#include <serialization/Serializable.hpp>
#include <serialization/Type.hpp>

using namespace std;

struct Drawing;
struct Rectangle;
struct Circle;


//---------
// Drawing
//---------

struct Drawing: public serialization::Serializable
{
    Drawing();
    Drawing(double x, double y);

    virtual void log(ostream& os);

    double x, y;

    virtual void save(serialization::Serializer& serializer) const;
    virtual void load(serialization::Deserializer& deserializer);

};

Drawing::Drawing():
    x(0), y(0)
{}

Drawing::Drawing(double x, double y):
    x(x), y(y)
{}

void Drawing::log(ostream &o)
{
    char st[1000];
    sprintf(st,"[Drawing]   addr: %-10p  x: %5.1f  y: %5.1f",
            this, x, y);
    o << st << endl;
}

void Drawing::save(serialization::Serializer& serializer) const
{
    serializer << x << y;
}

void Drawing::load(serialization::Deserializer& deserializer)
{
    deserializer >> x >> y;
}

//-----------
// Rectangle
//-----------

struct Rectangle: public Drawing
{
    Rectangle();
    Rectangle(double x, double y, double width, double height);

    void log(ostream& os);

    double width;
    double height;


    virtual void save(serialization::Serializer& serializer) const;
    virtual void load(serialization::Deserializer& deserializer);
};

Rectangle::Rectangle():
    Drawing(0,0), width(1), height(1)
{}

Rectangle::Rectangle(double x, double y, double width, double height):
    Drawing(x,y),
    width(width),
    height(height)
{}

void Rectangle::log(ostream &o)
{
    char st[1000];
    sprintf(st,"[Rectangle] addr: %-10p  x: %5.1f  y: %5.1f  w: %5.1f  h: %5.1f",
            this, x, y, width, height);
    o << st << endl;
}

void Rectangle::save(serialization::Serializer& serializer) const
{
    serializer << x << y << width << height;
}

void Rectangle::load(serialization::Deserializer& deserializer)
{
    deserializer >> x >> y >> width >> height;
}


//--------
// Circle
//--------

struct Circle: public Drawing
{
    Circle();
    Circle(double x, double y, double radius);

    void log(ostream& os);

    double radius;

    virtual void save(serialization::Serializer& serializer) const;
    virtual void load(serialization::Deserializer& deserializer);

};

Circle::Circle():
    Drawing(0,0), radius(1)
{}

Circle::Circle(double x, double y, double radius):
    Drawing(x,y),
    radius(radius)
{}

void Circle::log(ostream &o)
{
    char st[1000];
    sprintf(st,"[Circle]    addr: %-10p  x: %5.1f  y: %5.1f  radius: %5.1f",
            this, x, y, radius);
    o << st << endl;
}

void Circle::save(serialization::Serializer& serializer) const
{
    serializer << x << y << radius;
}

void Circle::load(serialization::Deserializer& deserializer)
{
    deserializer >> x >> y >> radius;
}


//-----
// Log
//-----

ostream &operator<<(ostream &o, Drawing &drawing)
{
    drawing.log(o);
    return o;
}

//-------------
// DrawingList
//-------------

struct DrawingList: public serialization::Serializable
{
    void log(ostream& os);

    void add(Drawing *drawing);

    std::vector<Drawing*> drawings;

    virtual void save(serialization::Serializer& serializer) const;
    virtual void load(serialization::Deserializer& deserializer);
};

void DrawingList::add(Drawing *drawing)
{
    drawings.push_back(drawing);
}

ostream &operator<<(ostream &o, DrawingList &drawingList)
{
    o << "[DrawingList]" << endl;
    for (auto a: drawingList.drawings)
        o << "   " << *a;
    return o;
}

void DrawingList::save(serialization::Serializer& serializer) const
{
    serializer << drawings;
}

void DrawingList::load(serialization::Deserializer& deserializer)
{
    deserializer >> drawings;
}



using namespace serialization;



//-------------
// Description
//-------------

template<typename T>
void registerTypeToSchema(serialization::Schema&)
{}

template<>
void registerTypeToSchema<Rectangle>(serialization::Schema &schema)
{
    CustomType *type = retrieveOrCreateCustomType<Rectangle>(schema,"Rectangle");
    assert(type->getNumFields() == 0);

    type->addField(FieldType(CoreSchema::ID_float64, FieldType::NORMAL),"x");
    type->addField(FieldType(CoreSchema::ID_float64, FieldType::NORMAL),"y");
    type->addField(FieldType(CoreSchema::ID_float64, FieldType::NORMAL),"w");
    type->addField(FieldType(CoreSchema::ID_float64, FieldType::NORMAL),"h");
}

template<>
void registerTypeToSchema<Circle>(serialization::Schema &schema)
{
    CustomType *type = retrieveOrCreateCustomType<Circle>(schema,"Circle");
    assert(type->getNumFields() == 0);

    type->addField(FieldType(CoreSchema::ID_float64, FieldType::NORMAL),"x");
    type->addField(FieldType(CoreSchema::ID_float64, FieldType::NORMAL),"y");
    type->addField(FieldType(CoreSchema::ID_float64, FieldType::NORMAL),"radius");
}

template<>
void registerTypeToSchema<DrawingList>(serialization::Schema &schema)
{
    CustomType *type = retrieveOrCreateCustomType<DrawingList>(schema,"DrawingList");
    assert(type->getNumFields() == 0);

    type->addField(FieldType(CoreSchema::ID_pointer, FieldType::REPEATED_POINTER),"drawings");
}

//-------
// Main
//-------

int main()
{
    DrawingList drawings;
    drawings.add(new Rectangle(0,0,5,4));
    drawings.add(new Circle(1,1,3));

    //
    // clog << drawings;


#if 1
    try {

        Serializer serializer("drawings.bin");

        Schema &schema = serializer.getSchema();
        registerTypeToSchema<DrawingList>(schema);
        registerTypeToSchema<Rectangle>(schema);
        registerTypeToSchema<Circle>(schema);

//        ID_Graph = schema.getTypeID<Graph>();
//        ID_Node  = schema.getTypeID<Node>();

        // serialize schema
        serializer.scheduleSerialization(&schema);
        serializer.run();

        // serialize user content
        serializer.scheduleSerialization(&drawings);
        serializer.run();

    }
    catch (std::exception &e)
    {
        std::clog << e.what() << std::endl;
    }
#endif


#if 1
    try {

        Deserializer deserializer("drawings.bin");

        //
        // deserialize schema
        //
        deserializer.run(0,CoreSchema::FIRST_USER_TYPEID-1);

        // get schema
        vector<serialization::Schema*> schemas;
        deserializer.collectDeserializedObjectsByType(serialization::CoreSchema::ID_Schema, schemas);
        serialization::Schema &schema = *schemas[0];

        // log
        clog << "Retrieved Schema:" << endl;
        clog << schema << endl;

        // for every type in the schema we have to add an object creator
        TypeID ID_Circle       = schema.getTypeByName("Circle")->getTypeID();
        TypeID ID_Rectangle    = schema.getTypeByName("Rectangle")->getTypeID();
        TypeID ID_DrawingList  = schema.getTypeByName("DrawingList")->getTypeID();

        // deserializer
        deserializer.addObjectFactory(ID_Circle,      new ObjectFactory<Circle>());
        deserializer.addObjectFactory(ID_Rectangle,   new ObjectFactory<Rectangle>());
        deserializer.addObjectFactory(ID_DrawingList, new ObjectFactory<DrawingList>());

        //
        // deserialize user content (a graph)
        //

        vector<DrawingList*> drawingLists;
        deserializer.collectDeserializedObjectsByType(ID_DrawingList, drawingLists);
        DrawingList &drawingList = *drawingLists[0];

        clog << "Retrieved Drawing List:" << endl;
        clog << drawingList << endl;


    }
    catch (std::exception &e)
    {
        std::clog << e.what() << std::endl;
    }
#endif

}
