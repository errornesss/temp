#pragma once

#include <stdbool.h>

#include "../dep/glad.h"
#include "../dep/stb.h"

#include "utils.h"

void textureInit(/* u32 flags */);
void textureLoad(u32 *texture, const char *source);
