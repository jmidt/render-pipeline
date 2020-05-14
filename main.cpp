#include "RenderingConfig.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <math.h>
#include "shader.hpp"
#include "libraries/stb_image.h"


// Callbacks
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// Inputs
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main(int argc, char *argv[]) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(800, 600, "Learning To Render", NULL, NULL);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Set size
  glViewport(0, 0, 800, 600);
  // Register callbacks
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // -- SHADERS -- //
  Shader myShader("shaders/tutorial.vs", "shaders/tutorial.fs");

  // Scene setup
  float vertices1[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};
  float vertices2[] = {0.4f, 0.5f, 0.0f,
                      -0.6f, 0.5f, 0.0f,
                      -0.6f, -0.5f, 0.0f};
  unsigned int indices[] = {0, 1, 2,
                            3, 4, 5};


  // Textures
  float texCoords[] = {
    0.0f, 0.0f,  // lower-left corner  
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner
  };

  // Initialization of texture
  unsigned int texture;
  glGenTextures(1, &texture); 
  glBindTexture(GL_TEXTURE_2D, texture);

  // Texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); 

  // Load image
  int width, height, nrChannels;
  unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0); 
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load image"; 
  } 
  stbi_image_free(data);

  // Initialization of buffer objects
  unsigned int VAO1, VAO2, VBO1, VBO2, EBO;
  
  glGenBuffers(1, &EBO);
  glGenBuffers(1, &VBO1);
  glGenVertexArrays(1, &VAO1);
  glGenBuffers(1, &VBO2);
  glGenVertexArrays(1, &VAO2);
  

  // Vertex Array Object for collecting VBO and interpretation
  // Bind vertex array first, then bind vertex buffer and perhaps element buffer.
  glBindVertexArray(VAO1);
  // Copy vertex data to vertex array buffer object
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
  // Copy indices to element buffer object 
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  // Link Vertex Attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Second triangle
  glBindVertexArray(VAO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    // Input
    processInput(window);

    // Rendering //
    // Sets color to clear with when clearing color buffer. Basically the
    // background color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Set shader program
    float timeValue = glfwGetTime();
    float greenValue = (std::sin(timeValue) / 2.f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProgram1, "ourColor");
    glUseProgram(shaderProgram1);
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
    // Choose vertex array object
    // Typically chooses the object to draw
    glBindVertexArray(VAO1);
    // Draw it
    glDrawArrays(GL_TRIANGLES, 0, 3);


    //Second triangle
    glUseProgram(shaderProgram2);
    glBindVertexArray(VAO2);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // When using indices, use DrawElements
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Get new events
    glfwPollEvents();
    // Hand-off
    glfwSwapBuffers(window);
  }

  // Graceful exit
  glfwTerminate();
  return 0;
}