#pragma once

#if 1

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <stdint.h>
#include <ostream>
#include <istream>
#include <fstream>
#include <stack>
#include <queue>
#include <typeindex>

#include <cstdint>
#include <cassert>

#include <serialization/Pointer.hpp>

namespace serialization {

typedef uint16_t Tag;
typedef void* RawPointerWithTag;

template <typename T>
struct TaggedPointer
{
    typedef uint64_t UInt64;

    //
    static const UInt64 bit47 = (1UL << 47);

    TaggedPointer()
    {
        data.ptr = 0;
    }

    Pointer<T> getPointer() const
    {
        Data x = data;
        x.aux.tag = (reinterpret_cast<UInt64>(data.ptr) & bit47) ? 0xFFFF : 0;
        return Pointer<T>(x.ptr);
    }

    RawPointer getRawPointer() const
    {
        Data x = data;
        x.aux.tag = (reinterpret_cast<UInt64>(data.ptr) & bit47) ? 0xFFFF : 0;
        return static_cast<RawPointerWithTag>(x.ptr);
    }

    RawPointerWithTag& getRawPointerWithTag()
    {
        // the most significant 16 bits are a tag
        return reinterpret_cast<RawPointerWithTag&>(data.ptr);
    }

//    TaggedPointer(T *ptr, Tag tag)
//    {
//        data.ptr = static_cast<void*>(ptr);
//        assert (data.aux.tag == 0 || data.aux.tag == 0xFF);
//        data.aux.tag = tag;
//    }
//    char* getAddressAsCharPtr()
//    {
//        return reinterpret_cast<char*>(&data.ptr);
//    }

    bool operator==(const TaggedPointer& other) const
    {
        // testing pointer with tag!
        return data.ptr == other.data.ptr; // getRawPointer();
    }

    bool operator<(const TaggedPointer& other) const
    {
        return data.ptr < other.data.ptr;
    }

    void setPointer(T* ptr)
    {
        Tag tag = getTag();
        data.ptr = ptr;
        assert (data.aux.tag == 0 || data.aux.tag == 0xFFFF);
        data.aux.tag = tag;
    }

    inline void setTag(Tag tag)
    {
        data.aux.tag = tag;
    }

    inline Tag getTag() const
    {
        return data.aux.tag;
    }

    size_t hash() const
    {
        return reinterpret_cast<size_t>(data.ptr);
    }

    struct Aux{
        Tag pad1;
        Tag pad2;
        Tag pad3;
        Tag tag;
    };

    union Data {
        T     *ptr;
        Aux    aux;
    };

    Data data; // data of tagged pointer
};

//struct RawTaggedPointerRef
//{
//    TaggedPointer<T> &tagged_ptr; // pointer to a storage location that stores "pointer"

//    explicit RawTaggedPointerRef(TaggedPointer<T> &tagptr) :
//        tagptr(p)
//    {}

//    void retarget(RawPointer new_target)
//    {
//        Tag tag = static_cast<uint64>(ptr) >> 48;
//        uint64 result = static_cast<uint64>(new_target) | (static_cast<uint64>(tag) << 48);
//        ptr = static_cast<RawPointerWithTag>(result);
//    }

//    RawPointer getRawPointer()
//    {
//        return ptr;
//    }
//};

template <typename T>
struct TaggedPointerRef
{
    explicit TaggedPointerRef(TaggedPointer<T> &ptr):
        tagged_ptr(ptr)
    {}

//    RawPointer& getRawPointer()
//    { return tagged_ptr.getRawPointerWithTag(); }

    RawPointerWithTag& getRawPointerWithTag()
    { return tagged_ptr.getRawPointerWithTag(); }

    TaggedPointer<T> &tagged_ptr; // reference to pointer of T
};

//struct RawTaggedPointerRef
//{
//    TaggedPointer *ptr_tagged_pointer; // pointer to a storage location that stores "pointer"

//    template<class T>
//    explicit TaggedPointerRef(T* &p) :
//        ptr_tagged_pointer(reinterpret_cast<TaggedPointer*>(&p))
//    {}

//    void retarget(TaggedPointer new_target)
//    {
//        *ptr_tagged_pointer = new_target;
//    }

//    TaggedPointer getTarget()
//    {
//        return *ptr_tagged_pointer;
//    }

//    TaggedPointer *getAddress()
//    {
//        return ptr_tagged_pointer;
//    }

//    const TaggedPointer *getAddress() const
//    {
//        return ptr_tagged_pointer;
//    }
//};

}

namespace std
{

//// specialization std::hash type for serialization::TaggedPointer
//template <>
//struct hash<serialization::TaggedPointer>
//{
//public :
//    size_t operator()(const serialization::TaggedPointer &rp ) const
//    {
//        return rp.hash();
//    }
//};
}


#endif
