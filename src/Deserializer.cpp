#include <iostream>

#include <serialization/Deserializer.hpp>
#include <serialization/Header.hpp>
#include <serialization/CoreSchema.hpp>

#include <serialization/SchemaSerialization.hpp>


namespace serialization
{

//------------
// Serializer
//------------


Deserializer::Deserializer(serialization::string filename):
    filename(filename),
    num_deserialized_objects(0)
{
    // register class factories for the types of the schema
    CoreSchema::initCoreDeserializer(*this);

    // schema is a retrieve type
    retrieve_types.insert(CoreSchema::ID_Schema);


//    buffer.resize(100000000);
//    istream.rdbuf()->pubsetbuf(&buffer[0], buffer.size());

    istream.open(filename, std::ios::in | std::ios::binary);
    if (!istream.good())
    {
        throw std::exception();
    }
}

void Deserializer::addRetrieveType(TypeID type_id)
{
    retrieve_types.insert(type_id);
}

void Deserializer::registerProgressCallback(Deserializer::ProgressCallback *pc)
{
    progress_callbacks.push_back(pc);
}

std::ifstream &Deserializer::getStream()
{
    return istream;
}

void Deserializer::addOutdatedPointer(PointerToRetarget p_retarget)
{
    RawPointer old_ptr = p_retarget.getRawPointer();

    // check if we already have the new location of old pointer
    auto itMapOldNew = mapOldNew.find(old_ptr);
    if (itMapOldNew != mapOldNew.end()) {
        // if we have the new location just retarget p_retarted
        p_retarget.retarget(itMapOldNew->second);
    }
    else
    {
        // add p_retarget to the outdated list of objects
        outdatedPointers[old_ptr].push_back(p_retarget);
        // this->outdatedPointers.push_back(p_retarget);
    }
}

void Deserializer::addOldNewMapping(RawPointer oldPtr, RawPointer newPtr)
{
    mapOldNew[oldPtr] = newPtr;

    auto it = outdatedPointers.find(oldPtr);
    if (it != outdatedPointers.end())
    {
        for (auto &ptr: it->second)
        {
            ptr.retarget(newPtr);
        }
        outdatedPointers.erase(it); // free some space
    }
}

vector<RawPointer>& Deserializer::getDeserializedObjectsByType(TypeID typeId) const
{
    static vector<RawPointer> empty_list; // allways empty

    auto it = deserializedObjects.find(typeId);
    if (it == deserializedObjects.end())
    {
        return empty_list;
    }
    else
    {
        return *it->second;
    }
}

void Deserializer::addDeserializedObjectByType(TypeID typeId, RawPointer ptr)
{
    auto it = deserializedObjects.find(typeId);
    vector<RawPointer> *vec = nullptr;
    if (it == deserializedObjects.end())
    {
        vec = new vector<RawPointer>();
        deserializedObjects[typeId] = vec;
    }
    else
    {
        vec = it->second;
    }
    vec->push_back(ptr);
}

void Deserializer::run(TypeID a, TypeID b)
{
    //
    // assume the core schema primitive and custom types are the same
    //
    Header header;
    while (1)
    {
        std::streampos checkpoint = istream.tellg();

        // try to read block
        (*this) >> header.typeIdAndFlags;   // repeated list is like this
        if (!istream.good())
            break;

        (*this) >> header.oldAddr;          // repeated list is like this
        if (!istream.good())
            break;

        // type not in range
        if (header.getTypeId() < a || header.getTypeId() > b)
        {
            istream.seekg(checkpoint);
            break;
        }

        // valid header
        RawPointer newAddr = classManager.createAndDeserialize(header.getTypeId(), *this);

        this->addOldNewMapping(header.oldAddr, newAddr);

        // insert deserialized object if in the retrieve types list
        if (this->retrieve_types.count(header.getTypeId()) == 1)
            this->addDeserializedObjectByType(header.getTypeId(),newAddr);

        num_deserialized_objects++;

        if ((num_deserialized_objects % 100000) == 0)
        {
            for (auto pcptr: progress_callbacks)
            {
                ProgressCallback &pc = *pcptr;
                pc(num_deserialized_objects);
            }
            // std::cout << "Deserialized " << count << " objects" << std::endl;
        }

    }

    // retarget pointers
//    for (auto outdatedPtr: outdatedPointers)
//    {
//        outdatedPtr.retarget(mapOldNew[outdatedPtr.getRawPointer()]);
//    }
}


static void margin(std::ostream &os, int k)
{
    for (int i=0;i<k;i++)
        os << " ";
}


//    struct Log {
//        ostream      &os;
//        Deserializer &deserializer;
//        int          depth;
//        str
//    };

void Deserializer::logNextBytesAs(Type *type,
                                  int depth,
                                  Schema& schema,
                                  std::ostream &os,
                                  string contextName)
{
    CustomType* t = type->asCustomType();
    if (t)
        this->logNextBytesAs(t, depth, schema, os, contextName);
    else
        this->logNextBytesAs(type->asPrimitiveType(), depth, schema, os, contextName);
}

void Deserializer::logNextBytesAs(CustomType *type,
                                  int depth,
                                  Schema& schema,
                                  std::ostream &os,
                                  string contextName)
{
    margin(os, 4 * depth);
    os << contextName << ": " << type->getName() << " = " << std::endl;
    for (auto &f: type->getFields())
    {
        FieldType ft = f.getFieldType();
        TypeID fieldTypeID = ft.getTypeID();

        // query schema for actual type description from TypeID
        Type* fieldType = schema.getTypeByID(fieldTypeID);

        if (!fieldType)
        {
            os << "[Error] Found field with TypeID=" << fieldTypeID
               << " but this TypeID is not present in given schema (file position: "
               << istream.tellg() << ")" << std::endl;
            return;
        }

        uint32 offset = 0;
        uint32 count = 1;
        if (ft.isRepeated())
        {
            Deserializer &d = *this;
            if (ft.getRepeat() == FieldType::REPEAT_16)
            {
                uint16 aux;
                d >> aux;
                count = aux;
            }
            else if (ft.getRepeat() == FieldType::REPEAT_32)
            {
                uint32 aux;
                d >> aux;
                count = aux;
            }

            offset = 1;
            margin(os, 4 * (depth + offset));
            os << f.getName() << ": " << count << " x " << fieldType->getName() << std::endl;


        }


        for (uint32 i=0;i<count;i++)
        {
            if (ft.isPointerOrTaggedPointer())
            {
                Deserializer &d = *this;
                RawPointer rp;
                d >> rp;
                margin(os, 4 * (depth + offset + 1));
                if (!ft.isRepeated())
                    os << f.getName();
                os << ": pointer = " <<  "0x" <<std::hex << (uint64) rp << std::dec << std::endl;
            }
            else
            {
                logNextBytesAs(fieldType, depth + 1 + offset, schema, os, ft.isRepeated() ? "" : f.getName());
            }
        }

    }
}

void Deserializer::logNextBytesAs(PrimitiveType *type,
                                  int depth,
                                  Schema& ,
                                  std::ostream &os,
                                  string contextName)
{
    margin(os, 4 * depth);
    os << contextName << ": " << type->getName() << " = ";

    uint8 byte;
    unsigned char b;

    Deserializer &d = *this;

    int32    i32;
    int64    i64;
    uint32   ui32;
    uint64   ui64;
    float32  f32;
    float64  f64;

    switch (type->getTypeID())
    {
    case CoreSchema::ID_float32:
        d >> f32;
        os << f32;
        break;
    case CoreSchema::ID_float64:
        d >> f64;
        os << f64;
        break;
    case CoreSchema::ID_int32:
        d >> i32;
        os << i32;
        break;
    case CoreSchema::ID_int64:
        d >> i64;
        os << i64;
        break;
    case CoreSchema::ID_uint32:
        d >> ui32;
        os << ui32;
        break;
    case CoreSchema::ID_uint64:
        d >> ui64;
        os << ui64;
        break;
    default:
        for (int i=0;i<type->getSize();i++)
        {
            d >> byte;
            b = (unsigned char) byte;
            os << "0x" << std::hex << (b < 10 ? "0" : "") << (int) b << std::dec << " ";
        }
        break;
    }

    os << std::endl;
}


void Deserializer::log(Schema &schema, std::ostream &os)
{
    //
    // assume the core schema primitive and custom types are the same
    //
    Header header;
    while (1)
    {
        std::streampos checkpoint = istream.tellg();

        // try to read block
        (*this) >> header.typeIdAndFlags;   // repeated list is like this
        if (!istream.good())
            break;

        (*this) >> header.oldAddr;          // repeated list is like this
        if (!istream.good())
            break;


        // query schema for actual type description from TypeID
        Type* type = schema.getTypeByID(header.getTypeId());

        if (!type)
        {
            os << "[Error] Found block with TypeID=" << header.getTypeId()
               << " but this TypeID is not present in given schema (file position: "
               << checkpoint << ")" << std::endl;
            return;
        }

        os << "----- "
           << "0x" << std::hex << (uint64) header.oldAddr
           << "----- " << std::endl;
        logNextBytesAs(type, 0, schema, os, "");

    }

}

void Deserializer::addObjectFactory(TypeID typeId, AbstractObjectFactory *classFactory)
{
    classManager.add(typeId, classFactory);
}

//    template <typename T>
//    void Deserializer::addClassFactory(TypeID typeId, AbstractClassFactory *classFactory)
//    {
//        classManager.add(typeId, classFactory);
//    }



Deserializer::~Deserializer()
{
    std::clog << "~Deserializer()" << std::endl;
    std::clog << "   istream.close()" << std::endl;
    istream.close();
}


#if 0
Deserializer &operator>>(Deserializer &deserializer, RawPointerRef& p)
{
    char* value_ptr   = reinterpret_cast<char*>(p.getAddress());
    std::istream &istream = deserializer.getStream();
    istream.read(value_ptr, sizeof(rawptr));
    return deserializer;
}

Deserializer &operator>>(Deserializer &deserializer, RawPointer& p)
{
    std::istream &istream = deserializer.getStream();
    istream.read(p.getAddressAsCharPtr(), sizeof(rawptr));
    return deserializer;
}
#endif

//-----------------------------------------------------------------------------
// deserialize primitive types
//-----------------------------------------------------------------------------

template <int N>
inline Deserializer& read(Deserializer &d, char* base)
{
    d.getStream().read(base, N); // little endian
    return d;
}

template <typename T>
inline char* cp(T *ptr)
{ return reinterpret_cast<char*>(ptr); }

//Deserializer &operator>>(Deserializer &d, char    &value) {
//    char ch;
//    d.getStream().read(&ch, 1);
//    std::cout << "Char: " << ch << std::endl;
//    value = ch;
//    return d; // read<1>(d, cp(&value));
//}

Deserializer &operator>>(Deserializer &d, char    &value) { return read<1>(d, cp(&value)); }
Deserializer &operator>>(Deserializer &d, int8    &value) { return read<1>(d, cp(&value)); }
Deserializer &operator>>(Deserializer &d, int16   &value) { return read<2>(d, cp(&value)); }
Deserializer &operator>>(Deserializer &d, int32   &value) { return read<4>(d, cp(&value)); }
Deserializer &operator>>(Deserializer &d, int64   &value) { return read<8>(d, cp(&value)); }
Deserializer &operator>>(Deserializer &d, uint8   &value) { return read<1>(d, cp(&value)); }
Deserializer &operator>>(Deserializer &d, uint16  &value) { return read<2>(d, cp(&value)); }
Deserializer &operator>>(Deserializer &d, uint32  &value) { return read<4>(d, cp(&value)); }
Deserializer &operator>>(Deserializer &d, uint64  &value) { return read<8>(d, cp(&value)); }
Deserializer &operator>>(Deserializer &d, float32 &value) { return read<4>(d, cp(&value)); }
Deserializer &operator>>(Deserializer &d, float64 &value) { return read<8>(d, cp(&value)); }

// this operator will match types: RawPointer and RawPointerWithTag
Deserializer &operator>>(Deserializer &d, void*   &value) { return read<8>(d, cp(&value)); }

//-----------------------------------------------------------------------------
// deserialize header
//-----------------------------------------------------------------------------

Deserializer& operator>>(Deserializer& deserializer, Header& header)
{
    // std::ostream &ostream = s.getStream();
    deserializer >> header.typeIdAndFlags;   // repeated list is like this
    deserializer >> header.oldAddr;          // repeated list is like this
    return deserializer;
}














#if 0
Deserializer &operator>>(Deserializer &deserializer, string& st)
{
    uint32 length;
    deserializer >> length;

    vector<char> chars(length);

    std::istream &istream = deserializer.getStream();
    istream.read(&chars[0], length); // little endian

    st.assign(chars.begin(), chars.end());

    return deserializer;
}
#endif


}
