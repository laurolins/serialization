LOCAL=$$system(echo $$HOME)/local

macx {
# mac only
#  -stdlib=libc++
QMAKE_CXXFLAGS  += -std=c++11 -mmacosx-version-min=10.7 -Wno-unused-parameter
QMAKE_LFLAGS    +=
}
unix:!macx{
# linux only
# QMAKE_CXXFLAGS  += -std=c++11 -I$${LOCAL}/include -Wunused-parameter
QMAKE_CXXFLAGS  += -std=c++11 -Wunused-parameter
QMAKE_LFLAGS    +=
}
QT    -=  gui core opengl
LIBS  -= -lQtGui -lQtCore

INCLUDEPATH += include

HEADERS =  \
build/config.h \
include/serialization/BasicTypes.hpp \
include/serialization/CoreSchema.hpp \
include/serialization/CustomType.hpp \
include/serialization/Deserializer.hpp \
include/serialization/Field.hpp \
include/serialization/FieldType.hpp \
include/serialization/Header.hpp \
include/serialization/Macros.hpp \
include/serialization/PrimitiveType.hpp \
include/serialization/Schema.hpp \
include/serialization/SchemaDescription.hpp \
include/serialization/SchemaLog.hpp \
include/serialization/SchemaSerialization.hpp \
include/serialization/Serializable.hpp \
include/serialization/Serializer.hpp \
include/serialization/TaggedPointer.hpp \
include/serialization/Type.hpp \
include/serialization/Pointer.hpp

SOURCES =  \
#examples/slog.cpp \
examples/example_tagged_pointer.cpp \
src/CoreSchema.cpp \
src/CustomType.cpp \
src/Deserializer.cpp \
src/Field.cpp \
src/FieldType.cpp \
src/Header.cpp \
src/PrimitiveType.cpp \
src/Schema.cpp \
src/SchemaDescription.cpp \
src/SchemaLog.cpp \
src/SchemaSerialization.cpp \
src/Serializable.cpp \
src/Serializer.cpp \
src/Type.cpp
