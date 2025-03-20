#include "player.h"

#define PI 3.141592

static void Player_Update(Scene *scene, Entity *entity, float time);

bool Player_Create(Entity *entity){
	Entity_Reset(entity);

	ent->radius = 0.5f;
	ent->height = 0.5f;

	entity->update = Player_Update;
	entity->position.y = -3.5f;

	return true;
}

static void Player_Update(Scene *scene, Entity *entity, float time){
	Context *context = scene->game->context;
	Vec3 direction = {0.0f, 0.0f, 0.0f};
	Mat4 tmp1;

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
		direction.y += +1.0f;

	scene->camera.angle.y -= context->mouse_xrel * time * 0.5f;
	scene->camera.angle.x -= context->mouse_yrel * time * 0.5f;

	if(scene->camera.angle.x > PI / 2)
		scene->camera.angle.x = PI / 2;

	if(scene->camera.angle.x < -PI / 2)
		scene->camera.angle.x = -PI / 2;


	Mat4_RotateY(&tmp1, scene->camera.angle.y);

	Vec3_Mul(&direction, &direction, 4.0f);
	Mat4_MulVector(&direction, &tmp1, &direction);

	entity->velocity = direction;
	//Vec3_Add(&entity->position, &entity->position, &direction);

	scene->camera.position = entity->position;
}
