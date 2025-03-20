#include "player.h"

static void Player_Update(Scene *scene, Entity *entity, float time);

bool Player_Create(Entity *entity){
	Entity_Reset(entity);

	entity->update = Player_Update;

	return true;
}

static void Player_Update(Scene *scene, Entity *entity, float time){
	Context *context = scene->game->context;
	Vec3 direction = {0.0f, 0.0f, 0.0f};
	Mat4 tmp1, tmp2;

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

	Mat4_RotateY(&tmp1, scene->camera.angle.y);
	/*
	Mat4_RotateX(&tmp2, scene->camera.angle.x);
	Mat4_Mul(&tmp1, &tmp2, &tmp1);
	*/

	Vec3_Mul(&direction, &direction, time * 4.0f);
	Mat4_MulVector(&direction, &tmp1, &direction);

	Vec3_Add(&entity->position, &entity->position, &direction);

	scene->camera.position = entity->position;
}
