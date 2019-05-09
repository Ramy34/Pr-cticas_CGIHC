/*---------------------------------------------------------*/
/* ----------------   Práctica 4 --------------------------*/
/*-----------------    2019-2   ---------------------------*/
/*------------- Ramsés Castro Molano---------------*/
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

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -15.0f,
		angX = 0.0f,
		angy = 0.0f;

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
	//Shader myShader("shaders/shader.vs", "shaders/shader.fs");
	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");

	projectionShader.use();

	// create transformations and Projection
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	glm::mat4 tmp = glm::mat4(1.0f);

	//Use "projection" in order to change how we see the information
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	//projection = glm::ortho(-5.0f, 5.0f, -3.0f, 3.0f, 0.1f, 10.0f);

	//Use "view" in order to affect all models
	view = glm::translate(view, glm::vec3(movX, movY, movZ));
	view = glm::rotate(view, glm::radians(angy), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::rotate(view, glm::radians(angX), glm::vec3(1.0f, 0.0f, 0.0f));
	// pass them to the shaders
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);


	glBindVertexArray(VAO);
	//projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	//model = glm::scale(model,glm::vec3(5.0f,3.0f,4.0f));
	//projectionShader.setMat4("model", model);
	//glDrawArrays(GL_QUADS, 0, 24); //A lonel cube :(
	//---Figura---//
/*	//-----Torso------//
	model = glm::scale(model, glm::vec3(4.0f, 5.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //A lonel cube :(
	model = glm::mat4(1.0f);

	//----Cuello----//
	model = glm::translate(model, glm::vec3(0.0f, 2.75f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //A lonel cube :(
	model = tmp;
	//---Cabeza---//
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 2.5f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //A lonel cube :(
	model = glm::mat4(1.0f);
	//--Parte baja--//
	model = glm::translate(model, glm::vec3(0.0f, -3.5f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 2.0f, 1.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 0.6f, 0.8f));
	glDrawArrays(GL_QUADS, 0, 24); //A lonel cube :(
	model = glm::mat4(1.0f);
	//---Pierna derecha--//
	model = glm::translate(model, glm::vec3(1.25f, -6.5f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(1.5f, 4.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //A lonel cube :(
	model = tmp;
	//---Pierna izquierda---//
	model = glm::translate(model, glm::vec3(-2.5f, 0.0f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(1.5f, 4.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //A lonel cube :(
	model = tmp;
	//---Pie izquierdo---//
	model = glm::translate(model, glm::vec3(-0.75f, -2.5f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //A lonel cube :(
	model = tmp;
	//---Pie derecho---//
	model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //A lonel cube :(
	model = glm::mat4(1.0f);
	//---Brazo derecho---//
	model = glm::translate(model, glm::vec3(3.0f, 2.0f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //A lonel cube :(
	model = tmp;
	//--Brazo izquierdo--//
	model = glm::translate(model, glm::vec3(-6.0f, 0.0f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //A lonel cube :(
	model = tmp;
	//---Parte baja brazo izquierdo---//
	model = glm::translate(model, glm::vec3(-0.75f, -2.0f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.5f, 3.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //A lonel cube :(
	model = tmp;
	//---Parte baja brazo izquierdo---//
	model = glm::translate(model, glm::vec3(7.5f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 3.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //A lonel cube :( */

	/*model = glm::translate(model, glm::vec3(7.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24);*/
	
	//---- Hipopotamo ----/
	//-----Cuerpo----//
	model = glm::mat4(1.0f);
	tmp = model;
	model = glm::scale(model, glm::vec3(4.0f, 3.0f, 4.0f));
	projectionShader.setVec3("aColor", glm::vec3(0.45f, 0.25f, 0.3f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;
	//---Cabeza---//
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.5f));
	tmp = model;
	model = glm::scale(model, glm::vec3(3.0f, 2.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;
	//---Barbilla---//
	model = glm::translate(model, glm::vec3(0.0f, -1.25f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(3.0f, 0.5f, 1.0f));
	projectionShader.setVec3("aColor", glm::vec3(0.45f, 0.3f, 0.35f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;
	//---Trompa---/
	model = glm::translate(model, glm::vec3(0.0f, 0.75f, 0.6f));
	tmp = model;
	model = glm::scale(model, glm::vec3(2.5f, 1.0f, 0.25f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;
	//---Nariz---//
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.4f));
	tmp = model;
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 0.52f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;
	//---Fosas Nasales---//
	model = glm::translate(model, glm::vec3(1.0f, 0.375f, 0.25f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.025f));
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	model = glm::translate(model, glm::vec3(-2.0f, 0.0f, -0.1f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;
	//--Nariz parte de arriba--//
	model = glm::translate(model, glm::vec3(2.0f, 0.125f, -0.025f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.35f, 0.25f, 0.25f));
	projectionShader.setVec3("aColor", glm::vec3(0.45f, 0.3f, 0.35f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.35f, 0.25f, 0.25f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;
	//---Esclerótica---//
	model = glm::translate(model, glm::vec3(2.0f, 0.25f, -0.625f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.125f));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.125f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;
	//---Pupila---//
	model = glm::translate(model, glm::vec3(2.0f, -0.15f, 0.1f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.025f));
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.025f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;
	//---Dientes---//
	model = glm::translate(model, glm::vec3(2.0f, -1.0f, 0.65f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.125f));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;
	
	model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.125f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;
	//---Lengua---//
	model = glm::translate(model, glm::vec3(1.0f, -0.05f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(1.75f, 0.125f, 0.25f));
	projectionShader.setVec3("aColor", glm::vec3(0.99f, 0.3f, 0.3f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;
	//---Hocico---//
	model = glm::translate(model, glm::vec3(0.0f, -0.3f, -0.25f));
	tmp = model;
	model = glm::scale(model, glm::vec3(3.0f, 0.5f, 0.8f));
	projectionShader.setVec3("aColor", glm::vec3(0.45f, 0.3f, 0.35f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;
	//---Parte baja del cuerpo---//
	model = glm::translate(model, glm::vec3(0.0f, -0.75f, -3.5f));
	tmp = model;
	model = glm::scale(model, glm::vec3(4.0f, 1.0f, 2.0f));
	projectionShader.setVec3("aColor", glm::vec3(0.45f, 0.3f, 0.35f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;
	//---Patas---//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.5f, -2.5f, 1.5f));
	tmp = model;
	model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));
	projectionShader.setVec3("aColor", glm::vec3(0.45f, 0.25f, 0.3f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;
	//---Pata parte abajo---//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.5f, -3.75f, 1.25f));
	tmp = model;
	model = glm::scale(model, glm::vec3(1.0f, 0.5f, 0.5f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(1.0f, 0.5f, 0.5f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(1.0f, 0.5f, 0.5f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	model = glm::translate(model, glm::vec3(-3.0f, -0.0f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(1.0f, 0.5f, 0.5f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	//---Pezuñas---//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.5f, -3.75f, 1.75f));
	tmp = model;
	model = glm::scale(model, glm::vec3(1.0f, 0.5f, 0.5f));
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(1.0f, 0.5f, 0.5f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(1.0f, 0.5f, 0.5f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	model = glm::translate(model, glm::vec3(-3.0f, -0.0f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(1.0f, 0.5f, 0.5f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	//---Orejas---//
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.75f, 1.0f, 2.125f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.25f));
	projectionShader.setVec3("aColor", glm::vec3(0.45f, 0.3f, 0.35f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	model = glm::translate(model, glm::vec3(-3.5f, 0.0f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.25f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;
	
	model = glm::translate(model, glm::vec3(-0.25f, 0.25f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.25f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.25f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.125f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.2f, 0.4f, 0.025f));
	projectionShader.setVec3("aColor", glm::vec3(0.45f, 0.25f, 0.3f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	model = glm::translate(model, glm::vec3(-4.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.4f, 0.025f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	//---trasero---/
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(3.0f, 2.0f, 0.5f));
	projectionShader.setVec3("aColor", glm::vec3(0.45f, 0.25f, 0.3f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	model = glm::translate(model, glm::vec3(0.0f, 0.25f, -0.25f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.25f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

	model = glm::translate(model, glm::vec3(0.0f, -0.5f, -0.25f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.5f, 1.5f, 0.25f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	model = tmp;

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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 4: Castro Molano Ramsés", NULL, NULL);
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
		glClearColor(0.3f, 0.7f, 0.9f, 1.0f);
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
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.08f;
	if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		angy += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		angy -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		angX += 0.5f;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		angX -= 0.5f;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		angX = 0.0f;
		angy = 0.0f;
		movX = 0.0f;
		movY = 0.0f;
		movZ = -15.0;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}