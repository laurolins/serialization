#include <serialization/Schema.hpp>

#include <serialization/CustomType.hpp>
#include <serialization/PrimitiveType.hpp>

#include <cassert>

namespace serialization
{

    //------------
    // SchemaBase
    //------------

    Schema::Schema():
        freeTypeID(0)
    {}

    Schema::~Schema()
    {
        for (auto a: types)
            delete a;
    }

    void Schema::rebuildCacheMaps()
    {
        cacheNameToType.clear();
        cacheTypeIDToType.clear();
        for (auto t: types)
        {
            cacheNameToType[t->getName()] = t;
            cacheTypeIDToType[t->getTypeID()] = t;
        }
    }

    Type*
    Schema::getTypeByName(string name)
    {
        if (cacheNameToType.size() != types.size())
            rebuildCacheMaps();

        auto it = cacheNameToType.find(name);
        if (it != cacheNameToType.end())
            return it->second;
        return nullptr;
    }

    Type*
    Schema::getTypeByID(TypeID typeID)
    {
        if (cacheTypeIDToType.size() != types.size())
            rebuildCacheMaps();

        auto it = cacheTypeIDToType.find(typeID);
        if (it != cacheTypeIDToType.end())
            return it->second;
        return nullptr;
    }

    const vector<Type*> &Schema::getTypes() const
    {
        return types;
    }

    uint32 Schema::size() const
    {
        return types.size();
    }

    CustomType *Schema::_addCustomType(string name)
    {
        int id = freeTypeID++;
        CustomType *t = new CustomType(this, id, name);
        types.push_back(t);
        cacheNameToType[name]  = t;
        cacheTypeIDToType[id] = t;
        return t;
    }

    PrimitiveType *Schema::_addPrimitiveType(string name, int size)
    {
        int id = freeTypeID++;
        PrimitiveType *t = new PrimitiveType(this, id, name, size);
        types.push_back(t);
        cacheNameToType[name] = t;
        cacheTypeIDToType[id] = t;
        return t;
    }

    void Schema::setFreeTypeID(TypeID nextFreeID)
    {
        assert(freeTypeID < nextFreeID);
        freeTypeID = nextFreeID;
    }

}
