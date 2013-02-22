#pragma once

namespace serialization {

    class Serializer;
    class Deserializer;

    class Serializable {
    public:
        virtual ~Serializable() {}
        virtual void save(Serializer& serializer) const = 0;
        virtual void load(Deserializer& deserializer) = 0;
    };


#ifndef SWIG

    Serializer&   operator<<(Serializer& serializer,  const Serializable& obj);
    Deserializer& operator>>(Deserializer& deserializer,    Serializable& obj);

#endif

}
