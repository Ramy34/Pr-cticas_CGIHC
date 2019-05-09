/*----------------------------------------------------------*/
/* ----------------   Pr�ctica 5  --------------------------*/
/*-----------------    2019-2    ---------------------------*/
/*------------- Alumno: Castro Molano Rams�s ---------------*/
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
float	movX = -2.0f,
		movY = 0.0f,
		movZ = -10.0f,
		rotX = 0.0f,
		rotY = 0.0f;

float	hombro = 0.0f,
		codo = 0.0f,
		muneca = 0.0f,
		dedo = 0.0f,
		dedo_parte1 = 0.0f,
		dedo_parte2 = 0.0f;

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
	glm::mat4 tmp1 = glm::mat4(1.0f);
	glm::mat4 tmp2 = glm::mat4(1.0f);
	glm::mat4 tmp3 = glm::mat4(1.0f);

	
	//Use "projection" in order to change how we see the information
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	//Use "view" in order to affect all models
	view = glm::translate(view, glm::vec3(movX, movY, movZ));
	view = glm::rotate(view, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	// pass them to the shaders
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);


	glBindVertexArray(VAO);
	//Colocar c�digo aqu�
	model = glm::rotate(model, glm::radians(hombro), glm::vec3(0.0f, 0.0f, 1.0f)); //Coloca la articulaci�n
	tmp = model = glm::translate(model, glm::vec3(1.25f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f,1.0f,1.0f)); //Escalamos
	projectionShader.setMat4("model", model); //Pasamos la matriz al Shader
	projectionShader.setVec3("aColor", glm::vec3(0.2f, 0.2f, 0.2f));
	glDrawArrays(GL_QUADS, 0, 24); //A Hombro
	
	model = glm::translate(tmp, glm::vec3(1.25f, 0.0f,0.0f));//LLeva al punto de articulaci�n
	model = glm::rotate(model, glm::radians(codo), glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca la articulaci�n
	tmp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f)); //Mueve al centro de la figura
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f)); //Escalamos
	projectionShader.setMat4("model", model); //Pasamos la matriz al Shader
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.7f, 0.7f));
	glDrawArrays(GL_QUADS, 0, 24); //B Codo

	model = glm::translate(tmp, glm::vec3(1.0f, 0.0f, 0.0f));//LLeva al punto de articulaci�n
	model = glm::rotate(model, glm::radians(muneca), glm::vec3(0.0f, 0.0f, 1.0f)); //Coloca la articulaci�n
	tmp1 = tmp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f)); //Mueve al centro de la figura
	model = glm::scale(model, glm::vec3(0.5f, 1.0f, 1.0f)); //Escalamos
	projectionShader.setMat4("model", model); //Pasamos la matriz al Shader
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.7f, 0.7f));
	glDrawArrays(GL_QUADS, 0, 24); //C Mu�eca

	model = glm::translate(tmp, glm::vec3(0.25f, 0.35f, 0.375f));//LLeva al punto de articulaci�n
	model = glm::rotate(model, glm::radians(dedo_parte1), glm::vec3(0.0f, 0.0f, 1.0f)); //Coloca la articulaci�n
	tmp = model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f)); //Mueve al centro de la figura
	model = glm::scale(model, glm::vec3(0.75f, 0.3f, 0.25f)); //Escalamos
	projectionShader.setMat4("model", model); //Pasamos la matriz al Shader
	projectionShader.setVec3("aColor", glm::vec3(0.7f, 0.7f, 0.7f));
	glDrawArrays(GL_QUADS, 0, 24); //D dedo

	model = glm::translate(tmp, glm::vec3(0.375f, 0.0f, 0.0f));//LLeva al punto de articulaci�n
	model = glm::rotate(model, glm::radians(dedo_parte2), glm::vec3(0.0f, 0.0f, 1.0f)); //Coloca la articulaci�n
	tmp = model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f)); //Mueve al centro de la figura
	model = glm::scale(model, glm::vec3(0.75f, 0.3f, 0.25f)); //Escalamos
	projectionShader.setMat4("model", model); //Pasamos la matriz al Shader
	projectionShader.setVec3("aColor", glm::vec3(0.7f, 1.0f, 0.7f));
	glDrawArrays(GL_QUADS, 0, 24); //E dedo 

	model = glm::translate(tmp1, glm::vec3(0.25f, -0.35f, 0.375f));//LLeva al punto de articulaci�n
	model = glm::rotate(model, glm::radians(-dedo_parte1), glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca la articulaci�n
	tmp = model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f)); //Mueve al centro de la figura
	model = glm::scale(model, glm::vec3(0.75f, 0.3f, 0.25f)); //Escalamos
	projectionShader.setMat4("model", model); //Pasamos la matriz al Shader
	projectionShader.setVec3("aColor", glm::vec3(0.7f, 0.7f, 1.0f));
	glDrawArrays(GL_QUADS, 0, 24); //F dedo

	model = glm::translate(tmp, glm::vec3(0.375f, 0.0f, 0.0f));//LLeva al punto de articulaci�n
	model = glm::rotate(model, glm::radians(-dedo_parte2), glm::vec3(0.0f, 1.0f, 0.0f)); //Coloca la articulaci�n
	tmp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f)); //Mueve al centro de la figura
	model = glm::scale(model, glm::vec3(0.5f, 0.3f, 0.25f)); //Escalamos
	projectionShader.setMat4("model", model); //Pasamos la matriz al Shader
	projectionShader.setVec3("aColor", glm::vec3(0.3f, 0.4f, 0.8f));
	glDrawArrays(GL_QUADS, 0, 24); //G dedo

	model = glm::translate(tmp1, glm::vec3(0.25f, 0.35f, 0.0f));//LLeva al punto de articulaci�n
	model = glm::rotate(model, glm::radians(dedo_parte1), glm::vec3(0.0f, 0.0f, 1.0f)); //Coloca la articulaci�n
	tmp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); //Mueve al centro de la figura
	model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f)); //Escalamos
	projectionShader.setMat4("model", model); //Pasamos la matriz al Shader
	projectionShader.setVec3("aColor", glm::vec3(0.2f, 0.7f, 0.2f));
	glDrawArrays(GL_QUADS, 0, 24); //H dedo

	model = glm::translate(tmp, glm::vec3(0.5f, 0.0f, 0.0f));//LLeva al punto de articulaci�n
	model = glm::rotate(model, glm::radians(dedo_parte2), glm::vec3(0.0f, 0.0f, 1.0f)); //Coloca la articulaci�n
	tmp = model = glm::translate(model, glm::vec3(0.625f, 0.0f, 0.0f)); //Mueve al centro de la figura
	model = glm::scale(model, glm::vec3(1.25f, 0.3f, 0.25f)); //Escalamos
	projectionShader.setMat4("model", model); //Pasamos la matriz al Shader
	projectionShader.setVec3("aColor", glm::vec3(0.6f, 0.6f, 0.8f));
	glDrawArrays(GL_QUADS, 0, 24); //I dedo

	model = glm::translate(tmp1, glm::vec3(0.25f, 0.35f, -0.375f));//LLeva al punto de articulaci�n
	model = glm::rotate(model, glm::radians(dedo_parte1), glm::vec3(0.0f, 0.0f, 1.0f)); //Coloca la articulaci�n
	tmp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); //Mueve al centro de la figura
	model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f)); //Escalamos
	projectionShader.setMat4("model", model); //Pasamos la matriz al Shader
	projectionShader.setVec3("aColor", glm::vec3(0.5f, 0.7f, 0.3f));
	glDrawArrays(GL_QUADS, 0, 24); //J dedo

	model = glm::translate(tmp, glm::vec3(0.5f, 0.0f, 0.0f));//LLeva al punto de articulaci�n
	model = glm::rotate(model, glm::radians(dedo_parte2), glm::vec3(0.0f, 0.0f, 1.0f)); //Coloca la articulaci�n
	tmp = model = glm::translate(model, glm::vec3(0.625f, 0.0f, 0.0f)); //Mueve al centro de la figura
	model = glm::scale(model, glm::vec3(1.25f, 0.3f, 0.25f)); //Escalamos
	projectionShader.setMat4("model", model); //Pasamos la matriz al Shader
	projectionShader.setVec3("aColor", glm::vec3(0.8f, 0.4f, 0.3f));
	glDrawArrays(GL_QUADS, 0, 24); //K dedo

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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 5", NULL, NULL);
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
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
			movX = -2.0f,
			movY = 0.0f,
			movZ = -10.0f,
			rotX = 0.0f,
			rotY = 0.0f,
			hombro = 0.0f,
			codo = 0.0f,
			muneca = 0.0f,
			dedo = 0.0f,
			dedo_parte1 = 0.0f,
			dedo_parte2 = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		rotY += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		rotY -= 0.18f;
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
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		rotX += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		rotX -= 0.18f;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		if (hombro < 90.0f) {
			hombro += 0.5f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		if (hombro > -110.0f) {
			hombro -= 0.5f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		if (codo > -45.0f) {
			codo -= 0.5f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		if (codo < 0.0f) {
			codo += 0.5f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS){
		if (muneca < 60.0f) {
			muneca += 0.5f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		if (muneca > -40.0f) {
			muneca -= 0.5f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		if (dedo_parte1 < 20.0f) {
			dedo_parte1 += 0.5f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		if (dedo_parte1 > -50.0f) {
			dedo_parte1 -= 0.5f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		if (dedo_parte2 < 0.0f) {
			dedo_parte2 += 0.5f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		if (dedo_parte2 > -90.0f) {
			dedo_parte2 -= 0.5f;
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}