#include <bits/stdc++.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow* window;
#include <glm/glm.hpp>
#include </usr/include/glm/gtc/matrix_transform.hpp>
using namespace glm;
#include "common/shader.hpp"

const GLint WIDTH = 800, HEIGHT = 800;
const GLfloat R = 0.15, G = 0.15, B = 0.15, A = 0;
double xposMouse, yposMouse;
int widthWindow, heightWindow;

int initWindow(){
	if( !glfwInit() ){
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3.3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3.3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

 	window = glfwCreateWindow( WIDTH, HEIGHT, "Color triangle", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glClearColor(R, G, B, A);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void destroyWindows(GLuint vertexbuffer, GLuint colorbuffer, GLuint VertexArrayID, GLuint programID){
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glfwTerminate();
}

void configLayout(GLuint vertexbuffer, GLuint colorbuffer){
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,0,(void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);
}

// ROAD POSITION
glm::mat3 move_roud_lines(
	glm::vec3(1, 0, 0),
	glm::vec3(0, 1, 0),
	glm::vec3(0, 0, 1)
);
// CAR 1 POSITION
glm::mat3 move_marea_glx(
	glm::vec3(1, 0, 0),
	glm::vec3(0, 1, 0),
	glm::vec3(0, 0, 1)
);
// CAR 2 POSITION
glm::mat3 move_car_2(
	glm::vec3(1, 0, 0),
	glm::vec3(0, 1, 0),
	glm::vec3(0, 0, 1)
);


float velocidade = 0.01;

void KeyboardMovimentObject(GLFWwindow *window, int key, int scancode, int action, int mods){
	// VIRA CARO
	if (key == GLFW_KEY_RIGHT){
		switch (action){
			case GLFW_PRESS:
				if(move_marea_glx[0][2]+0.65 < 1.3 && velocidade > 0){
					move_marea_glx[0][2] += 0.65;
				}		
			break;
		}		
	}
	if (key == GLFW_KEY_LEFT){
		switch (action){
			case GLFW_PRESS:
				if(move_marea_glx[0][2] > 0 && velocidade > 0){
					move_marea_glx[0][2] -= 0.65;

					if(move_marea_glx[0][2] < 0) move_marea_glx[0][2] = 0;
				}
			break;
		}		
	}

	// VELOCIDADE
	if(key == GLFW_KEY_UP){
		if(velocidade == 0) velocidade = 0.01;
		if(velocidade < 0.05){
			velocidade *= 1.2;
		}		
	}
	if(key == GLFW_KEY_DOWN){
		if(velocidade > 0.01){
			velocidade *= 0.8;
		}else{
			velocidade = 0;
		}
	}
}

void randPositionCar2(){
	int rand = random() % 4;

	rand <= 1 && move_car_2[0][2] != 0 ?
		move_car_2[0][2] = 0 :
	rand <= 2 && move_car_2[0][2] != 0.65 ?
		move_car_2[0][2] = 0.65 :
	rand <= 3 && move_car_2[0][2] != 1.3 ?
		move_car_2[0][2] = 1.3 :
	move_car_2[0][2] = 0;

	move_car_2[1][2] = 2;
}

int main( void ){
 	initWindow();

	GLuint VertexArrayID;		

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );

	// ROAD LINES
	static const GLfloat vertex_road_lines[] = { 
		// 1
		-100, 130, 4,
		-100, 110, 4,
		-95, 130, 4,
		-100, 110, 4,
		-95, 130, 4,
		-95, 110, 4,

		-100, 100, 4,
		-100, 80, 4,
		-95, 100, 4,
		-100, 80, 4,
		-95, 100, 4,
		-95, 80, 4,
		
		-100, 70, 4,
		-100, 50, 4,
		-95, 70, 4,
		-100, 50, 4,
		-95, 70, 4,
		-95, 50, 4,
		
		-100, 40, 4,
		-100, 20, 4,
		-95, 40, 4,
		-100, 20, 4,
		-95, 40, 4,
		-95, 20, 4,
		
		-100, 10, 4,
		-100, -10, 4,
		-95, 10, 4,
		-100, -10, 4,
		-95, 10, 4,
		-95, -10, 4,
		
		-100, -20, 4,
		-100, -40, 4,
		-95, -20, 4,
		-100, -40, 4,
		-95, -20, 4,
		-95, -40, 4,
		
		-100, -50, 4,
		-100, -70, 4,
		-95, -50, 4,
		-100, -70, 4,
		-95, -50, 4,
		-95, -70, 4,
		
		-100, -80, 4,
		-100, -100, 4,
		-95, -80, 4,
		-100, -100, 4,
		-95, -80, 4,
		-95, -100, 4,
		
		-100, -110, 4,
		-100, -130, 4,
		-95, -110, 4,
		-100, -130, 4,
		-95, -110, 4,
		-95, -130, 4,
		// 2
		-35, 130, 4,
		-35, 110, 4,
		-30, 130, 4,
		-35, 110, 4,
		-30, 130, 4,
		-30, 110, 4,

		-35, 100, 4,
		-35, 80, 4,
		-30, 100, 4,
		-35, 80, 4,
		-30, 100, 4,
		-30, 80, 4,
		
		-35, 70, 4,
		-35, 50, 4,
		-30, 70, 4,
		-35, 50, 4,
		-30, 70, 4,
		-30, 50, 4,
		
		-35, 40, 4,
		-35, 20, 4,
		-30, 40, 4,
		-35, 20, 4,
		-30, 40, 4,
		-30, 20, 4,
		
		-35, 10, 4,
		-35, -10, 4,
		-30, 10, 4,
		-35, -10, 4,
		-30, 10, 4,
		-30, -10, 4,
		
		-35, -20, 4,
		-35, -40, 4,
		-30, -20, 4,
		-35, -40, 4,
		-30, -20, 4,
		-30, -40, 4,
		
		-35, -50, 4,
		-35, -70, 4,
		-30, -50, 4,
		-35, -70, 4,
		-30, -50, 4,
		-30, -70, 4,
		
		-35, -80, 4,
		-35, -100, 4,
		-30, -80, 4,
		-35, -100, 4,
		-30, -80, 4,
		-30, -100, 4,
		
		-35, -110, 4,
		-35, -130, 4,
		-30, -110, 4,
		-35, -130, 4,
		-30, -110, 4,
		-30, -130, 4,
		// 3
		35, 130, 4,
		35, 110, 4,
		30, 130, 4,
		35, 110, 4,
		30, 130, 4,
		30, 110, 4,

		35, 100, 4,
		35, 80, 4,
		30, 100, 4,
		35, 80, 4,
		30, 100, 4,
		30, 80, 4,
		
		35, 70, 4,
		35, 50, 4,
		30, 70, 4,
		35, 50, 4,
		30, 70, 4,
		30, 50, 4,
		
		35, 40, 4,
		35, 20, 4,
		30, 40, 4,
		35, 20, 4,
		30, 40, 4,
		30, 20, 4,
		
		35, 10, 4,
		35, -10, 4,
		30, 10, 4,
		35, -10, 4,
		30, 10, 4,
		30, -10, 4,
		
		35, -20, 4,
		35, -40, 4,
		30, -20, 4,
		35, -40, 4,
		30, -20, 4,
		30, -40, 4,
		
		35, -50, 4,
		35, -70, 4,
		30, -50, 4,
		35, -70, 4,
		30, -50, 4,
		30, -70, 4,
		
		35, -80, 4,
		35, -100, 4,
		30, -80, 4,
		35, -100, 4,
		30, -80, 4,
		30, -100, 4,
		
		35, -110, 4,
		35, -130, 4,
		30, -110, 4,
		35, -130, 4,
		30, -110, 4,
		30, -130, 4,
		// 4
		100, 130, 4,
		100, 110, 4,
		95, 130, 4,
		100, 110, 4,
		95, 130, 4,
		95, 110, 4,

		100, 100, 4,
		100, 80, 4,
		95, 100, 4,
		100, 80, 4,
		95, 100, 4,
		95, 80, 4,
		
		100, 70, 4,
		100, 50, 4,
		95, 70, 4,
		100, 50, 4,
		95, 70, 4,
		95, 50, 4,
		
		100, 40, 4,
		100, 20, 4,
		95, 40, 4,
		100, 20, 4,
		95, 40, 4,
		95, 20, 4,
		
		100, 10, 4,
		100, -10, 4,
		95, 10, 4,
		100, -10, 4,
		95, 10, 4,
		95, -10, 4,
		
		100, -20, 4,
		100, -40, 4,
		95, -20, 4,
		100, -40, 4,
		95, -20, 4,
		95, -40, 4,
		
		100, -50, 4,
		100, -70, 4,
		95, -50, 4,
		100, -70, 4,
		95, -50, 4,
		95, -70, 4,
		
		100, -80, 4,
		100, -100, 4,
		95, -80, 4,
		100, -100, 4,
		95, -80, 4,
		95, -100, 4,
		
		100, -110, 4,
		100, -130, 4,
		95, -110, 4,
		100, -130, 4,
		95, -110, 4,
		95, -130, 4,
	};
	
	static const GLfloat color_road_lines[] = { 
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,

		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,

		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,

		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,
		1, 1, 0,

	};

	static const GLfloat vertex_marea_glx[] = {
		// Roda direita dianteira
		-50, -35, 1,
		-45, -35, 1,
		-50, -45, 1,
		-45, -35, 1,
		-50, -45, 1,
		-45, -45, 1,
		// // Roda esquerda dianteira
		-85, -35, 1,
		-80, -35, 1,
		-85, -45, 1,
		-80, -35, 1,
		-85, -45, 1,
		-80, -45, 1,
		// Roda direita traseira
		-50, -75, 1,
		-45, -75, 1,
		-50, -85, 1,
		-45, -75, 1,
		-50, -85, 1,
		-45, -85, 1,
		// Roda esquerda traseira
		-85, -75, 1,
		-80, -75, 1,
		-85, -85, 1,
		-80, -75, 1,
		-85, -85, 1,
		-80, -85, 1,

		// BASE
		-50, -30, 1,
		-80, -30, 1,
		-50, -90, 1,
		-80, -30, 1,
		-50, -90, 1,
		-80, -90, 1,
	};

	static const GLfloat color_marea_glx[] = {
		1
	};

	static const GLfloat color_car_2[] = {
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1
	};

	GLuint vertexbuffer;
	glGenBuffers	(1, &vertexbuffer);
	
	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);

	GLuint MatrixID = glGetUniformLocation(programID, "MatrizCombinada");
	glm::mat3 mMove_road_lines = glm::mat3(1.0f);
	glm::mat3 mMove_marea_glx = glm::mat3(1.0f);
	glm::mat3 mMove_car_2 = glm::mat3(1.0f);

	move_car_2[1][2] = 2;
	randPositionCar2();
	do{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);
		configLayout(vertexbuffer, colorbuffer);

		// TRANSFORMAÇÕES
		glfwSetKeyCallback(window, KeyboardMovimentObject);

		// MOVE ROAD LINE
		if(move_roud_lines[1][2] > -0.3){
			move_roud_lines[1][2] -= velocidade;
		}else{
			move_roud_lines[1][2] = -velocidade;
		}
		
		// MOVE CAR 2
		if(move_car_2[1][2] > -1){
			move_car_2[1][2] += -velocidade;
		}else{
			move_car_2[1][2] = 2;
			randPositionCar2();
		}
		// BATIDA
		if(
			mMove_car_2[0][2] == mMove_marea_glx[0][2] &&
			mMove_car_2[1][2] <= mMove_marea_glx[1][2] + 0.6
		){
			move_car_2[1][2] += 0.1;
		}

		// ROAD LINES
		mMove_road_lines = move_roud_lines;
		glUniformMatrix3fv(MatrixID, 1, GL_TRUE, &mMove_road_lines[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(color_road_lines), color_road_lines, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_road_lines), vertex_road_lines, GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertex_road_lines)); 

		// CARRO 1
		mMove_marea_glx = move_marea_glx;
		glUniformMatrix3fv(MatrixID, 1, GL_TRUE, &mMove_marea_glx[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(color_marea_glx), color_marea_glx, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_marea_glx), vertex_marea_glx, GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertex_marea_glx)); 
		
		// CARRO 2
		mMove_car_2 = move_car_2;
		glUniformMatrix3fv(MatrixID, 1, GL_TRUE, &mMove_car_2[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(color_car_2), color_car_2, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_marea_glx), vertex_marea_glx, GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertex_marea_glx)); 

		// ENT OBJETOS ==================================================================================
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glfwSwapBuffers(window);
		glfwPollEvents();

	} 
	while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS
		&& glfwWindowShouldClose(window) == 0);

	destroyWindows(vertexbuffer, colorbuffer, VertexArrayID, programID);

	return 0;
}

