#pragma once

#include <iostream>
#include <vector>

#include <serialization/BasicTypes.hpp>
#include <serialization/Header.hpp>
#include <serialization/SchemaSerialization.hpp>

#include <serialization/Serializable.hpp>

#include <serialization/Pointer.hpp>
#include <serialization/TaggedPointer.hpp>


namespace serialization
{



///////////////////////////////////////////////////////////////////////////////
//
// DECLARATION BLOCK
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Serializer
//-----------------------------------------------------------------------------

class Serializer
{
private:

    //--------------
    // Serializable
    //--------------

    struct PendingItem {
    public:
        virtual ~PendingItem() {};
        virtual void serialize(Serializer &s) = 0;
        // virtual rawptr getPointer() = 0;
    };

private:

    //---------------
    // SerializeItem
    //---------------

    template <typename T>
    struct TypedPendingItem: public PendingItem
    {
        TypedPendingItem(T &item, int typeId);
        void serialize(Serializer &serializer);
    private:
        T&  item;
        int typeId;
    };


public:
    Serializer(Schema &schema, string filename);

    ~Serializer();

    template <typename T>
    bool scheduleSerialization(Pointer<T> ptr);
    void run();

    // void push()
    std::ofstream &getStream();

    Schema& getSchema();

private:

    // the serializer needs a schema to be able to get
    // an ID for each type being serialized
    Schema       &schema; // could even tag as const

    string        filename;
    std::ofstream ostream;
    queue<PendingItem*> pending;

    // set of addresses already serialized or scheduled to be serialized
    unordered_set<RawPointer>  hitSet;


public:

    int countSerializedObjects;
};


//-----------------------------------------------------------------------------
// SequenceReader
//-----------------------------------------------------------------------------

template <typename Seq, typename Item>
struct SequenceReader
{
    explicit
    SequenceReader(const Seq& seq):
        seq(seq)
    {}

    size_t size() const
    { return seq.size(); }

    Item operator[](size_t index) const
    { return seq[index]; }

    const Seq &seq;
};

//----------------------------------------------
// different kinds of operator<< for Serializer
//----------------------------------------------

// primitive types
Serializer &operator<<(Serializer &serializer, char    value);
Serializer &operator<<(Serializer &serializer, int8    value);
Serializer &operator<<(Serializer &serializer, int16   value);
Serializer &operator<<(Serializer &serializer, int32   value);
Serializer &operator<<(Serializer &serializer, int64   value);
Serializer &operator<<(Serializer &serializer, uint8   value);
Serializer &operator<<(Serializer &serializer, uint16  value);
Serializer &operator<<(Serializer &serializer, uint32  value);
Serializer &operator<<(Serializer &serializer, uint64  value);
Serializer &operator<<(Serializer &serializer, float32 value);
Serializer &operator<<(Serializer &serializer, float64 value);
Serializer &operator<<(Serializer &serializer, RawPointer        value);
Serializer &operator<<(Serializer &serializer, RawPointerWithTag value);

// define a more general way to serialize sequences of any kind
template <typename Seq, typename Item>
Serializer &operator<<(Serializer &serializer, SequenceReader<Seq, Item> seq)
{
    uint32 size = seq.size();
    serializer << size;
    for (size_t i=0;i<size;i++)
        serializer << seq[i];
    return serializer;
}

// Block Header
Serializer& operator<<(Serializer &serializer, Header &header);





///////////////////////////////////////////////////////////////////////////////
//
// IMPLEMENTATION BLOCK
//
///////////////////////////////////////////////////////////////////////////////



//-----------------------------------------------------------------------------
// Serializer::TypedPendingItem
//-----------------------------------------------------------------------------

template <typename T>
Serializer::TypedPendingItem<T>::TypedPendingItem(T &item, int typeId):
    item(item), typeId(typeId)
{}

template <typename T>
void Serializer::TypedPendingItem<T>::serialize(Serializer &serializer)
{
    // add header
    Header header(typeId, (RawPointer) &item);

    // std::cout << "serializing header and item: " << (void*) (&item) << std::endl;

    serializer << header;
    serializer << item;
}


//-----------------------------------------------------------------------------
// Serializer
//-----------------------------------------------------------------------------

template<typename T>
Serializer& operator<<(Serializer &serializer, Pointer<T> ptr)
{
//    std::cout << "writing pointer..." << ptr.getRawPointer() << std::endl;
    serializer << ptr.getRawPointer();
    serializer.scheduleSerialization(ptr);
    return serializer;
}

template<typename T>
Serializer& operator<<(Serializer &serializer, TaggedPointer<T> ptr)
{
//    std::cout << "writing tagged pointer..." << ptr.getRawPointer() << std::endl;
    serializer << ptr.getRawPointerWithTag(); // write pointer with tag
    serializer.scheduleSerialization(ptr.getPointer());
    return serializer;
}

template <typename T>
bool Serializer::scheduleSerialization(Pointer<T> ptr)
{
    RawPointer rawptr = ptr.getRawPointer();

    if (!rawptr)
        return false;

    //
    // TODO: test if type exist
    //
    // if address doesn't exist on hitSet then schedule
    // object to be serialized
    //
    if (hitSet.count(rawptr) == 0)
    {

//        std::clog << "scheduleSerialization: " << typeid(T).name() << std::endl;
        // std::clog << "   ptr: " << "0x" << std::hex << (long) ptr << std::dec << std::endl;

        hitSet.insert(rawptr);

        TypeID typeId;
        if (schema.collectTypeID<T>(typeId))
        {
//            std::clog << "   Pushing pending object as "
//                      << schema.getTypeByID(typeId)->getName()
//                      << std::endl;

            pending.push(new TypedPendingItem<T>(ptr.getObject(), typeId));
        }
        else
        {
            schema.collectDynamicTypeID(typeId, ptr.getObject());

//            std::clog << "   Pushing pending object as "
//                      << schema.getTypeByID(typeId)->getName()
//                      << std::endl;

            pending.push(new TypedPendingItem<T>(ptr.getObject(), typeId));
        }
        return true;
    }
    else return false;
}

////
//// operator templates
////

////    // serialize pointers
////    template<class T>
////    Serializer& operator<<(Serializer &serializer, TaggedPointer<T> tagged_ptr)
////    {
////        serializer << ptr.getRawPointerWithTag();
////        serializer.scheduleSerialization(tagged_ptr.getPointer());
////        return serializer;
////    }


//template<class T>
//Serializer& operator<<(Serializer &serializer, TaggedPointer<T> tptr)
//{
//    // just get the raw bytes of the pointer as a RawPointer (note
//    // that it includes the tag and it is not a proper pointer)
//    TaggedPointer rawptr(tptr.ptr);
//    serializer << rawptr; // write pointer with tag et

//    // schedule serialization of the real pointer
//    serializer.scheduleSerialization(tptr.);
//    return serializer;
//}


//// serialize vector of pointers (REPEATED field)
//template<class T>
//Serializer& operator<<(Serializer &serializer, const std::vector<T> &vec)
//{
//    serializer << (uint32) vec.size();
//    for (auto obj: vec)
//        serializer << obj;
//    return serializer;
//}

//    // serialize vector of objects of type T
//    template<class T>
//    Serializer& operator<<(Serializer &serializer, const std::vector<T> &vec)
//    {
//        serializer << (uint32) vec.size();
//        for (auto obj: vec)
//            serializer << obj;
//        return serializer;
//    }

}
