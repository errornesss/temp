#include "texture.h"

void textureInit(/* u32 flags */) {
  stbi_set_flip_vertically_on_load(true);
  /* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); */
}

void textureLoad(u32 *texture, const char *source) {
  i32 width, height, nrChannels;
  unsigned char *data = stbi_load(source, &width, &height, &nrChannels, 0);
  glGenTextures(1, texture);
  glBindTexture(GL_TEXTURE_2D, *texture);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("failed to load %s\n", source);
  }
  stbi_image_free(data);
}

