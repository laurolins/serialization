#pragma once

#include <assert.h>

#include <serialization/BasicTypes.hpp>
#include <serialization/CustomType.hpp>
#include <serialization/PrimitiveType.hpp>

namespace serialization {

//------------
// SchemaBase
//------------

class Schema
{
public:
    Schema();

    virtual ~Schema();


    uint32         size() const;


    const vector<Type*>& getTypes() const;

    void setFreeTypeID(TypeID nextFreeID);

    //
    Type* getTypeByName(string name);
    Type* getTypeByID(TypeID typeID);

    // addCustomType //
    template <typename T>
    CustomType* addCustomType(string name)
    {
        CustomType *customType = _addCustomType(name);
        type_index ti = type_index(typeid(T));
        typeIndexToId[ti] = customType->getTypeID();
        return customType;
    }

    // addPrimitiveType //
    template <typename T>
    PrimitiveType* addPrimitiveType(string name)
    {
        int size = sizeof(T);
        PrimitiveType *primitiveType = _addPrimitiveType(name, size);
        type_index ti = type_index(typeid(T));
        typeIndexToId[ti] = primitiveType->getTypeID();
        return primitiveType;
    }

    // getTypeID //
    template<typename T>
    bool collectTypeID(TypeID& typeId) const
    {
        type_index ti = type_index(typeid(T));
        auto it = typeIndexToId.find(ti);
        if (it == typeIndexToId.end())
            return false;
        typeId = it->second;
        return true;
    }

    // getTypeID //
    template<typename T>
    bool collectDynamicTypeID(TypeID& typeId, T& obj) const
    {
        type_index ti = type_index(typeid(obj));
        auto it = typeIndexToId.find(ti);
        if (it == typeIndexToId.end())
            return false;
        typeId = it->second;
        return true;
    }


private:

    TypeID                                 freeTypeID;

    vector<Type*>                          types;

    mutable unordered_map<string, Type*>           cacheNameToType;
    mutable unordered_map<TypeID, Type*>           cacheTypeIDToType;
    unordered_map<type_index, TypeID>      typeIndexToId;

private:

    CustomType *_addCustomType(string name);

    PrimitiveType *_addPrimitiveType(string name, int size);

    void rebuildCacheMaps();

    // friend methods
    friend Serializer& operator<<(Serializer& serializer, const Schema &schema);
    friend Deserializer& operator>>(Deserializer& deserializer, Schema &schema);
};

// auxiliar function
template<typename T>
CustomType *retrieveOrCreateCustomType(Schema &schema, string name)
{
    Type *t = schema.getTypeByName(name);
    if (t)
    {
        assert(t->asCustomType());
        return t->asCustomType();
    }
    else
    {
        return schema.addCustomType<T>(name);
    }
}


template <typename T>
CustomType *getCustomType(Schema &schema)
{
    return retrieveOrCreateCustomType<T>(schema,T::TYPENAME);
}

template <typename T>
TypeID getCustomTypeID(Schema &schema)
{
    return retrieveOrCreateCustomType<T>(schema,T::TYPENAME)->getTypeID();
}


}
