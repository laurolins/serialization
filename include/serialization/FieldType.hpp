#pragma once

#include <serialization/BasicTypes.hpp>
#include <serialization/Type.hpp>

namespace serialization
{

//-----------
// FieldType
//-----------

class FieldType
{
public:

    enum Repeat   { NO_REPEAT, REPEAT_16, REPEAT_32 };
    enum Kind     { VALUE, POINTER, TAGGED_POINTER  };

    FieldType();
    FieldType(TypeID typeId, Kind kind=VALUE, Repeat repeat=NO_REPEAT);
    FieldType(Type  *type,   Kind kind=VALUE, Repeat repeat=NO_REPEAT);

    TypeID   getTypeID()  const;
    Kind     getKind()    const;
    Repeat   getRepeat()  const;
    bool     isRepeated() const;
    bool     isPointerOrTaggedPointer() const;

private:

    uint16 typeId;
    uint8  kind;
    uint8  repeat;

    // give access to private memeber to the deserialization function
    friend Serializer&   operator<<(Serializer&   serializer,   const FieldType &fieldType);
    friend Deserializer& operator>>(Deserializer& deserializer, FieldType &fieldType);

};

}
