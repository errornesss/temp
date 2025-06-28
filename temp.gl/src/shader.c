#include "shader.h"

u32 shaderCompile(u32 type, const char *source) {
  u32 id = glCreateShader(type);
  glShaderSource(id, 1, &source, NULL);
  glCompileShader(id);

  i32 success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(id, sizeof(infoLog), NULL, infoLog);
    const char *typeStr;
    switch (type) {
      case GL_COMPUTE_SHADER: typeStr = "compute"; break;
      case GL_VERTEX_SHADER: typeStr = "vertex"; break;
      case GL_TESS_CONTROL_SHADER: typeStr = "tesselation control"; break;
      case GL_TESS_EVALUATION_SHADER: typeStr = "tesselation evaluation"; break;
      case GL_GEOMETRY_SHADER: typeStr = "geomety"; break;
      case GL_FRAGMENT_SHADER: typeStr = "fragment"; break;
    }
    printf("%s shader failed to compile\n%s\n", typeStr, infoLog);
  }

  return id;
}

u32 shaderCreate(ShaderSrc *shaderSrc) {
  u32 program = glCreateProgram();
  if (shaderSrc->vertex) {
    u32 vs = shaderCompile(GL_VERTEX_SHADER, shaderSrc->vertex);
    glAttachShader(program, vs);
    defer { glDeleteShader(vs); };
  }
  if (shaderSrc->fragment) {
    u32 fs = shaderCompile(GL_FRAGMENT_SHADER, shaderSrc->fragment);
    glAttachShader(program, fs);
    defer { glDeleteShader(fs); };
  }
  glLinkProgram(program);
  glValidateProgram(program);

  i32 success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
    printf("failed to link shader program\n%s", infoLog);
  }

  return program;
}


void shaderSrcFree(ShaderSrc *shaderSrc) {
  if (shaderSrc->vertex) free((char*)shaderSrc->vertex);
  if (shaderSrc->fragment) free((char*)shaderSrc->fragment);
}
