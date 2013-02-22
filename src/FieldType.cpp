#include <serialization/FieldType.hpp>

namespace serialization
{

    //-----------
    // FieldType
    //-----------

    FieldType::FieldType():
        typeId(-1),
        kind(VALUE),
        repeat(NO_REPEAT)
    {}

    FieldType::FieldType(TypeID typeId, FieldType::Kind kind, FieldType::Repeat repeat):
        typeId(static_cast<uint16>(typeId)),
        kind(static_cast<uint8>(kind)),
        repeat(static_cast<uint8>(repeat))
    {}

    FieldType::FieldType(Type  *type, FieldType::Kind kind, FieldType::Repeat repeat):
        FieldType::FieldType(type->getTypeID(), kind, repeat)
    {}

    TypeID FieldType::getTypeID() const
    {
        return typeId;
    }

    FieldType::Kind FieldType::getKind() const
    {
        return static_cast<Kind>(kind);
    }

    FieldType::Repeat FieldType::getRepeat() const
    {
        return static_cast<Repeat>(repeat);
    }

    bool FieldType::isRepeated() const
    {
        return repeat != NO_REPEAT;
    }

    bool serialization::FieldType::isPointerOrTaggedPointer() const
    {
        return (kind == POINTER || kind == TAGGED_POINTER);
    }

}
