#pragma once

namespace serialization
{

typedef void* RawPointer;

template <typename T>
struct Pointer
{
    explicit Pointer(T* ptr):
        ptr(ptr)
    {}

    RawPointer getRawPointer() const
    { return static_cast<RawPointer>(ptr); }

    T& getObject() const
    { return *ptr; }

    T* ptr; // local copy of a pointer of T
};

struct RawPointerRef
{
    RawPointer &ptr; // pointer to a storage location that stores "pointer"

    explicit RawPointerRef(RawPointer &p) :
        ptr(p)
    {}

    void retarget(RawPointer new_target)
    {
        ptr = new_target;
    }

    RawPointer getRawPointer()
    {
        return ptr;
    }
};

template <typename T>
struct PointerRef
{
    explicit PointerRef(T* &ptr):
        ptr(ptr)
    {}

    RawPointer& getRawPointer()
    { return reinterpret_cast<RawPointer&>(ptr); }

    T* &ptr; // reference to pointer of T
};

}

//namespace std
//{
//template <>
//struct hash<serialization::RawPointer>
//{
//public :
//    size_t operator()(const serialization::RawPointer &rp ) const
//    {
//        return rp.hash();
//    }
//};
//}


//struct RawPointer
//{
//    void* ptr;

//    RawPointer():
//        ptr(nullptr)
//    {}

//    template<typename T>
//    explicit RawPointer(T *p):
//        ptr(reinterpret_cast<void*>(p))
//    {}

//    explicit RawPointer(void *p):
//        ptr(p)
//    {}

//    void* asVoidPtr() const
//    {
//        return ptr;
//    }

//    char* asCharPtr() const
//    {
//        return reinterpret_cast<char*>(ptr);
//    }

//    char* getAddressAsCharPtr()
//    {
//        return reinterpret_cast<char*>(&ptr);
//    }

//    bool operator==(const RawPointer& other) const
//    {
//        return ptr == other.ptr;
//    }

//    bool operator<(const RawPointer& other) const
//    {
//        return ptr < other.ptr;
//    }

//    size_t hash() const
//    {
//        return reinterpret_cast<size_t>(ptr);
//    }

//};

//struct RawPointerRef
//{
//    RawPointer *ptr_pointer; // pointer to a storage location that stores "pointer"

//    template<class T>
//    explicit PointerRef(T* &p) :
//        ptr_pointer(reinterpret_cast<RawPointer*>(&p))
//    {}

//    void retarget(RawPointer new_target)
//    {
//        *ptr_pointer = new_target;
//    }

//    RawPointer getTarget()
//    {
//        return *ptr_pointer;
//    }

//    RawPointer *getAddress()
//    {
//        return ptr_pointer;
//    }

//    const RawPointer *getAddress() const
//    {
//        return ptr_pointer;
//    }
//};
