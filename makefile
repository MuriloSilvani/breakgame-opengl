CPPSOURCES = $(wildcard *.cpp)

Target: Breakgame

all:	Target

Breakgame: $(CPPSOURCES:.cpp=.o)
	g++ $< common/shader.cpp -o $@ -lglfw -lGLEW -lGL

run:
	./Breakgame
