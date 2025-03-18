#include "shader.h"

static int Shader_GetLocation(Shader *shader, const char *name);

bool Shader_Load(Shader *shader, const char *vertex_src, const char *fragment_src){
	bool loaded = true;
	unsigned int vertex_shader = 0, fragment_shader = 0;

	{
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_src, NULL);
		glCompileShader(vertex_shader);

		int sucess;
		char log[512];
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &sucess);

		if(!sucess){
			glGetShaderInfoLog(vertex_shader, 512, NULL, log);
			fprintf(stderr, "error loading vertex shader\n%s", log);
			loaded = false;
		}
	}

	{
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_src, NULL);
		glCompileShader(fragment_shader);

		int sucess;
		char log[512];
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &sucess);

		if(!sucess){
			glGetShaderInfoLog(fragment_shader, 512, NULL, log);
			fprintf(stderr, "error loading fragment shader\n%s", log);
			loaded = false;
		}
	}

	if(loaded){
		shader->id = glCreateProgram();
		glAttachShader(shader->id, vertex_shader);
		glAttachShader(shader->id, fragment_shader);
		glLinkProgram(shader->id);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		int sucess;
		char log[512];
		glGetProgramiv(shader->id, GL_LINK_STATUS, &sucess);

		if(!sucess){
			glGetProgramInfoLog(shader->id, 512, NULL, log);
			fprintf(stderr, "error linking shader program\n%s", log);
		}
	}

	return loaded;
}

bool Shader_Use(Shader *shader){
	glUseProgram(shader->id);

	return true;
}

bool Shader_SetUniform3f(Shader *shader, const char *name, float x, float y, float z){
	int location = Shader_GetLocation(shader, name);

	if(location < 0)
		return false;

	glUseProgram(shader->id);
	glUniform3f(location, x, y, z);

	return true;
}

bool Shader_SetUniform4f(Shader *shader, const char *name, float x, float y, float z, float w){
	int location = Shader_GetLocation(shader, name);

	if(location < 0)
		return false;

	glUseProgram(shader->id);
	glUniform4f(location, x, y, z, w);

	return true;
}

bool Shader_SetUniformMat4(Shader *shader, const char *name, const Mat4 *mat4){
	int location = Shader_GetLocation(shader, name);

	if(location < 0)
		return false;

	glUseProgram(shader->id);
	glUniformMatrix4fv(location, 1, GL_TRUE, mat4->arr);

	return true;
}

static int Shader_GetLocation(Shader *shader, const char *name){
	return glGetUniformLocation(shader->id, name);
}
