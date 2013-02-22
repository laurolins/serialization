#pragma once

#include <serialization/Schema.hpp>
#include <serialization/FieldType.hpp>
#include <serialization/Field.hpp>
#include <serialization/CustomType.hpp>
#include <serialization/PrimitiveType.hpp>
#include <serialization/Schema.hpp>

namespace serialization
{
    std::ostream& operator<<(std::ostream& o, const FieldType& fieldType);
    std::ostream& operator<<(std::ostream& o, const Field& field);
    std::ostream& operator<<(std::ostream& o, const PrimitiveType& primtype);
    std::ostream& operator<<(std::ostream& o, const CustomType& customType);
    std::ostream& operator<<(std::ostream& o, const Schema& s);
}
