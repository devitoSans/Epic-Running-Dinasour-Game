#include "libs.h"

int main() {
	//Make a window		====
	glfwInit();
	GLFWwindow* window;
	int frameBuffer_w = 0, frameBuffer_h = 0;
	createWindow(&window, &frameBuffer_w, &frameBuffer_h);

	//shader		====
	Shader programShader;
	unsigned int core_program;

	core_program = glCreateProgram();
	programShader.loadShader(&core_program);

	//VAO, VBO, EBO		====
	unsigned int VAO[2];
	VAO[0] = VAO_VBO_EBO(DinoVertices, sizeof(DinoVertices));
	VAO[1] = VAO_VBO_EBO(FloorVertices, sizeof(FloorVertices));

	//Texture		====
	//dinosaur's textture
	Draw dino;
	dino.core_program = core_program;
	int frames = sizeof(dino.dinoTexture) / 4;
	for (int i = 0; i < frames; i++) {
		std::stringstream nameStream;
		nameStream << i;
		if (i < 10) name = "running_dinasour/frame_0" + nameStream.str() + "_delay-0.04s.gif";
		else name = "running_dinasour/frame_" + nameStream.str() + "_delay-0.04s.gif";

		dino.dinoTexture[i] = loadTexture(name);
	}

	//floor's texture
	Draw floor;
	floor.core_program = core_program;
	floor.floorTexture = loadTexture("background.png");

	//box's texture
	Draw box;
	box.core_program = core_program;
	box.boxTexture = loadTexture("boxibox.jpg");

	//Health Bar's Texture
	Draw healthBar;
	healthBar.core_program = core_program;
	frames = sizeof(healthBar.healthBar_Texture) / 4;
	for (int i = 0; i < frames; i++) {
		std::stringstream nameStream;
		nameStream << i;
		name = "HealthBar/Health-" + nameStream.str() + ".png";
		healthBar.healthBar_Texture[i] = loadTexture(name);
	}

	//Game Over's texture
	Draw gameOver;
	gameOver.core_program = core_program;
	gameOver.gameOver_Texture = loadTexture("game_over.jpeg");

	//Model, Projection, Camera		====
	Matrix model;
	model.core_program = core_program;	//-> model
	float aspect = (float)frameBuffer_w / frameBuffer_h; //-> projection
	glm::vec3 cameraPos(2.f, 0.99f, 4.5f), cameraFront(0.f, 0.f, -1.f);	//-> camera

	//Main Loop		====
	while (!glfwWindowShouldClose(window)) {
		//IN GAME
		if (health > 0) {
			//RENDER
			glClearColor(1.f, 1.f, 1.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//DRAW
			glUseProgram(core_program);
			//draw the dino
			dino.drawDino(VAO[0]);
			//draw the floor
			floor.drawFloor(VAO[1]);
			//draw the box
			box.drawBox(VAO[1]);
			//draw the health bar
			healthBar.drawHealth_bar(VAO[1]);

			//PROJECTION AND CAMERA
			glm::mat4 projection(1.f);	//-> Projection
			glfwGetFramebufferSize(window, &frameBuffer_w, &frameBuffer_h);
			if (frameBuffer_w <= 0 && frameBuffer_h <= 0) {
				frameBuffer_w = 640;
				frameBuffer_h = 480;
			}

			aspect = frameBuffer_w / float(frameBuffer_h);
			projection = glm::perspective(glm::radians(45.f), aspect, 0.1f, 100.f);
			model.matrixLocation("projection", projection);
		 
			glm::mat4 camera(1.f);	//-> Camera
			camera = glm::lookAt(cameraPos, cameraPos + cameraFront, glm::vec3(0.f, 1.f, 0.f));
			model.matrixLocation("camera", camera);

			//MAKE MORE FASTER
			if (t != 0)
				if ((t % 10) == 0) movementSpeed += 0.00972f; 
			if (movementSpeed >= 10.f) movementSpeed = 10.f;

			//INPUT
			glfwPollEvents();
			//Esc -> for exit
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, 1);
			//Space -> for jumping
			if (DinoPosition.y > 0.f && jumpTime >= jumpMax) {
				DinoPosition.y -= gravity * deltaTime;
				gravity += v_decrement - 0.2f;
				if (DinoPosition.y < 0.05f) {
					DinoPosition.y = 0.f;
					jumpTime = 0;
					timeJump = 0;
					gravity = 10.f;
				}
			}
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || timeJump == 1) {
				if (jumpTime <= jumpMax) {
					DinoPosition.y += gravity * deltaTime;
					timeJump = 1;
					gravity -= v_decrement;
					jumpTime++;
				}
			}
		}

		//GAME OVER
		if (health <= 0) {
			//RENDER GAME OVER
			glClearColor(1.f, 1.f, 1.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//DRAW GAME OVER
			glUseProgram(core_program);
			gameOver.draw_gameOver(VAO[1]);

			//PROJECTION AND CAMERA
			glm::mat4 projection(1.f);	//-> Projection
			glfwGetFramebufferSize(window, &frameBuffer_w, &frameBuffer_h);
			if (frameBuffer_w <= 0 && frameBuffer_h <= 0) {
				frameBuffer_w = 640;
				frameBuffer_h = 480;
			}

			aspect = frameBuffer_w / float(frameBuffer_h);
			projection = glm::perspective(glm::radians(45.f), aspect, 0.1f, 100.f);
			model.matrixLocation("projection", projection);

			glm::mat4 camera(1.f);	//-> Camera
			camera = glm::lookAt(cameraPos, cameraPos + cameraFront, glm::vec3(0.f, 1.f, 0.f));
			model.matrixLocation("camera", camera);

			//INPUT
			glfwPollEvents();
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, 1);
		}

		//TIME PASSED
		t = glfwGetTime();

		//SWAP BUFFERS
		framesPassed++;
		if (framesPassed >= frames * speed) framesPassed = 0;
		glfwSwapBuffers(window);
		glFlush();
		glfwSwapInterval(1);
	}

	glDeleteTextures(sizeof(dino.dinoTexture) / 4, dino.dinoTexture);
	glDeleteProgram(core_program);
	glfwTerminate();
	return 0;
}
/*	=====	Functions	=====	*/

//create window
void createWindow(GLFWwindow** window, int* frameBuffer_w, int* frameBuffer_h) {
	//create window
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWmonitor* fullscreen = glfwGetPrimaryMonitor();		//-> fullscreen

	const GLFWvidmode* mode = glfwGetVideoMode(fullscreen);		//get the screen length
	*window = glfwCreateWindow(640, 480, "Epic Running Dinosaur", NULL, NULL);

	//set icon window
	icon[0].pixels = SOIL_load_image("image2/Epic_Dinosaur.jpg", &icon_w, &icon_h, NULL, SOIL_LOAD_RGBA);
	icon[0].width = icon_w;
	icon[0].height = icon_h;
	glfwSetWindowIcon(*window, 1, icon);

	//frame buffer
	glfwGetFramebufferSize(*window, frameBuffer_w, frameBuffer_h);
	glfwSetFramebufferSizeCallback(*window, frameBuffer);

	glfwMakeContextCurrent(*window);

	glewExperimental = GLU_TRUE;
	glewInit();

	//openGL mode
	glEnable(GL_DEPTH_TEST);
}

//frame buffer
void frameBuffer(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}

//VAO, VBO, EBO
unsigned int VAO_VBO_EBO(float* vertices, int verSize) {
	unsigned int VAO, VBO;

	//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verSize, vertices, GL_STATIC_DRAW);

	//Enabling VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Unbinding
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}

//load texture
unsigned int loadTexture(std::string filename) {
	unsigned int texture;
	std::string file_image = "image2/" + filename;
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	int image_w, image_h;

	unsigned char* image = SOIL_load_image(file_image.c_str(), &image_w, &image_h, NULL, SOIL_LOAD_RGBA);

	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else std::cout << "FAILED_TO_LOAD_IMAGE\n";

	SOIL_free_image_data(image);

	return texture;
}

//HitBox and Positive Value
void Hit_Box() {
	//POSITIVE VALUE
	if (BoxPosition.x < 0.f) BoxPosition.x *= -1;
	float HitBox_x = DinoPosition.x + BoxPosition.x;
	float HitBox_y = DinoPosition.y + BoxPosition.y;

	//HIT BOX
	if (HitBox_y < 0.25f && HitBox_x < 0.35f) health--;
}

//Generate Random Box
int random(int maxInput) {
	std::random_device rd;
	std::mt19937::result_type p = rd() ^ (
		(std::mt19937::result_type)
		std::chrono::duration_cast<std::chrono::seconds>(
			std::chrono::system_clock::now().time_since_epoch()
			).count() +
		(std::mt19937::result_type)
		std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::high_resolution_clock::now().time_since_epoch()
			).count());

	std::mt19937 gen(p);

	for (int i = 0; i < 10; i++) {
		int OutPutRnd = 0, inputTemp = 0;
		std::mt19937::result_type n;
		while ((n = gen()) > std::mt19937::max() -
			(std::mt19937::max() - 5) % 6) {
		}
		OutPutRnd = n % maxInput + 1;
		if (inputTemp == OutPutRnd) continue;
		inputTemp = n % maxInput + 1;
		if (i == 9) return OutPutRnd = n % maxInput + 1;
	}
	return 0;
}

/*	=====   Class Functions   =====	*/

//Draw
//dino
void Draw::drawDino(unsigned int VAO) {
	//animation's speed
	arrayAt = framesPassed / speed;

	//model, projection, view matrix
	glm::mat4 model(1.f);
	Translate(&model, DinoPosition);
	matrixLocation("model", model);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(core_program, "texSampler"), 0);
	glBindTexture(GL_TEXTURE_2D, dinoTexture[arrayAt % (sizeof(dinoTexture) / 4)]);
	glDrawElements(GL_TRIANGLE_FAN, sizeof(DinoIndeces) / 4, GL_UNSIGNED_INT, DinoIndeces);
}
//floor
void Draw::drawFloor(unsigned int VAO) {
	//movement speed
	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	makeFloor(VAO, movement);
	movement += movementSpeed * deltaTime;

	makeFloor(VAO, movement2);
	movement2 += movementSpeed * deltaTime;

	if (movement > 10.f) movement = -10.f;
	if (movement2 > 10.f) movement2 = -10.f;
}
void Draw::makeFloor(unsigned int VAO, float movementS) {

	//model, projection, view matrix
	glm::mat4 model(1.f);
	Translate(&model, glm::vec3(-movementS, -0.75f, 0.f));
	Scale(&model, glm::vec3(12.f, 1.f, 1.f));
	matrixLocation("model", model);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(core_program, "texSampler"), 0);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	glDrawElements(GL_TRIANGLE_FAN, sizeof(FloorIndeces) / 4, GL_UNSIGNED_INT, FloorIndeces);
}
//box
void Draw::drawBox(unsigned int VAO) {
	if (first == 1) temp_boxPosition.push_back(-11.f);
	if (chance == 0) {
		chance = 1;
		temp_boxPosition.clear();
		temp_boxPosition.push_back(-11.f);
	}
	float maxV = *std::max_element(temp_boxPosition.begin(), temp_boxPosition.end());

	if (maxV < -10.f || first == 1) {
		chance = random(10) + 5;
		chancePos.clear();
		temp_boxPosition.clear();

		if (chance != 0) {
			for (int i = 0; i < chance; i++) {
				backRandom : 
				currPosition = random(5) + i * 3;

				if (currPosition == lastPosition) goto backRandom;
				chancePos.push_back(currPosition);
				lastPosition = currPosition;
			}

			for (int i = 0; i < chance; i++) {
				temp_boxPosition.push_back(10.f);
			}
		}

		first = 0;
		s = 10.f;
	}

	if (chance != 0) {
		s -= movementSpeed * deltaTime;
		for (int i = 0; i < chance; i++) {
			if (temp_boxPosition[i] >= -10.f) {
				glm::mat4 model(1.f);
				
				temp_boxPosition[i] = s + chancePos[i];
				BoxPosition.x = temp_boxPosition[i];

				Translate(&model, BoxPosition);
				Scale(&model, glm::vec3(0.25f, 0.40f, 1.f));
				matrixLocation("model", model);

				makeBox(VAO);
				Hit_Box();
			}
		}
	}
}
void Draw::draw2Box(unsigned int VAO) {
	glm::mat4 model(1.f);
	BoxPosition.x = -movement - 2.f;

	Translate(&model, BoxPosition);
	Scale(&model, glm::vec3(0.25f, 0.40f, 1.f));
	matrixLocation("model", model);

	makeBox(VAO);
	Hit_Box();

	model = glm::mat4(1.f);
	BoxPosition.x = -movement - 2.25f;

	Translate(&model, BoxPosition);
	Scale(&model, glm::vec3(0.25f, 0.40f, 1.f));
	matrixLocation("model", model);

	makeBox(VAO);
	Hit_Box();
}
void Draw::draw3Box(unsigned int VAO) {
	glm::mat4 model(1.f);
	BoxPosition.x = -movement - 2.f;
	Translate(&model, BoxPosition);
	Scale(&model, glm::vec3(0.25f, 0.40f, 1.f));
	matrixLocation("model", model);
	makeBox(VAO);
	Hit_Box();

	model = glm::mat4(1.f);
	BoxPosition.x = -movement - 2.25f;
	Translate(&model, BoxPosition);
	Scale(&model, glm::vec3(0.25f, 0.40f, 1.f));
	matrixLocation("model", model);
	makeBox(VAO);
	Hit_Box();

	model = glm::mat4(1.f);
	BoxPosition.x = -movement - 2.5f;
	Translate(&model, BoxPosition);
	Scale(&model, glm::vec3(0.25f, 0.40f, 1.f));
	matrixLocation("model", model);
	makeBox(VAO);
	Hit_Box();
}
void Draw::makeBox(unsigned int VAO) {
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(core_program, "texSampler"), 0);
	glBindTexture(GL_TEXTURE_2D, boxTexture);
	glDrawElements(GL_TRIANGLES, sizeof(FloorIndeces), GL_UNSIGNED_INT, FloorIndeces);
}
//health bar
void Draw::drawHealth_bar(unsigned int VAO) {
	glm::mat4 model(1.f);
	Translate(&model, glm::vec3(1.5f, 2.3f, 0.f));
	Scale(&model, glm::vec3(4.f, 0.25f, 1.f));
	matrixLocation("model", model);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(core_program, "texSampler"), 0);
	glBindTexture(GL_TEXTURE_2D, healthBar_Texture[health]);
	glDrawElements(GL_TRIANGLES, sizeof(FloorIndeces), GL_UNSIGNED_INT, FloorIndeces);
}
//game over
void Draw::draw_gameOver(unsigned int VAO) {
	glm::mat4 model(1.f);
	Translate(&model, glm::vec3(1.9f, 1.f, 0.01f));
	Scale(&model, glm::vec3(2.f, 2.f, 1.f));
	matrixLocation("model", model);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(core_program, "model"), 0);
	glBindTexture(GL_TEXTURE_2D, gameOver_Texture);
	glDrawElements(GL_TRIANGLES, sizeof(FloorIndeces), GL_UNSIGNED_INT, FloorIndeces);
}

//Matrix
void Matrix::Translate(glm::mat4* model, glm::vec3 coord) {
	*model = glm::translate(*model, coord);
}
void Matrix::Rotate(glm::mat4* model, float degrees, glm::vec3 coord) {
	*model = glm::rotate(*model, glm::radians(degrees), coord);
}
void Matrix::Scale(glm::mat4* model, glm::vec3 coord) {
	*model = glm::scale(*model, coord);
}
void Matrix::matrixLocation(const char* name, glm::mat4 model) {
	glUniformMatrix4fv(glGetUniformLocation(core_program, name), 1, GL_FALSE, glm::value_ptr(model));
}

//shader
std::string Shader::readFile(char& type) {
	std::ifstream fileRead;
	std::string src, temp;

	if (type == 'v') fileRead.open("vertex.glsl");
	else fileRead.open("fragment.glsl");

	while (!fileRead.eof()) {
		std::getline(fileRead, temp);
		src.append("\n" + temp);
	}

	fileRead.close();
	return src;
}
unsigned int Shader::compileShader(char type) {
	std::string src;
	const char* srcChar;
	unsigned int shader;
	int success;
	char infoLog[512];

	src = readFile(type);
	srcChar = src.c_str();

	if (type == 'v') shader = glCreateShader(GL_VERTEX_SHADER);
	else shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(shader, 1, &srcChar, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << infoLog << std::endl;
		std::cout << "ERROR::MAIN.CPP::FAILED_TO_COMPILED_TYPE: " << type << std::endl;
	}

	return shader;
}
void Shader::loadShader(unsigned int* core_program) {
	unsigned int vertexShader = compileShader('v');
	unsigned int fragmentShader = compileShader('f');

	glAttachShader(*core_program, vertexShader);
	glAttachShader(*core_program, fragmentShader);

	glLinkProgram(*core_program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}