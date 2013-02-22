#include <iostream>

#include <serialization/CoreSchema.hpp>
#include <serialization/Serializer.hpp>

namespace serialization
{

//-----------------------------------------------------------------------------
// Serializer
//-----------------------------------------------------------------------------

Serializer::Serializer(Schema &schema, string filename):
    schema(schema),
    filename(filename),
    countSerializedObjects(0)
{
//    CoreSchema::initCoreSchema(*schema);
//        schema(new Schema()),

    ostream.open(filename, std::ios::out | std::ios::binary);
    if (ostream.fail())
    {
        throw std::exception();
    }
}

Serializer::~Serializer()
{
    std::clog << "~Serializer()" << std::endl;
    std::clog << "   ostream.close()" << std::endl;
    ostream.close();
}

void Serializer::run()
{
    while (pending.size())
    {
        PendingItem *s = pending.front();
        s->serialize(*this); // new items can be added to be serialized here
        pending.pop();
        delete s;

        countSerializedObjects++;
    }
}

std::ofstream &Serializer::getStream()
{
    return ostream;
}

Schema& Serializer::getSchema()
{
    return schema;
}

//-----------------------------------------------------------------------------
// serialize primitive types
//-----------------------------------------------------------------------------

template <int N>
inline Serializer& write(Serializer &s, char *base)
{
    // std::cout << "writing: " << N << " bytes on serializer" << std::endl;
    s.getStream().write(base, N); // little endian
    // s.getStream().flush();
    return s;
}

template <typename T>
inline char* cp(T *ptr)
{ return reinterpret_cast<char*>(ptr); }

Serializer &operator<<(Serializer &s, char    value) { return write<1>(s, cp(&value)); }
Serializer &operator<<(Serializer &s, int8    value) { return write<1>(s, cp(&value)); }
Serializer &operator<<(Serializer &s, int16   value) { return write<2>(s, cp(&value)); }
Serializer &operator<<(Serializer &s, int32   value) { return write<4>(s, cp(&value)); }
Serializer &operator<<(Serializer &s, int64   value) { return write<8>(s, cp(&value)); }
Serializer &operator<<(Serializer &s, uint8   value) { return write<1>(s, cp(&value)); }
Serializer &operator<<(Serializer &s, uint16  value) { return write<2>(s, cp(&value)); }
Serializer &operator<<(Serializer &s, uint32  value) { return write<4>(s, cp(&value)); }
Serializer &operator<<(Serializer &s, uint64  value) { return write<8>(s, cp(&value)); }
Serializer &operator<<(Serializer &s, float32 value) { return write<4>(s, cp(&value)); }
Serializer &operator<<(Serializer &s, float64 value) { return write<8>(s, cp(&value)); }
Serializer &operator<<(Serializer &s, void*   value) { return write<8>(s, cp(&value)); }

//-----------------------------------------------------------------------------
// serialize header
//-----------------------------------------------------------------------------

Serializer& operator<<(Serializer& serializer, Header& header)
{
    // std::ostream &ostream = s.getStream();
    serializer << header.typeIdAndFlags;   // repeated list is like this
    serializer << header.oldAddr;          // repeated list is like this
    return serializer;
}

}
