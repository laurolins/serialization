QMAKE_CXXFLAGS  += -std=c++11 -stdlib=libc++ -mmacosx-version-min=10.7 -Wno-unused-parameter
QMAKE_LFLAGS    += -std=c++11 -stdlib=libc++ -mmacosx-version-min=10.7 -Wno-unused-parameter

QT         -=  gui core

#CONFIG     += debug
#ROOT="/Users/lauro/projects/serialize"

INCLUDEPATH = include/

SOURCES = \
    src/Serializer.cpp \
    src/Deserializer.cpp \
    src/Header.cpp \
    src/FieldType.cpp \
    src/Field.cpp \
    src/Type.cpp \
    src/PrimitiveType.cpp \
    src/CustomType.cpp \
    src/SchemaSerialization.cpp \
    src/SchemaLog.cpp \
    src/SchemaDescription.cpp \
    src/CoreSchema.cpp \
    src/Schema.cpp \
    src/Serializable.cpp \
    examples/example3.cpp
    # examples/slog.cpp


HEADERS = \
    include/serialization/Serializer.hpp \
    include/serialization/Deserializer.hpp \
    include/serialization/BasicTypes.hpp \
    include/serialization/Header.hpp \
    include/serialization/FieldType.hpp \
    include/serialization/Field.hpp \
    include/serialization/Type.hpp \
    include/serialization/PrimitiveType.hpp \
    include/serialization/CustomType.hpp \
    include/serialization/SchemaSerialization.hpp \
    include/serialization/SchemaLog.hpp \
    include/serialization/SchemaDescription.hpp \
    include/serialization/CoreSchema.hpp \
    include/serialization/Schema.hpp \
    include/serialization/Serializable.hpp \
    include/serialization/Macros.hpp



# Serialization.cpp \
# Serialization.hpp \
