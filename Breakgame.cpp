#include <bits/stdc++.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include </usr/include/glm/gtc/matrix_transform.hpp>
using namespace glm;
#include "common/shader.hpp"

const GLint WIDTH = 800, HEIGHT = 800;
const GLfloat R = 0.15, G = 0.15, B = 0.15, A = 0;

GLFWwindow* window;
GLuint colorbuffer, vertexbuffer;

glm::mat3 escala = glm::mat3(1.0f);

int initWindow(){
	if(!glfwInit()){
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3.3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3.3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

 	window = glfwCreateWindow( WIDTH, HEIGHT, "Breakgame", NULL, NULL);
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
	return 0;
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

std::vector<glm::vec3> colorModel(int tam, float R, float G, float B) {
	int i;
	std::vector<glm::vec3> vertexColor;

	for(i=0;i<tam;i++){
		glm::vec3 color;
		color.r = R;
		color.g = G;
		color.b = B;
		vertexColor.push_back(color); 
	}	
	return vertexColor;
}

std::vector<glm::vec2> loadModel(const char *path){
	FILE * file = fopen(path, "r");
	std::vector<glm::vec2> vertices;
	if(file == NULL){
		printf("Impossible to open the file!\n");
		fclose(file);
		getchar();
		return vertices;
	}
	
	while (1) {
		glm::vec2 vertex;
		int res = fscanf(file, "%f %f\n", &vertex.x, &vertex.y);
		if (res == EOF) {
			break;
		}
		vertices.push_back(vertex);
	}
	fclose(file);
	return vertices;
}

void drawModel(std::vector<glm::vec2> vertices, glm::mat3 MatrizCombinada, GLuint MatrixID, GLfloat R, GLfloat G, GLfloat B) {
	
	glUniformMatrix3fv(MatrixID, 1, GL_TRUE, &MatrizCombinada[0][0]);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	std::vector<glm::vec3> color = colorModel(vertices.size(), R, G, B);
	glBufferData(GL_ARRAY_BUFFER, color.size()* sizeof(glm::vec3), &color[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()* sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	
}	

void MouseKeyboardMovimentObject(double deltaTime){

}

int main( void ){
 	initWindow();

	GLuint VertexArrayID;		
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint programID = LoadShaders("shaders/vertexShader.vertexshader", "shaders/fragmentShader.fragmentshader");
	std::vector<glm::vec3> color;

	glGenBuffers(1, &vertexbuffer);	
	glGenBuffers(1, &colorbuffer);

	GLuint MatrixID = glGetUniformLocation(programID, "MatrizCombinada");
	glm::mat3 MatrizCombinada = glm::mat3(1.0f);
	
	std::vector<glm::vec2> car = loadModel("models/car.txt");

	double lastTime = glfwGetTime();
 	int nbFrames = 0;

	do{
		double currentTime = glfwGetTime();
		nbFrames++;
		double deltaTime = currentTime - lastTime;
		if (deltaTime  >= 1.0 ) {
			 nbFrames = 0;
			 lastTime += 1.0;
		}
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(programID);
		configLayout(vertexbuffer, colorbuffer);

		MouseKeyboardMovimentObject(deltaTime);

		MatrizCombinada = escala;
		drawModel(car, MatrizCombinada, MatrixID, 0, 0, 0);

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

