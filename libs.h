#pragma once

/*	=====	Library(Include)	=====	*/
//Standard Library
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <conio.h>
#include <chrono>
#include <random>
#include <array>
#include <algorithm>

//GLFW and GLEW Library
#include <glew.h>
#include <glfw3.h>

//GLM Library
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//SOIL2 Library
#include <SOIL2.h>


/*	=====	Class	=====	*/
class Shader {
	private : 
		std::string readFile(char& type);
		unsigned int compileShader(char type);
	public : 
		//shader
		void loadShader(unsigned int* core_program);
};

class Matrix {
	protected :
		//Model, Projection, View Matrix
		void Translate(glm::mat4* model, glm::vec3 coord);
		void Rotate(glm::mat4* model, float degrees, glm::vec3 coord);
		void Scale(glm::mat4* model, glm::vec3 coord);
	public :
		void matrixLocation(const char* name, glm::mat4 model);
		unsigned int core_program;
};

class Draw: public Matrix {
	private: 
		void makeFloor(unsigned int VAO, float movementS);
		void makeBox(unsigned int VAO);
	public :
		unsigned int dinoTexture[16];
		void drawDino(unsigned int VAO);

		unsigned int floorTexture;
		void drawFloor(unsigned int VAO);

		unsigned int boxTexture;
		void drawBox(unsigned int VAO);
		void draw2Box(unsigned int VAO);
		void draw3Box(unsigned int VAO);

		unsigned int healthBar_Texture[10];
		void drawHealth_bar(unsigned int VAO);

		unsigned int gameOver_Texture;
		void draw_gameOver(unsigned int VAO);
};


/*	=====	Functions	=====	*/
//create window
void createWindow(GLFWwindow** window, int* frameBuffer_w, int* frameBuffer_h);

//frame buffer
void frameBuffer(GLFWwindow* window, int w, int h);

//VAO, VBO, EBO
unsigned int VAO_VBO_EBO(float* vertices, int verSize);

//Texture
unsigned int loadTexture(std::string filename);


/*	=====	Vertex and Index	=====	*/
float DinoVertices[] = {
	//position				//texcoord

	-0.4, 0.3f, -0.1f,		0.1f, 0.8f,		//0
	-0.4f, -0.25f, -0.1f,		0.1f, 0.21,		//1
	0.25f, -0.25f, -0.1f,		0.75f, 0.21f,	//2
	0.48f, 0.3f, -0.1f,		0.98f, 0.8f,	//3
};
unsigned int DinoIndeces[] = {
	0, 1, 2,
	0, 2, 3,
};

float FloorVertices[] = {
	//position				//texcoord

	-0.5f, 0.5f, 0.f,		0.f, 1.f,
	-0.5f, -0.5f, 0.f,		0.f, 0.f,		
	0.5f, -0.5f, 0.f,		1.f, 0.f,
	0.5f, 0.5f, 0.f,		1.f, 1.f
};
unsigned int FloorIndeces[] = {
	0, 1, 2,
	0, 2, 3
};


/*	=====	Variable	=====	*/
//Window's Icon
int icon_w = 0;
int icon_h = 0;
GLFWimage icon[1];

//Frames' file name
std::string name;

//Animation's speed
int framesPassed = 0, arrayAt, speed = 2;

//Time Passed
int t = 0;

//Movement speed
float currentFrame = 0, movementSpeed = 3;
float movement = 0, deltaTime = 0, lastFrame = 0;
float movement2 = -10.f, lastFrameS = 0;

//Dino Jump
glm::vec3 DinoPosition(0.f, 0.0f, 0.f);
int timeJump = 0, jumpTime = 0;
float gravity = 10.f + DinoPosition.y;
float jumpMax = 11.f + DinoPosition.y;
float v_decrement = gravity / jumpMax;

//Box Move
glm::vec3 BoxPosition(0.5f, -0.05f, 0.f);
std::vector<float> temp_boxPosition;
float s = 10.f;
std::vector<int> chancePos;

//Generate Random Box
int chance = 1, one = 0;
float currPosition = 0.f, lastPosition = 0.f;
int first = 1;

//Health
int health = 9;

//Game Over
float w_PosGameOver = 0;
float h_PosGameOver = 0;
