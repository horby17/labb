#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb_image.h>
#include"../header_files/shaderClass.h"
#include"../header_files/VAO.h"
#include"../header_files/VBO.h"
#include"../header_files/EBO.h"

// Coordenadas de Vertices
GLfloat vertices[] =
        {
                // COORDENADAS     COLORES        COORDENADAS PARA LA TEXTURA
                -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // Lower left corner
                -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // Upper left corner
                0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // Upper right corner
                0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f   // Lower right corner
        };

// Indices for vertices order
GLuint indices[] =
        {
                0, 2, 1,  // Upper triangle
                0, 3, 2   // Lower triangle
        };

int main()
{
    // Initialize GLFW
    glfwInit();
    // Tell GLFW what version of OpenGL we are using
    // In this case we are using OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW we are using the CORE profile
    // So that means we only have the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFWwindow object of 800 by 800 pixels
    GLFWwindow* window = glfwCreateWindow(800, 800, "Graficas de Computadoras de manera Estructurada", NULL, NULL);

    // Error check if the window fails to create
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Introduce the window into the current context
    glfwMakeContextCurrent(window);

    // Load GLAD so it configures OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Specify the viewport of OpenGL in the Window
    glViewport(0, 0, 800, 800);

    // Generates Shader object using shaders default.vert and default.frag
    Shader shaderProgram("default.vert", "default.frag");

    // Generates Vertex Array Object and binds it
    VAO VAO1;
    VAO1.Bind();

    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO1(vertices, sizeof(vertices));

    // Generates Element Buffer Object and links it to indices
    EBO EBO1(indices, sizeof(indices));

    // Links VBO attributes such as coordinates and colors to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Texture
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load("../resource_files/Textures/pngwing.com.png", &widthImg, &heightImg, &numColCh, 0);
    if (bytes == nullptr) {
        std::cout << "Failed to load texture" << std::endl;
        return -1;
    }
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
    shaderProgram.Activate();
    glUniform1i(tex0Uni, 0);

    // Main while loop
    while (!glfwWindowShouldClose(window))
    {
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use
        shaderProgram.Activate();
        // Assigns a value to the uniform
        glUniform1f(glGetUniformLocation(shaderProgram.ID, "scale"), 0.5f);
        glBindTexture(GL_TEXTURE_2D, texture);
        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }
    // Delete all the objects we've created
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    glDeleteTextures(1, &texture);
    shaderProgram.Delete();
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}
