#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "../dep/glad.h"

#include "utils.h"

typedef struct ShaderSrc {
  const char *vertex;
  const char *fragment;
} ShaderSrc;

u32 shaderCompile(u32 type, const char *src);
u32 shaderCreate(ShaderSrc *ss);
void shaderSrcFree(ShaderSrc *ss);
