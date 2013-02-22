#include <iostream>
#include <iomanip>

#include <serialization/SchemaLog.hpp>

namespace serialization
{

//--------------
// Log Routines
//--------------

std::ostream& operator<<(std::ostream &o, FieldType::Kind kind)
{
    switch (kind)
    {
    case FieldType::VALUE:
        o << "value";
        break;
    case FieldType::POINTER:
        o << "pointer";
        break;
    case FieldType::TAGGED_POINTER:
        o << "tagged_pointer";
        break;
    default:
        o << "unknown";
        break;
    }
    return o;
}

std::ostream& operator<<(std::ostream &o, FieldType::Repeat repeat)
{
    switch (repeat)
    {
    case FieldType::NO_REPEAT:
        o << "no_repeat";
        break;
    case FieldType::REPEAT_32:
        o << "repeat_32";
        break;
    case FieldType::REPEAT_16:
        o << "repeat_16";
        break;
    default:
        o << "unknown";
        break;
    }
    return o;
}









std::ostream& operator<<(std::ostream& o, const FieldType& fieldType)
{
    o << "typeId: "   << std::right << std::setw(4)  << fieldType.getTypeID()
      << "   kind: "    << std::left  << std::setw(15) << fieldType.getKind()
      << " repeat: "  << std::left  << std::setw(15) << fieldType.getRepeat();

//    char text[128];
//    sprintf(text,"typeId: %4d  pointer: %4s  repeated: %4s",
//            fieldType.getTypeID(),
//            fieldType.isPointer() ? "yes" : "no",
//            fieldType.isRepeated() ? "yes" : "no");
//    o << text;
    return o;
}


std::ostream& operator<<(std::ostream& o, const Field& field)
{
    char text[128];
    sprintf(text,"[Field] index: %4d  name: %-32s",
            field.getIndex(),
            field.getName().c_str());
    o << text << "  " << field.getFieldType();
    return o;
}

std::ostream& operator<<(std::ostream& o, const PrimitiveType& primtype)
{
    char text[10000];
    sprintf(text,"[PrimitiveType] typeId: %4d  name: %-32s  size: %8d",
            primtype.getTypeID(),
            primtype.getName().c_str(),
            primtype.getSize());
    o << text;
    return o;
}

std::ostream& operator<<(std::ostream& o, const CustomType& customType)
{
    const vector<Field> fields = customType.getFields();

    char text[10000];
    sprintf(text,"[CustomType] typeId: %4d  name: %-32s",
            customType.getTypeID(),
            customType.getName().c_str());
    o << text;
    for (auto& f: fields)
        o  << std::endl << f;
    return o;
}


std::ostream& operator<<(std::ostream& o, const Schema& s)
{
    const vector<Type*> types = s.getTypes();

    char text[128];
    sprintf(text,"[Schema] num_types: %4ld",
            types.size());

    o << text << std::endl;
    for (Type *t: types)
        if (t->asCustomType())
            o << *t->asCustomType() << std::endl;
        else
            o << *t->asPrimitiveType() << std::endl;
    return o;
}

}
