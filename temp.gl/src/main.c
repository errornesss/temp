#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "../dep/glad.h"
#include "../dep/glfw.h"
#include "../dep/stb.h"
#include "../dep/cglm.h"

#include "utils.h"
#include "config.h"
#include "shader.h"
#include "texture.h"

#define PI 3.14159265358979323846264338327950288

void framebuffer_size_callback(GLFWwindow *window, i32 width, i32 height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

i32 main(/* i32 argc, char *argv[] */) {
  if (!glfwInit()) {
    printf("glfw failed to initialise\n");
    return -1;
  }
  defer { glfwTerminate(); };

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_RESIZABLE, false);

  GLFWwindow *window = glfwCreateWindow(HEI, WID, TITLE, NULL, NULL);
  if (!window) {
    printf("failed to create window\n");
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("failed to initialise glad\n");
    return -1;
  }

  ShaderSrc shaderSrc = (ShaderSrc){
    .vertex = fileToStr("./src/shaders/shader.vs"),
    .fragment = fileToStr("./src/shaders/shader.fs"),
  };
  u32 shader = shaderCreate(&shaderSrc);
  defer { glDeleteProgram(shader); };
  glUseProgram(shader);
  shaderSrcFree(&shaderSrc);

  f32 verticies[] = {
//  | pos                | col              | tex
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 
    -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 
     0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 
     0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 
  };

  u32 indicies[] = {
    0, 1, 2,
    2, 0, 3,
  };

  u32 VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  {
    u8 sov = 8 * sizeof(f32);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sov, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sov, (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sov, (void*)(6 * sizeof(f32)));
    glEnableVertexAttribArray(2);
  }
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

  defer {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO); 
    glDeleteBuffers(1, &EBO); 
  };

  stbi_set_flip_vertically_on_load(true);
  u32 texture0, texture1;
  glGenTextures(1, &texture0);
  glBindTexture(GL_TEXTURE_2D, texture0);
  i32 width, height, nrChannels;
  unsigned char *data = stbi_load("./res/textures/container.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("failed to load %s\n", "container");
  }
  stbi_image_free(data);

  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  data = stbi_load("./res/textures/awesomeface.png", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("failed to load %s\n", "awesomeface");
  }
  stbi_image_free(data);

  glUniform1i(glGetUniformLocation(shader, "texture0"), 0);
  glUniform1i(glGetUniformLocation(shader, "texture1"), 1);

  defer {
    glDeleteTextures(1, &texture0);
    glDeleteTextures(1, &texture1);
  };

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window)) {
    f32 time = glfwGetTime();
    processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUniform4f(
      glGetUniformLocation(shader, "uCol"),
      cos(time)/2+.5,
      cos(time-PI/2)/2+.5,
      cos(time-PI/1)/2+.5,
      1.0f
    );

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glDrawElements(GL_TRIANGLES, sizeof(indicies)/sizeof(u32), GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}
