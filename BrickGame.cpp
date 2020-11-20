#include <bits/stdc++.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow *window;
#include <glm/glm.hpp>
using namespace glm;
#include "common/shader.hpp"
#include "common/text2D.hpp"

const GLint WIDTH = 800, HEIGHT = 800;
const GLfloat R = 0.0, G = 0.0, B = 0.0, A = 0.0;
GLuint colorbuffer, vertexbuffer;

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

glm::mat3 move_road = glm::mat3(1.0f);
glm::mat3 move_road_lines = glm::mat3(1.0f);
glm::mat3 move_car_1 = glm::mat3(1.0f);
glm::mat3 move_car_2 = glm::mat3(1.0f);

char text[256];
char state = 's';
// s = start
// p = play
// e = end

float speed = 0.01;
float distancedrived = 0.0;

// void moveCar(double deltaTime)
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

	// speed
	if (key == GLFW_KEY_UP)
	{
		if (speed == 0)
			speed = 0.01;
		if (speed < 0.05)
		{
			speed *= 1.2;
		}
	}
	if (key == GLFW_KEY_DOWN)
	{
		if (speed > 0.01)
		{
			speed *= 0.8;
		}
		else
		{
			speed = 0;
		}
	}
}

void randPositionCar2()
{
	int rand = random() % 4;

	rand <= 1 && move_car_2[0][2] != 0 ? move_car_2[0][2] = 0 : rand <= 2 && move_car_2[0][2] != 0.65 ? move_car_2[0][2] = 0.65 : rand <= 3 && move_car_2[0][2] != 1.3 ? move_car_2[0][2] = 1.3 : move_car_2[0][2] = 0;

	move_car_2[1][2] = 2;
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
	int nbFrames = 0;
	do
	{
		double currentTime = glfwGetTime();
		nbFrames++;
		double deltaTime = currentTime - lastTime;
		if (deltaTime >= 1.0)
		{
			nbFrames = 0;
			lastTime += 1.0;
		}
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(programID);
		configLayout(vertexbuffer, colorbuffer);

		// start
		if (state == 's')
		{
			sprintf(text, "%s", "JOGAR");
			printText2D(text, 280, 400, 50);
		}

		// end
		if (state == 'e')
		{
		}

		//
		// if (state == '')
		// {
		// }

		// play
		if (state == 'p')
		{
			glfwSetKeyCallback(window, moveCar);

			if (move_road_lines[1][2] > -0.3)
			{
				move_road_lines[1][2] -= speed;
			}
			else
			{
				move_road_lines[1][2] = -speed;
			}

			if (move_car_2[1][2] > -1)
			{
				move_car_2[1][2] += -speed;
			}
			else
			{
				move_car_2[1][2] = 2;
				randPositionCar2();
			}

			MatrizCombinada = move_road;
			drawModel(modelRoad, MatrizCombinada, MatrixID, 0.15, 0.15, 0.15);

			MatrizCombinada = move_road_lines;
			drawModel(modelLines, MatrizCombinada, MatrixID, 1.0, 1.0, 0.0);

			MatrizCombinada = move_car_1;
			drawModel(modelCar, MatrizCombinada, MatrixID, 1.0, 1.0, 1.0);

			MatrizCombinada = move_car_2;
			drawModel(modelCar, MatrizCombinada, MatrixID, 1.0, 0.0, 1.0);

			distancedrived += speed * lastTime;

			sprintf(text, "%.2fm", distancedrived);
			printText2D(text, 20, 5, 20);

			sprintf(text, "%.2fm/s", speed * 10);
			printText2D(text, 620, 5, 20);
		}

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
	destroyWindows(vertexbuffer, colorbuffer, VertexArrayID, programID);
	return 0;
}
