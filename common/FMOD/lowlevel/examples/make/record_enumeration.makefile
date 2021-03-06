NAME = record_enumeration

ifeq (${CPU}, armhf)
    FLAGS += -marm -march=armv6 -mfpu=vfp -mfloat-abi=hard
else ifeq (${CPU}, arm)
    FLAGS += -marm -march=armv6 -mfpu=vfp -mfloat-abi=softfp
else ifeq (${CPU}, x86)
    FLAGS += -m32
else
    override CPU = x86_64
    FLAGS += -m64
endif

ifeq (${CONFIG}, Debug)
    FLAGS += -g
    SUFFIX = L
else
    override CONFIG = Release
    FLAGS += -O2
    SUFFIX =
endif

SOURCE_FILES = \
    ../record_enumeration.cpp \
    ../common.cpp \
    ../common_platform.cpp

INCLUDE_DIRS = \
    -I../../../lowlevel/inc

LOWLEVEL_LIB = ../../../lowlevel/lib/${CPU}/libfmod${SUFFIX}.so

all:
	g++ -pthread ${FLAGS} -o ${NAME} ${SOURCE_FILES} -Wl,-rpath=\$$ORIGIN/$(dir ${LOWLEVEL_LIB}) ${LOWLEVEL_LIB} ${INCLUDE_DIRS}
