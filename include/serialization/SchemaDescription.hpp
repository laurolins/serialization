#ifndef SERIALIZATION_SCHEMADESCRIPTION_HPP
#define SERIALIZATION_SCHEMADESCRIPTION_HPP

#include "Schema.hpp"
#include "FieldType.hpp"
#include "Field.hpp"
#include "CustomType.hpp"
#include "PrimitiveType.hpp"
#include "Schema.hpp"

namespace serialization
{

    Schema &operator<<(Schema &schema, const FieldType& fieldType);
    Schema &operator<<(Schema &schema, const Field& field);
    Schema &operator<<(Schema &schema, const CustomType& customType);
    Schema &operator<<(Schema &schema, const PrimitiveType& customType);
    Schema &operator<<(Schema &schema, const Schema& s);

}

#endif // SCHEMADESCRIPTION_HPP
