#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <fstream>
#include <sstream>



void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
    {
        glfwDestroyWindow(window);
    }
}

std::string ReadFile(std::string path)
{
    std::ifstream is = std::ifstream(path, std::ios::binary);
    std::stringstream buffer;
    buffer << is.rdbuf();
    return buffer.str();
}


extern "C"
{
    _declspec(dllexport) unsigned long NvOptimusEnablement = 1;
    _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int main(void)
{


    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSwapInterval(1);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    else
    {
        std::cout << glGetString(GL_VERSION) << '\n';
        std::cout << glGetString(GL_VENDOR) << '\n';
    }
    
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    
    unsigned int VS = glCreateShader(GL_VERTEX_SHADER);
    std::string vertexsrc = ReadFile("Shaders/VertexShader.glsl");
    const char* vsrc = vertexsrc.c_str();
    glShaderSource(VS, 1, &vsrc, NULL);
    glCompileShader(VS);

    unsigned int FS = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragmentsrc = ReadFile("Shaders/FragmentShader.glsl");
    const char* fsrc = fragmentsrc.c_str();
    glShaderSource(FS, 1, &fsrc, nullptr);
    glCompileShader(FS);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, VS);
    glAttachShader(shaderProgram, FS);

    glLinkProgram(shaderProgram);

    glDeleteShader(VS);
    glDeleteShader(FS);


    float vertices[] =
    {
        -0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f,  0.5f,
         0.5f, -0.5f
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);
    glEnableVertexAttribArray(0);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

   

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use
        glUseProgram(shaderProgram);
        // Bind the VAO so OpenGL knows to use it
        glBindVertexArray(VAO);
        // Draw the triangle using the GL_TRIANGLES primitive
       glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices);
        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}