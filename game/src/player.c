#include "player.h"

#define PI 3.141592

static void Player_Update(Scene *scene, Entity *entity, float time);

bool Player_Create(Entity *entity){
	Entity_Reset(entity);

	entity->radius = 0.25f;
	entity->height = 0.9f;
	entity->sector = 6;

	entity->update = Player_Update;
	entity->position.y = -5.5f;

	return true;
}

static void Player_Update(Scene *scene, Entity *entity, float time){
	const char *keys = (const char *) SDL_GetKeyboardState(NULL);
	Context *context = scene->game->context;
	Vec3 direction = {0.0f, 0.0f, 0.0f};
	Mat4 tmp1;

	if(keys[SDL_SCANCODE_1])
		Context_SetFps(context, 60);

	if(keys[SDL_SCANCODE_2])
		Context_SetFps(context, 165);

	if(Context_GetKey(context, INPUT_LEFT))
		direction.x += -1.0f;

	if(Context_GetKey(context, INPUT_RIGHT))
		direction.x += +1.0f;

	if(Context_GetKey(context, INPUT_UP))
		direction.z += -1.0f;

	if(Context_GetKey(context, INPUT_DOWN))
		direction.z += +1.0f;

	if(Context_GetKey(context, INPUT_CRAWL))
		direction.y += -1.0f;

	if(Context_GetKey(context, INPUT_JUMP))
		direction.y += 1.0f;

	scene->camera.angle.y -= context->mouse_xrel * 0.004f;
	scene->camera.angle.x -= context->mouse_yrel * 0.004f;

	if(scene->camera.angle.x > PI / 2)
		scene->camera.angle.x = PI / 2;

	if(scene->camera.angle.x < -PI / 2)
		scene->camera.angle.x = -PI / 2;

	Mat4_RotateY(&tmp1, scene->camera.angle.y);

	float old_vel = direction.y;
	direction.y = 0.0f;
	Vec3_Normalize(&direction, &direction);
	direction.y = old_vel;
	Vec3_Mul(&direction, &direction, 3.0f);
	Mat4_MulVector(&direction, &tmp1, &direction);

	old_vel = entity->velocity.y;
	entity->velocity = direction;
	if(direction.y != 0.0f && old_vel == 0.0f){
		(void) old_vel;
	}
	else{
		old_vel -= 10.0f * time;
		entity->velocity.y = old_vel;
	}

	//Vec3_Add(&entity->position, &entity->position, &direction);

	scene->camera.position = entity->position;

	scene->camera.position.y += 0.5f;
}
