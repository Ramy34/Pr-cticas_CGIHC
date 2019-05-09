/*---------------------------------------------------------*/
/* ----------------   Práctica 3 --------------------------*/
/*-----------------    2019-2   ---------------------------*/
/*----------------- Ramsés Castro Molano ------------------*/
#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void display(void);
void getResolution(void);

//Cambiar colores
float colorR = 0.0f,
colorG = 0.0f,
colorB = 0.0f;


//For Keyboard
float movX = 0.0f,
	movY = 10.0f,
	movZ = -50.0f;


void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{	
		GLfloat vertices[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V5
		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V4

		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V7
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	//V3
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V0

		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V7

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V1
	};

	unsigned int indices[] =	//I am not using index for this session
	{
		0
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void display(void)
{

	glm::vec3 colorNaran = glm::vec3(colorR, colorG, colorB);

	//Shader myShader("shaders/shader.vs", "shaders/shader.fs");
	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");

	projectionShader.use();

	// create transformations and Projection
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	
	//Use "projection" in order to change how we see the information
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	//projection = glm::ortho(-5.0f, 5.0f, -3.0f, 3.0f, 0.1f, 10.0f);
	
	//Use "view" in order to affect all models
	//view = glm::rotate(view, 45, glm::vec3(movX, movY, movZ));
	view = glm::translate(view, glm::vec3(movX, movY, movZ));
	// pass them to the shaders
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);

	glBindVertexArray(VAO);
	//------------------------- Primer renglón --------------------------------------
	for (int i = 0; i < 5; i++) { 
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.0f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube 
	}
	//------------------------- Segundo renglón --------------------------------------
	model = glm::translate(model, glm::vec3(-7.0f, -1.0f, 0.0f)); 
	for (int i = 0; i < 4; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.0f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube
		if (i == 1) {
			model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
		}
	}
	model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 0.0f)); 
	for (int i = 0; i < 5; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f)); //Amarillo
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube 
	}
	//------------------------- Tercer renglón --------------------------------------
	model = glm::translate(model, glm::vec3(-8.0f, -1.0f, 0.0f));
	for (int i = 0; i < 2; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.0f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube
		if (i == 0) {
			model = glm::translate(model, glm::vec3(9.0f, 0.0f, 0.0f));
		}
	}
	model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f));
	for (int i = 0; i < 9; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f)); //Amarillo
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube 
	}
	//------------------------- Cuarto renglón --------------------------------------
	model = glm::translate(model, glm::vec3(-10.0f, -1.0f, 0.0f)); 
	for (int i = 0; i < 8; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f)); //Rojo
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube
		if (i == 1 || i==3 || i==5) {
			model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		}
	}

	model = glm::translate(model, glm::vec3(-9.0f, 0.0f, 0.0f));
	for (int i = 0; i < 3; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f)); //Amarillo
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube 
		if (i == 0 || i == 1) {
			model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		}
	}
	//------------------------- Quinto renglón --------------------------------------
	model = glm::translate(model, glm::vec3(-10.0f, -1.0f, 0.0f)); 
	for (int i = 0; i < 2; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.0f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube
		if (i==0) {
			model = glm::translate(model, glm::vec3(11.0f, 0.0f, 0.0f));
		}
	}
	
	model = glm::translate(model, glm::vec3(-12.0f, 0.0f, 0.0f)); 
	for (int i = 0; i < 10; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f)); //Rojo
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube
		if (i == 4) {
			model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		}
	}
	model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 0.0f)); 
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f)); //Amarillo
	glDrawArrays(GL_QUADS, 0, 24);
	//------------------------- Sexto renglón --------------------------------------
	model = glm::translate(model, glm::vec3(-7.0f, -1.0f, 0.0f)); 
	for (int i = 0; i < 2; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.0f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube
		if (i == 0) {
			model = glm::translate(model, glm::vec3(11.0f, 0.0f, 0.0f));
		}
	}

	model = glm::translate(model, glm::vec3(-12.0f, 0.0f, 0.0f)); 
	for (int i = 0; i < 10; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f)); //Rojo
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube
		if (i == 4) {
			model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		}
	}
	model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 0.0f)); 
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f)); //Amarillo
	glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube
	//------------------------- Séptimo renglón --------------------------------------
	model = glm::translate(model, glm::vec3(-7.0f, -1.0f, 0.0f));
	for (int i = 0; i < 2; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.0f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube
		if (i == 0) {
			model = glm::translate(model, glm::vec3(11.0f, 0.0f, 0.0f));
		}
	}

	model = glm::translate(model, glm::vec3(-12.0f, -0.0f, 0.0f));
	for (int i = 0; i < 5; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f)); //Amarillo
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube 
		if (i == 0 || i == 3) {
			model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
		}
	}

	model = glm::translate(model, glm::vec3(-10.0f, -0.0f, 0.0f));
	for (int i = 0; i < 6; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f)); //Rojo
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube 
		if (i == 2) {
			model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
		}
	}
	//------------------------- Octavo renglón --------------------------------------
	model = glm::translate(model, glm::vec3(-11.0f, -1.0f, 0.0f));
	for (int i = 0; i < 2; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.0f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube
		if (i == 0) {
			model = glm::translate(model, glm::vec3(11.0f, 0.0f, 0.0f));
		}
	}

	model = glm::translate(model, glm::vec3(-12.0f, -0.0f, 0.0f));
	for (int i = 0; i < 9; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f)); //Amarillo
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube 
		if (i == 1 || i == 6) {
			model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		}
	}

	model = glm::translate(model, glm::vec3(-9.0f, -0.0f, 0.0f));
	for (int i = 0; i < 2; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f)); //Rojo
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube 
		if (i == 0) {
			model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
		}
	}
	//------------------------- Noveno renglón --------------------------------------
	model = glm::translate(model, glm::vec3(-10.0f, -1.0f, 0.0f));
	for (int i = 0; i < 2; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.0f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube
		if (i == 0) {
			model = glm::translate(model, glm::vec3(11.0f, 0.0f, 0.0f));
		}
	}

	model = glm::translate(model, glm::vec3(-12.0f, -0.0f, 0.0f));
	for (int i = 0; i < 11; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f)); //Amarillo
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube 
	}
	//------------------------- Décimo renglón --------------------------------------
	model = glm::translate(model, glm::vec3(-11.0f, -1.0f, 0.0f));
	for (int i = 0; i < 2; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.0f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube
		if (i == 0) {
			model = glm::translate(model, glm::vec3(9.0f, 0.0f, 0.0f));
		}
	}

	model = glm::translate(model, glm::vec3(-10.0f, -0.0f, 0.0f));
	for (int i = 0; i < 7; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f)); //Amarillo
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube 
		if (i ==1 || i == 4) {
			model = glm::translate(model, glm::vec3(1.0f, -0.0f, 0.0f));
		}
	}

	model = glm::translate(model, glm::vec3(-7.0f, -0.0f, 0.0f));
	for (int i = 0; i < 2; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(0.46f, 0.23f, 0.16f)); //Café
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube 
		if (i == 0) {
			model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
		}
	}
	//------------------------- Undécima renglón --------------------------------------
	model = glm::translate(model, glm::vec3(-8.0f, -1.0f, 0.0f));
	for (int i = 0; i < 2; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.0f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube
		if (i == 0) {
			model = glm::translate(model, glm::vec3(9.0f, 0.0f, 0.0f));
		}
	}

	model = glm::translate(model, glm::vec3(-10.0f, -0.0f, 0.0f));
	for (int i = 0; i < 6; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f)); //Amarillo
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube 
		if (i == 2) {
			model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
		}
	}

	model = glm::translate(model, glm::vec3(-6.0f, -0.0f, 0.0f));
	for (int i = 0; i < 3; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(0.46f, 0.23f, 0.16f)); //Café
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube 
	}
	//------------------------- Duodécimo renglón --------------------------------------
	model = glm::translate(model, glm::vec3(-6.0f, -1.0f, 0.0f));
	for (int i = 0; i < 4; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.0f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube
		if (i == 1) {
			model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
		}
	}
	model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 0.0f));
	for (int i = 0; i < 5; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f)); //Amarillo
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube 
	}
	//------------------------- Treceavo renglón --------------------------------------
	model = glm::translate(model, glm::vec3(-5.0f, -1.0f, 0.0f));
	for (int i = 0; i < 5; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.0f)); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube 
	}

	//---------------------prueba----------------
	model = glm::translate(model, glm::vec3(-5.0f, -1.0f, 0.0f));
	for (int i = 0; i < 5; i++) {
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", colorNaran); //Naranja
		glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube 
	}

	/*projectionShader.setVec3("aColor", glm::vec3(1.0f,0.0f,0.0f));
	glDrawArrays(GL_QUADS, 0, 24); //A lonely cube :(

	model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.5f, 0.8f, 0.3f));
	glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube 

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 2.3f, 0.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube 

	model = glm::translate(model, glm::vec3(1.75f, -4.8f, 0.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 0.5f, 0.5f));
	glDrawArrays(GL_QUADS, 0, 24); //A no so lonely cube */

	glBindVertexArray(0);

}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 3", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();
	glEnable(GL_DEPTH_TEST);

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Backgound color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi funci�n de dibujo
		display();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //Mueve a la derecha
		movX += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //Mueve a la izquierda
		movX -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) //Mueve a arriba
		movY += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) //Mueve abajo
		movY -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //Rota z positivo
		movZ += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //Rota z negativo
		movZ -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) //Cambia el color azul +
		colorB += 0.05f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) //Cambia el color azul -
		colorB -= 0.05f;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) //Cambia el color rojo +
		colorR += 0.05f;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) //Cambia el color rojo -
		colorR -= 0.05f;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) //Cambia el color verde +
		colorG += 0.05f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) //Cambia el color verde -
		colorG -= 0.05f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}