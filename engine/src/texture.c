#include "texture.h"
#include "shader.h"
#include "mesh.h"

static const char *default_texture_vertex_src =
"#version 330 core\n"
"layout (location = 0) in vec3 in_pos;\n"
"layout (location = 1) in vec2 in_tex_coords;\n"
"uniform vec4 offset_tex_coords;\n"
"uniform mat4 model;\n"
"out vec2 out_tex_coords;\n"
"void main(){\n"
	"gl_Position = model * vec4(in_pos, 1.0f);\n"
	"gl_Position.y *= -1.0f;\n"
	"gl_Position.z = 0.0f;\n"
	"out_tex_coords = vec2(\n"
			"in_tex_coords.x * offset_tex_coords.z + offset_tex_coords.x,\n"
			"in_tex_coords.y * offset_tex_coords.w + offset_tex_coords.y\n"
	");\n"
"}\n";

static const char *default_texture_fragment_src =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 out_tex_coords;\n"
"uniform sampler2D frag_texture;\n"
"void main(){\n"
	"FragColor = texture(frag_texture, out_tex_coords);\n"
"}\n";

static const Vertex default_texture_vertices[] = {
	{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.0f},
	{{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.0f},
	{{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.0f},
	{{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.0f}
};

static const unsigned int default_texture_indices[] = {
	0, 1, 3,
	0, 2, 3
};

static bool loaded = false;

static Shader default_texture_shader;
static Mesh default_texture_mesh;

bool Texture_LoadDefaults(void){
	Shader_Load(
			&default_texture_shader,
			default_texture_vertex_src,
			default_texture_fragment_src
			);

	Mesh_Create(
			&default_texture_mesh,
			default_texture_vertices, 4,
			default_texture_indices, 6
			);

	return false;
}

bool TextureArray_Create(TextureArray *texture_array, int w, int h){
	texture_array->w = w;
	texture_array->h = h;
	texture_array->size = 0;

	glGenTextures(1, &texture_array->texture_id);

	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array->texture_id);

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 
			0,
			GL_RGBA8,
			w, h,
			64,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			NULL
			);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return true;
}

bool TextureArray_Load(TextureArray *texture_array, Context *context, const char *filename){
	(void) context;
	SDL_Surface *surface, *default_format;

	glGenTextures(1, &texture_array->texture_id);

	surface = IMG_Load(filename);

	if(surface == NULL){
		fprintf(stderr, "Failed to load texture: %s\n", filename);
		return false;
	}

	default_format = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
	SDL_FreeSurface(surface);
	surface = default_format;

	glTexSubImage3D(
			GL_TEXTURE_2D_ARRAY, 
			0, 
			0, 0, texture_array->size++,
			texture_array->w, texture_array->h, 1,
			GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels
			);

	SDL_FreeSurface(surface);

	return true;
}

bool MegaTexture_Load(MegaTexture *mega_texture, Context *context, const char *filename){
	(void) context;

	if(!loaded){
		Texture_LoadDefaults();
		loaded = true;
	}

	SDL_Surface *surface;

	if(mega_texture == NULL)
		return false;

	surface = IMG_Load(filename);

	if(surface == NULL){
		return false;
	}

	SDL_Surface *default_format = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
	SDL_FreeSurface(surface);
	surface = default_format;

	glGenTextures(1, &mega_texture->texture_id);
	glBindTexture(GL_TEXTURE_2D, mega_texture->texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			surface->w,
			surface->h,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			surface->pixels
			);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	mega_texture->w = surface->w;
	mega_texture->h = surface->h;

	SDL_FreeSurface(surface);


	return true;
}

bool MegaTexture_LoadData(MegaTexture *mega_texture, Context *context, size_t offset){
	// TODO
	(void) mega_texture;
	(void) context;
	(void) offset;
	
	return true;
}

bool MegaTexture_Destroy(MegaTexture *mega_texture){
	if(mega_texture == NULL)
		return false;

	glDeleteTextures(1, &mega_texture->texture_id);

	return true;
}

bool Texture_Create(Texture *texture, MegaTexture *mega_texture, int x, int y, int w, int h, int cell_w, int cell_h){
	if(mega_texture == NULL || texture == NULL)
		return false;

	(void) h;

	texture->mega_texture = mega_texture;

	texture->x = x;
	texture->y = y;
	texture->cell_w = cell_w;
	texture->cell_h = cell_h;

	texture->num_cells_x = w / cell_w;

	return true;
}

bool Texture_Render(Context *context, Texture *texture, int x, int y, int id, int flip){
	Mat4 model;
	Mat4 temp;

	Mat4_Identity(&model);

	/* Dimensionar ao tamanho certo */
	Mat4_Scale(&temp, texture->cell_w, texture->cell_h, 1.0f);
	Mat4_Mul(&model, &temp, &model);

	/* Definir a posição */
	Mat4_Transform(&temp, (float) x, (float) y, 0.0f);
	Mat4_Mul(&model, &temp, &model);

	/* Definir a escala correta para ser renderizada */
	Mat4_Scale(&temp, 2.0f / context->width, 2.0f / context->height, 1.0f);
	Mat4_Mul(&model, &temp, &model);

	/* Inverter */
	Mat4_Transform(&temp, -1.0f, -1.0f, 0.0f);
	Mat4_Mul(&model, &temp, &model);
	
	Shader_SetUniformMat4(&default_texture_shader, "model", &model);
	Shader_SetUniform4f(
			&default_texture_shader,
			"offset_tex_coords",
			(float) (texture->x + (id % texture->num_cells_x) * texture->cell_w) / texture->mega_texture->w,
			(float) (texture->y + (id / texture->num_cells_x) * texture->cell_h) / texture->mega_texture->h,
			(float) texture->cell_w / texture->mega_texture->w,
			(float) texture->cell_h / texture->mega_texture->h
			);

	glBindTexture(GL_TEXTURE_2D, texture->mega_texture->texture_id);
	Mesh_Render(&default_texture_mesh, &default_texture_shader);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}
