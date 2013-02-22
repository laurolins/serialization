#ifndef SERIALIZATION_MACROS_HPP
#define SERIALIZATION_MACROS_HPP




#define SERIALIZATION_FRIEND_OPERATORS(type) \
friend serialization::Serializer& serialization::operator<<( \
    serialization::Serializer& serializer, \
    const type &obj); \
friend serialization::Deserializer& serialization::operator>>( \
    serialization::Deserializer& deserializer, \
    type &obj);


//
// add deserialization operator as a friend of the class
//
#define SERIALIZATION_FRIEND_HEADER(type) \
namespace serialization \
{ \
    class Deserializer; \
    class Serializer; \
    Serializer &operator<<(Serializer &deserializer, const type &obj); \
    Deserializer &operator>>(Deserializer &deserializer, type &obj); \
}





#endif // MACROS_HPP
