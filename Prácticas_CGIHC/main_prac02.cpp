/*---------------------------------------------------------*/
/* ----------------   Práctica 2 --------------------------*/
/*-----------------    2019-2   ---------------------------*/
/*------------- Ramsés Castro Molano ---------------*/
#include <glew.h>
#include <glfw3.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 1920;
int SCR_HEIGHT = 1080;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;
GLuint shaderProgramRed, shaderProgramColor;

static const char* myVertexShader = "										\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec4 aPos;											\n\
																			\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);							\n\
}";

static const char* myVertexShaderColor = "									\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec3 aPos;											\n\
layout (location = 1) in vec3 aColor;										\n\
out vec3 ourColor;															\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos, 1.0);											\n\
	ourColor = aColor;														\n\
}";

// Fragment Shader
static const char* myFragmentShaderRed = "									\n\
#version 330																\n\
																			\n\
out vec3 finalColor;														\n\
																			\n\
void main()																	\n\
{																			\n\
    finalColor = vec3(1.0f, 0.0f, 0.0f);									\n\
}";

static const char* myFragmentShaderColor = "								\n\
#version 330 core															\n\
out vec4 FragColor;															\n\
in vec3 ourColor;															\n\
																			\n\
void main()																	\n\
{																			\n\
	FragColor = vec4(ourColor, 1.0f);										\n\
}";

void myData(void);
void setupShaders(void);
void display(void);
//void getResolution(void);


/*void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = 90000;//mode->width;
	SCR_HEIGHT = 90000;//(mode->height) - 80;
}*/

void myData()
{
	

	float vertices[] = 
	{   // El rango va de [-1,1]
		// positions         Color
		//Puntos para la letra "C"
		-0.6f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f,    //0
		-0.6f,  0.85f, 0.0f,  1.0f, 0.0f, 0.0f,    //1
		-0.6f,  0.6f,  0.0f,  1.0f, 0.0f, 0.0f,    //2
		-0.6f,  0.75f, 0.0f,  1.0f, 0.0f, 0.0f,    //3
		-0.85f, 0.85f, 0.0f,  0.0f, 1.0f, 0.0f,    //4
		-0.85f, 0.75f, 0.0f,  0.0f, 0.0f, 1.0f,    //5
		-1.0f,  0.6f,  0.0f,  1.0f, 0.0f, 0.0f,    //6
		-1.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f,    //7
		//Puntos para la letra "R"
		-0.4f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,      //8
		-0.1f, 1.0f, 0.0f,   0.5f, 0.5f, 0.0f,      //9
		-0.4f, 0.9f, 0.0f,   1.0f, 0.0f, 0.0f,      //10
		-0.1f, 0.9f, 0.0f,   0.0f, 0.0f, 1.0f,      //11
		-0.3f, 0.9f, 0.0f,   0.0f, 0.0f, 1.0f,      //12
		-0.4f, 0.6f, 0.0f,   0.0f, 0.0f, 1.0f,      //13
		-0.3f, 0.6f, 0.0f,   0.9f, 0.9f, 0.0f,      //14
		-0.2f, 0.9f, 0.0f,   0.0f, 0.0f, 1.0f,      //15
		-0.1f, 0.7f, 0.0f,   0.0f, 0.73f, 0.0f,     //16
		-0.2f, 0.7f, 0.0f,   0.0f, 0.0f, 1.0f,      //17
		-0.3f, 0.7f, 0.0f,   1.0f, 0.0f, 0.0f,      //18
		-0.1f, 0.6f, 0.0f,   0.0f, 0.0f, 1.0f,      //19
		-0.3f, 0.8f, 0.0f,   0.0f, 0.0f, 1.0f,      //20
		-0.2f, 0.8f, 0.0f,   0.0f, 0.0f, 1.0f,      //21
		//Puntos para la otra letra C
		0.5f,  1.0f,  0.0f,   0.6f, 0.0f, 0.7f,     //22
		0.5f,  0.85f, 0.0f,   0.0f, 1.0f, 0.0f,     //23
		0.5f,  0.6f,  0.0f,   0.0f, 1.0f, 0.0f,     //24
		0.5f,  0.75f, 0.0f,   0.0f, 0.2f, 0.8f,     //25
		0.25f, 0.85f, 0.0f,   0.0f, 1.0f, 0.0f,     //26
		0.25f, 0.75f, 0.0f,   0.0f, 0.4f, 0.9f,     //27
		0.1f,  0.6f,  0.0f,   0.0f, 0.0f, 0.0f,     //28
		0.1f,  1.0f,  0.0f,   0.9f, 0.4f, 0.0f,     //29
		//Puntos para la estrella - Pentagono
	   -0.1f,   0.0f, 0.0f,   0.0f, 0.4f, 0.6f,     //30
	   -0.15f, -0.2f, 0.0f,   0.0f, 0.4f, 0.6f,     //31
		0.0f,  -0.4f, 0.0f,   0.0f, 0.4f, 0.6f,     //32
		0.1f,   0.0f, 0.0f,   0.0f, 0.4f, 0.6f,     //33
		0.15f, -0.2f, 0.0f,   0.0f, 0.4f, 0.6f,     //34
		//Puntos para la estrella - Puntas
		0.0f,   0.3f, 0.0f,   0.0f, 0.0f, 0.0f,     //35
		0.21f, -0.6f, 0.0f,   0.0f, 0.0f, 0.0f,     //36
	   -0.21f, -0.6f, 0.0f,   0.0f, 0.0f, 0.0f,     //37
		0.35f,  0.0f, 0.0f,   0.0f, 0.0f, 0.0f,     //38
	   -0.35f,  0.0f, 0.0f,   0.0f, 0.0f, 0.0f      //39
	};

	unsigned int indices[] = //Orden al momento del dibujo
	{
		0,1,4, //Inicia la C
		0,4,7,
		7,4,5,
		7,6,5,
		6,5,2,
		5,3,2,
		8,9,10, //Inicia la R
		9,10,11,
		10,12,13,
		12,13,14,
		11,15,16,
		15,16,17,
		16,18,19,
		18,20,21,
		18,21,17,
		22,23,26, //Inicia la C
		22,26,29,
		29,26,27,
		29,28,27,
		28,27,24,
		27,25,24,
		30,31,32, //Inicia el pentagono
		30,32,33,
		32,33,34,
		30,33,35, //Inician las puntas
		32,34,36,
		31,32,37,
		30,31,39,
		33,34,38

	};


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void setupShaders()
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &myVertexShader, NULL);
	glCompileShader(vertexShader);

	unsigned int vertexShaderColor = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderColor, 1, &myVertexShaderColor, NULL);
	glCompileShader(vertexShaderColor);

	unsigned int fragmentShaderRed = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderRed, 1, &myFragmentShaderRed, NULL);
	glCompileShader(fragmentShaderRed);

	unsigned int fragmentShaderColor = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderColor, 1, &myFragmentShaderColor, NULL);
	glCompileShader(fragmentShaderColor);


	//Crear el Programa que combina Geometr�a con Color
	shaderProgramRed = glCreateProgram();
	glAttachShader(shaderProgramRed, vertexShader);
	glAttachShader(shaderProgramRed, fragmentShaderRed);
	glLinkProgram(shaderProgramRed);

	shaderProgramColor = glCreateProgram();
	glAttachShader(shaderProgramColor, vertexShaderColor);
	glAttachShader(shaderProgramColor, fragmentShaderColor);
	glLinkProgram(shaderProgramColor);
	//Check for errors 

	//ya con el Programa, el Shader no es necesario
	glDeleteShader(vertexShader);
	glDeleteShader(vertexShaderColor);
	glDeleteShader(fragmentShaderRed);
	glDeleteShader(fragmentShaderColor);

}

void display(void)
{
	glUseProgram(shaderProgramColor);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glPointSize(5.0);
	//glDrawElements(GL_LINE_LOOP, 8, GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 87, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_POINTS, 0, 5);


	glBindVertexArray(0);

	glUseProgram(0);

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
	//getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 2 Ramsés Castro Molano", NULL, NULL);
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
	//Configurar Shaders
	setupShaders();
    

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Backgound color Cambia el color del fondo
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		//Mi función de dibujo
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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}