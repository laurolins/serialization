#include <serialization/BasicTypes.hpp>
#include <serialization/CoreSchema.hpp>
#include <serialization/Serializer.hpp>
#include <serialization/Deserializer.hpp>
#include <serialization/SchemaLog.hpp>
#include <serialization/TaggedPointer.hpp>

#if 1

#include <iostream>
#include <iomanip>

using namespace serialization;

// typedef serialization::Tag Tag;

struct A {

    A()
    {}

    A(Tag tag)
    { tag_ptr.setTag(tag); }

    serialization::TaggedPointer<A> tag_ptr;

    void setTag(Tag tag)
    { tag_ptr.setTag(tag); }

    Tag getTag() const
    { return tag_ptr.getTag(); }

};

namespace serialization {

Schema &operator<<(Schema &s, A &)
{
    CustomType *type = retrieveOrCreateCustomType<A>(s,"A");
    assert(type->getNumFields() == 0);

    type->addField(FieldType(CoreSchema::ID_tagged_pointer, FieldType::TAGGED_POINTER, FieldType::NO_REPEAT),"tag_ptr");
    return s;
}

Serializer& operator<<(Serializer& s, A &a)
{
    s << a.tag_ptr;
    return s;
}

Deserializer& operator>>(Deserializer& d, A &a)
{
    d >> TaggedPointerRef<A>(a.tag_ptr);
    return d;
}

}




std::ostream &operator<<(std::ostream& os, const A& a)
{
    os << "A[ tag: "
       << std::setw(4)  << a.getTag()
       << ", ptr: ";

    if (!a.tag_ptr.getRawPointer())
        os << std::setw(5) << "null";
    else
        os << std::setw(5) << a.tag_ptr.getPointer().getObject().getTag();
    os << " ]";
    return os;
}

int main()
{
    A a1(1);
    A a2(2);
    A a3(3);

    a1.tag_ptr.setPointer(&a2);
    a2.tag_ptr.setPointer(&a1);
    a3.tag_ptr.setPointer(&a2);

    std::cout << a1 << std::endl;
    std::cout << a2 << std::endl;
    std::cout << a3 << std::endl;


    { // serialization

        // prepare schema
        Schema schema;

        CoreSchema::initCoreSchema(schema);
        schema << a1; // register type A into schema

        // serializer needs one schema only to map type into IDs
        Serializer serializer(schema, "/tmp/db.bin");

        // always serialize the whole schema first
        serializer.scheduleSerialization<Schema>(Pointer<Schema>(&schema));
        serializer.run();

        // then serialize the user data
        serializer.scheduleSerialization<A>(Pointer<A>(&a1));
        serializer.scheduleSerialization<A>(Pointer<A>(&a2));
        serializer.scheduleSerialization<A>(Pointer<A>(&a3));
        serializer.run();
    }

    { // deserialization

        Deserializer deserializer("/tmp/db.bin");
        deserializer.run(0, CoreSchema::LAST_BUILTIN_TYPEID);

        Schema *schema = deserializer.collectFirstDeserializedObjectByType<Schema>(CoreSchema::ID_Schema);
        assert(schema);

        std::cout << *schema << std::endl;

        Type* typeA = schema->getTypeByName("A");
        assert(typeA);

        // register
        deserializer.addObjectFactory(typeA->getTypeID(), new ObjectFactory<A>());
        deserializer.addRetrieveType(typeA->getTypeID());



        //
        deserializer.run(CoreSchema::FIRST_USER_TYPEID,CoreSchema::MAXIMUM_USER_TYPEID);



        //
        std::vector<A*> result;
        deserializer.collectDeserializedObjectsByType(typeA->getTypeID(), result);

        // assert(result.size() == 3);

        for (auto a: result)
        {
            std::cout << *a << std::endl;
        }
    }

}
#else

using namespace serialization;


struct B
{
    int x;
};

namespace serialization {

Schema &operator<<(Schema &s, B& b)
{
    CustomType *type = retrieveOrCreateCustomType<B>(s,"B");
    assert(type->getNumFields() == 0);
    type->addField(FieldType(CoreSchema::ID_tagged_pointer, FieldType::POINTER),"x");
    return s;
}

}


int main()
{
    {
        Serializer serializer("/tmp/db.bin");

        Schema &schema = serializer.getSchema();
        B b;
        schema << b;
        serializer.scheduleSerialization<Schema>(Pointer<Schema>(&schema));

        serializer.run();
    }

    {
        Deserializer deserializer("/tmp/db.bin");
        deserializer.run(0, 100000);
        Schema *schema = deserializer.collectFirstDeserializedObjectByType<Schema>(CoreSchema::ID_Schema);
        assert(schema);
        std::cout << *schema << std::endl;
    }

    return 0;
}
#endif
