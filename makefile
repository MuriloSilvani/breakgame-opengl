CPPSOURCES = $(wildcard *.cpp)

Target: BrickGame

all:	Target

BrickGame: $(CPPSOURCES:.cpp=.o)
	g++ $< common/shader.cpp common/text2D.cpp common/texture.cpp -o $@ -lglfw -lGLEW -lGL

run:
	./BrickGame
