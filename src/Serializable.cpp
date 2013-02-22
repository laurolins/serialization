#include <serialization/Serializable.hpp>

#include <serialization/Deserializer.hpp>
#include <serialization/Serializer.hpp>

namespace serialization {

    Serializer& operator<<(Serializer& serializer, const Serializable& obj)
    {
        obj.save(serializer);
        return serializer;
    }

    Deserializer& operator>>(Deserializer& deserializer, Serializable& obj)
    {
        obj.load(deserializer);
        return deserializer;
    }

}
