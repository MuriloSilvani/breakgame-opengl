CPPSOURCES = $(wildcard *.cpp)

ifeq (${CPU}, armhf)
    FLAGS += -marm -march=armv6 -mfpu=vfp -mfloat-abi=hard
else ifeq (${CPU}, arm)
    FLAGS += -marm -march=armv6 -mfpu=vfp -mfloat-abi=softfp
else ifeq (${CPU}, x86)
    #FLAGS += -m32
else
    override CPU = x86_64
    #FLAGS += -m64
endif

ifeq (${CONFIG}, Debug)
    #FLAGS += -g
    SUFFIX = L
else
    override CONFIG = Release
    #FLAGS += -O2
    SUFFIX =
endif

SOURCE_FILES = \
    common/texture.cpp \
    common/shader.cpp \

Target: BrickGame

all:	Target

BrickGame: $(CPPSOURCES:.cpp=.o)
	g++ $< common/shader.cpp common/text2D.cpp common/texture.cpp -o $@ -lAntTweakBar -lglfw -lGLEW -lGL

run:
	./BrickGame
