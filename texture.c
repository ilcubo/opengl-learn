#include "texture.h"

#include <glad/gl.h>
#include <stdio.h>
#include <stdlib.h>

#include "stb_image.h"

struct texture {
  int width;
  int height;
  int nrChannels;
  GLint tex_unit;
  unsigned int id;
};

typedef struct texture *Texture;

Texture load_texture_rgb(char *file_path, GLint tex_unit) {
    Texture texture = malloc(sizeof(struct texture));
    if (texture) {
        glGenTextures(1, &(texture->id));
        texture->tex_unit = tex_unit;
        glActiveTexture(tex_unit);
        glBindTexture(GL_TEXTURE_2D, texture->id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned char *data = stbi_load(file_path, &(texture->width), &(texture->height), &(texture->nrChannels), 3);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            fprintf(stderr, "ERROR: Failed to load texture");
        }
        stbi_image_free(data);
    }
    return texture;
}

Texture load_texture_rgba(char *file_path, GLint tex_unit) {
    Texture texture = malloc(sizeof(struct texture));
    if (texture) {
        glGenTextures(1, &(texture->id));
        glActiveTexture(tex_unit);
        glBindTexture(GL_TEXTURE_2D, texture->id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned char *data = stbi_load(file_path, &(texture->width), &(texture->height), &(texture->nrChannels), 4);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            fprintf(stderr, "ERROR: Failed to load texture");
        }
        stbi_image_free(data);
    }
    return texture;
}