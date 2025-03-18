#ifndef SHADER_H
#define SHADER_H

#include "context.h"
#include "mat4.h"

typedef struct {
	unsigned int id;
} Shader;

bool Shader_Load(Shader *shader, const char *vertex_src, const char *fragment_src);

bool Shader_Use(Shader *shader);

bool Shader_SetUniform3f(Shader *shader, const char *name, float x, float y, float z);
bool Shader_SetUniform4f(Shader *shader, const char *name, float x, float y, float z, float w);
bool Shader_SetUniformMat4(Shader *shader, const char *name, const Mat4 *mat4);

#endif
