#include "entity.h"

bool Entity_Reset(Entity *ent){
	if(ent == NULL)
		return false;

	ent->removed = false;
	ent->free = false;

	ent->health = 1.0f;

	ent->type = -1;

	ent->position = (Vec2){0.0f, 0.0f};
	ent->offset_sprite = (Vec2){0.0f, 0.0f};
	ent->size     = (Vec2){0.0f, 0.0f};
	ent->velocity = (Vec2){0.0f, 0.0f};

	ent->collision_layer = 0;
	ent->collision_mask  = 0;
	ent->trigger_mask  = 0;

	ent->target = NULL;
	ent->child = NULL;

	ent->state = -1;

	ent->texture = NULL;
	ent->cell_id = 0;
	ent->next_think = 0;

	ent->update = NULL;
	ent->think = NULL;
	ent->onCollision = NULL;
	ent->onTrigger = NULL;

	return true;
}
