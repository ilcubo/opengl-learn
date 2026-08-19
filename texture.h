#ifndef __TEXTURE_H__
#define __TEXTURE_H__
#include <glad/gl.h>
#include "stb_image.h"

struct texture;
typedef struct texture *Texture;

Texture load_texture_rgb(char *file_path, GLint tex_unit);
Texture load_texture_rgba(char *file_path, GLint tex_unit);
#endif
