#pragma once

#include <serialization/BasicTypes.hpp>
#include <serialization/Header.hpp>
#include <serialization/Type.hpp>
#include <serialization/Schema.hpp>
#include <serialization/SchemaSerialization.hpp>
#include <serialization/TaggedPointer.hpp>

#include <unordered_set>

#include <functional>

#include <ostream>
#include <vector>

namespace serialization {

///////////////////////////////////////////////////////////////////////////////
//
// DECLARATION BLOCK
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// AbstractClassFactory
//-----------------------------------------------------------------------------

class AbstractObjectFactory // base interface
{
public:
    virtual ~AbstractObjectFactory(){}
    virtual RawPointer createAndDeserialize(Deserializer &deserializer) = 0;
};

//-----------------------------------------------------------------------------
// ClassFactory
//-----------------------------------------------------------------------------

template< typename T >
class ObjectFactory: public AbstractObjectFactory
{
public:
    RawPointer createAndDeserialize(Deserializer &deserializer);
};

//-----------------------------------------------------------------------------
// ClassManager
//-----------------------------------------------------------------------------

class ClassManager
{
public:

    void add( TypeID typeId, AbstractObjectFactory* factory )
    {
        factories[typeId] = factory;
    }

    RawPointer createAndDeserialize( TypeID typeId, Deserializer &deserializer)
    {
        return factories[typeId]->createAndDeserialize(deserializer);
    }

private:

    unordered_map<TypeID, AbstractObjectFactory*> factories;

};


//-----------------------------------------------------------------------------
// Deserializer
//-----------------------------------------------------------------------------

struct PointerToRetarget
{
    typedef uint16 Tag;
    static const uint64 bit47 = (1UL << 47);

    struct PaddedTag{
        Tag pad1;
        Tag pad2;
        Tag pad3;
        Tag tag;
    };

    union Data {
        RawPointer ptr;
        PaddedTag  padded_tag;
    };

    // ptr might be tagged in this case (indicated by the flag)
    PointerToRetarget(RawPointer& ptr, bool tagged):
        ptr(ptr),
        tagged(tagged)
    {}

    RawPointer getRawPointer()
    {
        if (!tagged) return ptr;
        else
        {
            Data x;
            x.ptr = ptr;
            if (reinterpret_cast<uint64>(x.ptr) & bit47)
                x.padded_tag.tag = 0xFFFF;
            else
                x.padded_tag.tag = 0;
            return x.ptr;
        }
    }

    void retarget(RawPointer new_target)
    {
        if (!tagged)
            ptr = new_target;
        else
        {
            Data d;
            d.ptr = ptr;
            Tag tag = d.padded_tag.tag;
            d.ptr = new_target;
            d.padded_tag.tag = tag;
            ptr = d.ptr;
        }
    }

public: // attributes

    RawPointer& ptr;
    bool        tagged;

};


//-----------------------------------------------------------------------------
// Deserializer
//-----------------------------------------------------------------------------

class Deserializer {
public:
    typedef uint64_t                       Progress;
    typedef std::function<void (Progress)> ProgressCallback;

public:


    Deserializer(string filename);

    ~Deserializer();

    std::ifstream &getStream();

    void addOutdatedPointer(PointerToRetarget p_retarget);

    void addOldNewMapping(RawPointer oldPtr, RawPointer newPtr);

    void addObjectFactory(TypeID typeId, AbstractObjectFactory* classFactory);

    vector<RawPointer> &getDeserializedObjectsByType(TypeID typeId) const;

    void run(TypeID a, TypeID b);

    void log(Schema &schema, std::ostream &o);

    template <typename T>
    void collectDeserializedObjectsByType(TypeID typeId, vector<T*>& output) const;

    template <typename T>
    T* collectFirstDeserializedObjectByType(TypeID typeId) const;

    void addRetrieveType(TypeID type_id);

    void registerProgressCallback(ProgressCallback *pc);

private:

    void addDeserializedObjectByType(TypeID typeId, RawPointer ptr);
    void logNextBytesAs(Type *type, int depth, Schema &schema, std::ostream &os, string contextName);
    void logNextBytesAs(CustomType *type, int depth, Schema &schema, std::ostream &os, string contextName);
    void logNextBytesAs(PrimitiveType *type, int depth, Schema &schema, std::ostream &os, string contextName);

private:

    string        filename;
    std::ifstream istream;

    unordered_map<TypeID, vector<RawPointer>* > deserializedObjects;

    unordered_map<RawPointer, RawPointer>                  mapOldNew;
    unordered_map<RawPointer, vector<PointerToRetarget> >  outdatedPointers;

    std::unordered_set<TypeID>                  retrieve_types; // objects of this type can be retrieved after deserialization

    ClassManager  classManager;

    Progress                       num_deserialized_objects;

    std::vector<ProgressCallback*> progress_callbacks;
};


//-----------------------------------------------------------------------------
// SequenceWriter
//-----------------------------------------------------------------------------

template <typename Seq, typename Item>
struct SequenceWriter
{
    SequenceWriter(Seq& seq):
        seq(seq)
    {}

    void resize(size_t size)
    { seq.resize(size); }

    Item& operator[](size_t index)
    { return seq[index]; }

    Seq &seq;
};

#if 1
template <typename Seq, typename Item>
struct RefSequenceWriter
{
    RefSequenceWriter(Seq& seq):
        seq(seq)
    {}

    void resize(size_t size)
    { seq.resize(size); }

    Item &operator[](size_t index)
    { return seq[index]; }

    Seq &seq;
};
#endif

//-----------------------------------------------------------------------------
// deserialize primitive types
//-----------------------------------------------------------------------------

Deserializer &operator>>(Deserializer &deserializer, char    &value);
Deserializer &operator>>(Deserializer &deserializer, int8    &value);
Deserializer &operator>>(Deserializer &deserializer, int16   &value);
Deserializer &operator>>(Deserializer &deserializer, int32   &value);
Deserializer &operator>>(Deserializer &deserializer, int64   &value);
Deserializer &operator>>(Deserializer &deserializer, uint8   &value);
Deserializer &operator>>(Deserializer &deserializer, uint16  &value);
Deserializer &operator>>(Deserializer &deserializer, uint32  &value);
Deserializer &operator>>(Deserializer &deserializer, uint64  &value);
Deserializer &operator>>(Deserializer &deserializer, float32 &value);
Deserializer &operator>>(Deserializer &deserializer, float64 &value);
Deserializer &operator>>(Deserializer &deserializer, void*   &value);
// |-> Deserializer &operator>>(Deserializer &deserializer, RawPointer        &value);
// |-> Deserializer &operator>>(Deserializer &deserializer, RawPointerWithTag &value);

// references and pointers
Deserializer &operator>>(Deserializer &deserializer, RawPointerRef pref);

template <typename T>
Deserializer& operator>>(Deserializer &deserializer, PointerRef<T> pref);

template <typename T>
Deserializer& operator>>(Deserializer &deserializer, TaggedPointerRef<T> pref);

// references and pointers
template <typename Seq, typename Item>
Deserializer &operator>>(Deserializer &deserializer, SequenceWriter<Seq,Item> seq);

// references and pointers
template <typename Seq, typename Item>
Deserializer &operator>>(Deserializer &deserializer, RefSequenceWriter<Seq,Item> seq);




///////////////////////////////////////////////////////////////////////////////
//
// IMPLEMENTATION BLOCK
//
///////////////////////////////////////////////////////////////////////////////


//
// ObjectFactory: method templates
//

template< typename T >
RawPointer ObjectFactory<T>::createAndDeserialize(Deserializer &deserializer)
{
    T& obj = *(new T());
    deserializer >> obj;
    return RawPointer(&obj);
}

//
// Deserializer: method templates
//

template <typename T>
void Deserializer::collectDeserializedObjectsByType(TypeID typeId, vector<T*>& output) const
{
    auto it = deserializedObjects.find(typeId);
    if (it == deserializedObjects.end())
        return;
    for (auto obj: *it->second)
    {
        output.push_back(reinterpret_cast<T*>(obj));
    }
}

template <typename T>
T* Deserializer::collectFirstDeserializedObjectByType(TypeID typeId) const
{
    auto it = deserializedObjects.find(typeId);
    if (it == deserializedObjects.end())
        return nullptr;
    for (auto obj: *it->second)
        return reinterpret_cast<T*>(obj);
    return nullptr;
}

template <typename T>
void registerObjectFactory(Deserializer& deserializer, Schema& schema)
{
    deserializer.addObjectFactory(
                schema.getTypeByName(T::TYPENAME)->getTypeID(),
                new ObjectFactory<T>());
}





//------------------------------------------------------------------------------
// operator<< templates
//------------------------------------------------------------------------------

template <typename T>
Deserializer& operator>>(Deserializer &deserializer, PointerRef<T> pref)
{
    deserializer >> pref.getRawPointer(); // this is a reference to a raw pointer
    deserializer.addOutdatedPointer(PointerToRetarget(pref.getRawPointer(), false));
    return deserializer;
}

template <typename T>
Deserializer& operator>>(Deserializer &deserializer, TaggedPointerRef<T> pref)
{
    deserializer >> pref.getRawPointerWithTag(); // this is a reference to a raw pointer with ref memory
    deserializer.addOutdatedPointer(PointerToRetarget(pref.getRawPointerWithTag(), true));
    return deserializer;
}

// references and pointers
template <typename Seq, typename Item>
Deserializer &operator>>(Deserializer &deserializer, SequenceWriter<Seq,Item> seq)
{
    uint32 size;
    deserializer >> size;
    seq.resize(size);
//    std::cout << "Size of seq seq: " << seq.seq.size() << std::endl;
    for (uint32 i=0;i<size;i++)
    {
        deserializer >> seq[i];
//        Item item;
//        deserializer >> item;
//        seq[i] = item;
    }
    return deserializer;
}

// references and pointers
template <typename Seq, typename Item>
Deserializer &operator>>(Deserializer &deserializer, RefSequenceWriter<Seq,Item> seq)
{
    typedef typename std::remove_pointer<Item>::type DerefItem;

    uint32 size;
    deserializer >> size;
    seq.resize(size);
    for (uint32 i=0;i<size;i++)
        deserializer >> PointerRef<DerefItem>(seq[i]);
    return deserializer;
}

}
