#include "context.h"

static void Context_SetDefaultMapping(Context *context);

Context *Context_Create(const char *title, int w, int h, Mems *memory){
	printf("Hi\n");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	Mix_Init(MIX_INIT_MP3);

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
		return NULL;
	}

	Context *context;

	context = (Context *) Mems_Alloc(memory, sizeof(Context));

	context->window = SDL_CreateWindow(
			title,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			w, h,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
			);

	context->gl_context = SDL_GL_CreateContext(context->window);

	if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
        fprintf(stderr, "Failed to initialize GLAD\n");
	}

	context->width = w;
	context->height = h;

	glViewport(0, 0, w, h);

	SDL_RenderSetLogicalSize(context->renderer, w, h);

	context->memory = memory;

	context->quit = false;
	context->tick = 0;
	context->data = NULL;
	context->dt = 0.0f;
	context->min_time_frame = 0;

	context->key_mapping = Mems_Alloc(memory, sizeof(int) * NUM_KEYS);
	context->button_mapping = Mems_Alloc(memory, sizeof(int) * NUM_KEYS);

	Context_SetDefaultMapping(context);

	context->keyboard_state = NULL;
	context->controller = NULL;

	Context_OpenGameController(context, 0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	return context;
}

bool Context_GetKey(Context *context, int key){
	if(context->keyboard_state != NULL)
		if(context->keyboard_state[context->key_mapping[key]])
			return true;

	if(context->controller != NULL)
		if(SDL_GameControllerGetButton(context->controller, context->button_mapping[key]))
			return true;

	return false;
}

bool Context_SetFps(Context *context, size_t fps){
	context->min_time_frame = 1000 / fps;

	return true;
}

bool Context_OpenGameController(Context *context, int joystick_index){
	context->controller = SDL_GameControllerOpen(joystick_index);

	return context->controller != NULL;
}

bool Context_SetDataFromMem(Context *context, const char *data, int size){
	context->data = SDL_RWFromConstMem((const void *) data, size);

	return context->data != NULL;
}

bool Context_SetDataFromFile(Context *context, const char *filename){
	context->data = SDL_RWFromFile(filename, "rb");

	return context->data != NULL;
}

void Context_PollEvent(Context *context){
	SDL_Event e;
	size_t new_tick;

	new_tick = SDL_GetTicks();
	context->delta_tick = (new_tick - context->tick);

	if(context->delta_tick < context->min_time_frame && context->min_time_frame != 0){
		SDL_Delay(context->min_time_frame - context->delta_tick);
	}

	context->delta_tick = context->min_time_frame;

	context->dt = 0.001f * context->delta_tick;
	context->tick = SDL_GetTicks();
	
	while(SDL_PollEvent(&e)){
		if(e.type == SDL_QUIT)
			context->quit = true;
		else if(e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
			glViewport(0, 0, e.window.data1, e.window.data2);
		}
	}

	context->keyboard_state = SDL_GetKeyboardState(NULL);
}

bool Context_Destroy(Context *context){
	SDL_GL_DeleteContext(context->gl_context);
	SDL_DestroyWindow(context->window);

	if(context->data != NULL)
		SDL_RWclose(context->data);

	IMG_Quit();
	SDL_Quit();
	Mix_Quit();

	return true;
}

static void Context_SetDefaultMapping(Context *context){
	context->key_mapping[INPUT_LEFT] = SDL_SCANCODE_A;
	context->key_mapping[INPUT_RIGHT] = SDL_SCANCODE_D;
	context->key_mapping[INPUT_UP] = SDL_SCANCODE_W;
	context->key_mapping[INPUT_DOWN] = SDL_SCANCODE_S;
	context->key_mapping[INPUT_Y] = SDL_SCANCODE_J;
	context->key_mapping[INPUT_B] = SDL_SCANCODE_K;
	context->key_mapping[INPUT_X] = SDL_SCANCODE_U;
	context->key_mapping[INPUT_A] = SDL_SCANCODE_I;
	context->key_mapping[INPUT_L] = SDL_SCANCODE_LEFT;
	context->key_mapping[INPUT_R] = SDL_SCANCODE_RIGHT;
	context->key_mapping[INPUT_START] = SDL_SCANCODE_RETURN;
	context->key_mapping[INPUT_SELECT] = SDL_SCANCODE_SPACE;

	context->button_mapping[INPUT_LEFT] = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
	context->button_mapping[INPUT_DOWN] = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
	context->button_mapping[INPUT_UP] = SDL_CONTROLLER_BUTTON_DPAD_UP;
	context->button_mapping[INPUT_RIGHT] = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
	context->button_mapping[INPUT_Y] = SDL_CONTROLLER_BUTTON_X;
	context->button_mapping[INPUT_B] = SDL_CONTROLLER_BUTTON_A;
	context->button_mapping[INPUT_X] = SDL_CONTROLLER_BUTTON_Y;
	context->button_mapping[INPUT_A] = SDL_CONTROLLER_BUTTON_B;
	context->button_mapping[INPUT_L] = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
	context->button_mapping[INPUT_R] = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
	context->button_mapping[INPUT_START] = SDL_CONTROLLER_BUTTON_START;
	context->button_mapping[INPUT_SELECT] = SDL_CONTROLLER_BUTTON_BACK;
}
