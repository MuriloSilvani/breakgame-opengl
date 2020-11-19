CPPSOURCES = $(wildcard *.cpp)

Target: BrickGame

all:	Target

BrickGame: $(CPPSOURCES:.cpp=.o)
	g++ $< common/shader.cpp -o $@ -lglfw -lGLEW -lGL

run:
	./BrickGame
