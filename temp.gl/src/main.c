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
// #include "texture.h"

f32 dt, prevTime;

vec3
  cameraPos = { 0, 0, 3 },
  cameraFront = { 0, 0, -1 },
  cameraUp = { 0, 1, 0 };
bool firstMouse = true;
vec2 lastPos = { WID/2.0f, HEI/2.0f };
f32
  yaw = -90.0f,
  pitch = 0.0f,
  fov = 45.0f;

void framebuffer_size_callback(GLFWwindow *window, i32 width, i32 height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  {
    f32 speed = 5 * dt;
    vec3 mult = {}, norm = {};
    glm_vec3_scale(cameraFront, speed, mult);
    glm_vec3_crossn(cameraFront, cameraUp, norm);
    glm_vec3_scale(norm, speed, norm);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      glm_vec3_add(cameraPos, mult, cameraPos);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
      glm_vec3_sub(cameraPos, mult, cameraPos);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
      glm_vec3_sub(cameraPos, norm, cameraPos);
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
      glm_vec3_add(cameraPos, norm, cameraPos);
  }
}

void mouse_callback(GLFWwindow *window, f64 xposIn, f64 yposIn) {
  vec2 pos = { xposIn, yposIn };

  if (firstMouse) {
    lastPos[x] = pos[x];
    lastPos[y] = pos[y];
    firstMouse = false;
  }

  vec2 offset = { pos[x] - lastPos[x], lastPos[y] - pos[y] };
  lastPos[x] = pos[x];
  lastPos[y] = pos[y];

  f32 sens = 0.1f;
  offset[x] *= sens;
  offset[y] *= sens;

  yaw += offset[x];
  pitch += offset[y];

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)

    pitch = -89.0f;

  vec3 front = {
    [x] = cos(glm_rad(yaw)) * cos(glm_rad(pitch)),
    [y] = sin(glm_rad(pitch)),
    [z] = sin(glm_rad(yaw)) * cos(glm_rad(pitch))
  };
  glm_vec3_normalize_to(front, cameraFront);
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


  GLFWwindow *window = glfwCreateWindow(WID, HEI, TITLE, NULL, NULL);
  if (!window) {
    printf("failed to create window\n");
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

  /* f32 verticies[] = {
//  | pos                | col              | tex
    -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 
    -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 
     0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 
     0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 
  }; */

  f32 verticies[] = {
//  | pos                 | tex
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 
    
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 
    
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 
    
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 
    
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 
    
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 
    -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 
    
  };

  vec3 cubePositions[] = {
    {  0.0f,  0.0f,  0.0f  },
    {  2.0f,  5.0f, -15.0f },
    { -1.5f, -2.2f, -2.5f  },
    { -3.8f, -2.0f, -12.3f },
    {  2.4f, -0.4f, -3.5f  },
    { -1.7f,  3.0f, -7.5f  },
    {  1.3f, -2.0f, -2.5f  },
    {  1.5f,  2.0f, -2.5f  },
    {  1.5f,  0.2f, -1.5f  },
    { -1.3f,  1.0f, -1.5f  }
  };

  u32 indicies[] = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 5,
    8, 9, 10, 10, 11, 8,
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16,
    20, 21, 22, 22, 23, 20,
  };

  u32 VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  {
    u8 spv = 5 * sizeof(f32);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, spv, (void*)0);
    glEnableVertexAttribArray(0);
    // glVertexAttribPointer(1, 3, GL_FLOAT, false, sov, (void*)(3 * sizeof(f32)));
    // glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, spv, (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
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
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture0);
  glUniform1i(glGetUniformLocation(shader, "texture1"), 1);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  defer {
    glDeleteTextures(1, &texture0);
    glDeleteTextures(1, &texture1);
  };

  i32
    modelLoc = glGetUniformLocation(shader, "model"),
    viewLoc = glGetUniformLocation(shader, "view"),
    projectionLoc = glGetUniformLocation(shader, "projection");


  mat4 projection = GLM_MAT4_IDENTITY_INIT;
  glm_perspective(PI/4, (f32)WID/HEI, 0.1f, 100.0f, projection);
  glUniformMatrix4fv(projectionLoc, 1, false, projection[0]);

  glEnable(GL_DEPTH_TEST);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window)) {
    f32 time = glfwGetTime();
    dt = time - prevTime;
    prevTime = time;

    processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* glUniform4f(
      glGetUniformLocation(shader, "uCol"),
      cos(time)/2+0.5f,
      cos(time-PI/2)/2+0.5f,
      cos(time-PI/1)/2+0.5f,
      1.0f
    ); */

    mat4 view = GLM_MAT4_IDENTITY_INIT;
    {
      vec3 target = {};
      glm_vec3_add(cameraPos, cameraFront, target);
      glm_lookat(cameraPos, target, cameraUp, view);
    }
    glUniformMatrix4fv(viewLoc, 1, false, view[0]);

    
    for (u8 i = 0; i < sizeof(cubePositions)/sizeof(vec3); i++) {
      mat4 model = GLM_MAT4_IDENTITY_INIT;
      glm_translate(model, cubePositions[i]);
      glm_rotate(model, time, (vec3){1, 1, 1});
      f32 angle = 20 * i;
      glm_rotate(model, glm_rad(angle), (vec3){1, 0.3, 0.5});
      glUniformMatrix4fv(modelLoc, 1, false, model[0]);

      glDrawElements(GL_TRIANGLES, sizeof(indicies)/sizeof(u32), GL_UNSIGNED_INT, 0);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}
