#include <bits/stdc++.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow *window;
#include <glm/glm.hpp>
using namespace glm;
#include "common/shader.hpp"
#include "common/text2D.hpp"
#include "AntTweakBar/include/AntTweakBar.h"

const GLint WIDTH = 1280, HEIGHT = 720;
const GLfloat R = 0.0, G = 0.0, B = 0.0, A = 0.0;
GLuint colorbuffer, vertexbuffer;

glm::mat3 move_road = glm::mat3(1.0f);
glm::mat3 move_road_lines = glm::mat3(1.0f);
glm::mat3 move_car_1 = glm::mat3(1.0f);
glm::mat3 move_car_2 = glm::mat3(1.0f);

double color_road[] = {0.15, 0.15, 0.15};
double color_road_lines[] = {1.0, 1.0, 0.0};
double color_car_1[] = {1.0, 1.0, 1.0};
double color_car_2[] = {1.0, 0.0, 1.0};

char text[255];
char state[20] = "play";
// start
// scoreboard
// settings
// play
// end

float speed = 1;
float distancedrived = 0.0;

void addBars()
{
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(WIDTH, HEIGHT);

	TwBar *bar = TwNewBar("BRICKGAME");
	TwSetParam(bar, NULL, "position", TW_PARAM_CSTRING, 1, "550 200");
	TwSetParam(bar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");

	TwAddVarRW(bar, "color_road", TW_TYPE_COLOR4F, &color_road, "colormode=rgb");
	TwAddVarRW(bar, "color_road_lines", TW_TYPE_COLOR4F, &color_road_lines, "colormode=rgb");
	TwAddVarRW(bar, "color_car_1", TW_TYPE_COLOR4F, &color_car_1, "colormode=rgb");
	TwAddVarRW(bar, "color_car_2", TW_TYPE_COLOR3F, &color_car_2, "colormode=rgb");
}

int initWindow()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3.3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3.3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// window = glfwCreateWindow(WIDTH, HEIGHT, "Brick game", glfwGetPrimaryMonitor(), NULL);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Brick game", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	addBars();
	glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)TwEventMouseButtonGLFW);
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)TwEventMousePosGLFW);
	glfwSetScrollCallback(window, (GLFWscrollfun)TwEventMouseWheelGLFW);
	glfwSetKeyCallback(window, (GLFWkeyfun)TwEventKeyGLFW);
	glfwSetCharCallback(window, (GLFWcharfun)TwEventCharGLFW);

	glClearColor(R, G, B, A);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glDepthFunc(GL_LESS);
	return 0;
}

void destroyWindows(GLuint vertexbuffer, GLuint colorbuffer, GLuint VertexArrayID, GLuint programID)
{
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glfwTerminate();
}

void configLayout(GLuint vertexbuffer, GLuint colorbuffer)
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
}

std::vector<glm::vec3> colorir(int tam, float R, float G, float B)
{
	int i;
	std::vector<glm::vec3> vertexColor;
	for (i = 0; i < tam; i++)
	{
		glm::vec3 color;
		color.r = R;
		color.g = G;
		color.b = B;
		vertexColor.push_back(color);
	}
	return vertexColor;
}

std::vector<glm::vec2> loadModel(const char *path, int z)
{
	FILE *file = fopen(path, "r");
	std::vector<glm::vec2> vertices;
	if (file == NULL)
	{
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		fclose(file);
		getchar();
		return vertices;
	}
	while (1)
	{
		glm::vec2 vertex;
		int res = fscanf(file, "%f %f\n", &vertex.x, &vertex.y);
		// vertex.z = z;
		if (res == EOF)
			break;
		vertices.push_back(vertex);
	}
	fclose(file);
	return vertices;
}

void drawModel(std::vector<glm::vec2> vertices, glm::mat3 MatrizCombinada, GLuint MatrixID, GLfloat R, GLfloat G, GLfloat B)
{
	glUniformMatrix3fv(MatrixID, 1, GL_TRUE, &MatrizCombinada[0][0]);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	std::vector<glm::vec3> color = colorir(vertices.size(), R, G, B);
	glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(glm::vec3), &color[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void randPositionCar2()
{
	srand(time(NULL));
	int rand = random() % 4;

	color_car_2[0] = ((random() % 10) + 1) / 10.0;
	color_car_2[1] = ((random() % 10) + 1) / 10.0;
	color_car_2[2] = ((random() % 10) + 1) / 10.0;

	rand <=
							1 &&
					move_car_2[0][2] != 0
			? move_car_2[0][2] = 0
			: rand <= 2 && move_car_2[0][2] != 0.65 ? move_car_2[0][2] = 0.65 : rand <= 3 && move_car_2[0][2] != 1.3 ? move_car_2[0][2] = 1.3 : move_car_2[0][2] = 0;

	move_car_2[1][2] = 2;
}

void moveScenary(double syncSpeed)
{
	if (move_road_lines[1][2] > -0.3)
	{
		move_road_lines[1][2] -= (syncSpeed * speed);
	}
	else
	{
		move_road_lines[1][2] = 0.29;
	}

	if (move_car_2[1][2] > -1)
	{
		move_car_2[1][2] -= (syncSpeed * speed) * 2;
	}
	else
	{
		move_car_2[1][2] = -2;
		randPositionCar2();
	}
}

void moveCar(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	// VIRA CARO
	if (key == GLFW_KEY_RIGHT)
	{
		switch (action)
		{
		case GLFW_PRESS:
			if (move_car_1[0][2] + 0.65 < 1.3 && speed > 0)
			{
				move_car_1[0][2] += 0.65;
			}
			break;
		}
	}
	if (key == GLFW_KEY_LEFT)
	{
		switch (action)
		{
		case GLFW_PRESS:
			if (move_car_1[0][2] > 0 && speed > 0)
			{
				move_car_1[0][2] -= 0.65;

				if (move_car_1[0][2] < 0)
					move_car_1[0][2] = 0;
			}
			break;
		}
	}
}

void changeState(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_1)
	{
		strcpy(state, "start");
	}
	if (key == GLFW_KEY_2)
	{
		strcpy(state, "end");
	}
	if (key == GLFW_KEY_3)
	{
		strcpy(state, "scoreboard");
	}
	if (key == GLFW_KEY_4)
	{
		strcpy(state, "play");
	}
	if (key == GLFW_KEY_5)
	{
		strcpy(state, "settings");
	}
}

int main(void)
{
	initWindow();
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	GLuint programID = LoadShaders("./shaders/vertexShader.vertexshader", "./shaders/fragmentShader.fragmentshader");
	std::vector<glm::vec3> color;

	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &colorbuffer);

	GLuint MatrixID = glGetUniformLocation(programID, "MatrizCombinada");
	glm::mat3 MatrizCombinada = glm::mat3(1.0f);

	initText2D("utils/Holstein.DDS");

	std::vector<glm::vec2> modelRoad = loadModel("models/road.txt", 0);
	std::vector<glm::vec2> modelLines = loadModel("models/lines.txt", 1);
	std::vector<glm::vec2> modelCar = loadModel("models/car.txt", 2);

	move_car_2[1][2] = 2;
	randPositionCar2();

	double lastTime = glfwGetTime();
	int fps = 0;

	double lastFrameTime = 0;
	double sync;
	do
	{
		double currentTime = glfwGetTime();

		fps++;
		if (currentTime - lastTime >= 1.0)
		{
			printf("%d fps\n", fps);
			fps = 0;
			lastTime += 1.0;

			if (strcmp(state, "play") == 0)
			{
				speed *= 1.005;
			}
		}
		distancedrived += (speed / 10);

		sync = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(programID);
		configLayout(vertexbuffer, colorbuffer);

		// start
		if (strcmp(state, "start") == 0)
		{
			printText2D("BRICK GAME", 120, 450, 60);

			printText2D("PLAY", 370, 300, 20);
			printText2D("SCOREBOARD", 70, 300, 20);
			printText2D("SETTINGS", 570, 300, 20);

			printText2D("EXIT", 300, 100, 50);
		}

		// end
		if (strcmp(state, "end") == 0)
		{
			printText2D("YOU LOSE", 200, 500, 50);

			sprintf(text, "Total distance: %.2fm", distancedrived);
			printText2D(text, 200, 450, 20);

			printText2D("PLAY AGAIN", 170, 300, 50);
			printText2D("EXIT", 300, 100, 50);
		}

		// scoreboard
		if (strcmp(state, "scoreboard") == 0)
		{
			printText2D("SCOREBOARD", 170, 500, 50);
		}

		// settings
		if (strcmp(state, "settings") == 0)
		{
			printText2D("SETTINGS", 170, 500, 50);

			TwDraw();
		}

		// play
		if (strcmp(state, "play") == 0)
		{
			glfwSetKeyCallback(window, moveCar);

			moveScenary(sync);

			drawModel(modelRoad, move_road, MatrixID, color_road[0], color_road[1], color_road[2]);
			drawModel(modelLines, move_road_lines, MatrixID, color_road_lines[0], color_road_lines[1], color_road_lines[2]);
			drawModel(modelCar, move_car_1, MatrixID, color_car_1[0], color_car_1[1], color_car_1[2]);
			drawModel(modelCar, move_car_2, MatrixID, color_car_2[0], color_car_2[1], color_car_2[2]);

			sprintf(text, "%.1fm", distancedrived);
			printText2D(text, 20, 5, 20);

			sprintf(text, "%.1fm/s", speed * 10);
			printText2D(text, 620, 5, 20);
		}

		// glfwSetKeyCallback(window, changeState);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
	destroyWindows(vertexbuffer, colorbuffer, VertexArrayID, programID);
	return 0;
}
