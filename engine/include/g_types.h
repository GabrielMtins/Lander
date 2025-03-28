#ifndef G_TYPES_H
#define G_TYPES_H

#include "g_def.h"
#include "context.h"
#include "vec2.h"
#include "texture.h"
#include "resource.h"
#include "entvars.h"
#include "mesh.h"

typedef struct scene_s Scene;
typedef struct game_s Game;
typedef struct entity_s Entity;
typedef struct world_s World;
typedef struct hud_s Hud;
typedef struct tile_s Tile;

struct game_s {
	Context *context;
	Scene *main_scene;
	Resource *resources;
};

typedef struct {
	Vec2 position;

	Vec2 offset;
	Vec2 scale;

	int portal;

	float texture;

	Vec3 normal;
} WallCfg;

typedef struct {
	WallCfg *walls;
	size_t num_walls;

	struct {
		Vec2 offset;
		Vec2 scale;

		float height;

		int wall_step;
		float step;

		float texture;

		Vec3 normal;
	} bottom, top;

	bool is_static;
	float offset_height;

	bool visited;

	Vec2 inside_point;
} Sector;

struct world_s {
	Texture *texture;
	uint32_t collision_layer;

	Sector sectors[MAX_SECTORS];
	size_t num_sectors;

	WallCfg walls[MAX_WALLS];
	size_t num_walls;

	int collided[MAX_COLLISIONS];

	float min_height, max_height;

	Mesh mesh;
};

struct hud_s  {
	Texture *texture;
	uint8_t tiles[HUD_NUM_TILES];
};

struct scene_s {
	Game *game;

	/* array de entidades */
	Entity *entities;
	size_t num_entities;

	/* stack de memória que indica quais entidades estão livres */
	Entity *free_entities_stack[MAX_ENTITIES];
	int top_free_index;

	World *world;
	Hud *hud;

	struct {
		Vec3 position;
		Vec3 angle;
	} camera;

	uint32_t tick;

	void (*loadNextScene)(Scene *);
};

/* Estrutura da entidade de fato. Se quiser adicionar componentes
 * a mais, adicione na EntVars por questão de organização */
struct entity_s {
	/* Essa flag é interna utilizada para saber se a entidade é válida ou se foi removida. */
	bool removed;
	/* Marque essa flag como true para que seja liberada da memória no próximo frame. */
	bool free;

	/* Variável que indica a vida de uma entidade.
	 * Quando health <= 0, a flag free é marcada como true
	 * para a remoção. */
	float health;

	/* Tipo da entidade. */
	int type;

	Vec3 position;
	float radius;
	float height;
	Vec3 velocity;

	/* Máscaras de colisão. */
	uint32_t collision_layer;
	uint32_t collision_mask;
	uint32_t trigger_mask;

	/* Pointer para indicar um target: inimigo etc */
	Entity *target;
	/* Pointer para indicar um filho da entidade */
	Entity *child;

	/* Flag para máquina de estado */
	int state;

	/* Textura atual utilizada */
	Texture *texture;
	/* Id da célula da texture */
	int cell_id;

	/* Tempo em ms para o próximo tempo em que a função think() será chamada */
	size_t next_think;

	/* Indica qual setor o centro da entidade está */
	int sector;

	/* A função update é chamada em toda interação */
	void (*update)(Scene *, Entity *, float);
	/* A função think é chamada apenas quando tick > next_think */
	void (*think)(Scene *, Entity *);
	/* A função é chamada toda vez que ocorre uma colisão */
	void (*onCollision)(Scene *, Entity *, Entity *);
	/* A função é chamada toda vez que há um trigger */
	void (*onTrigger)(Scene *, Entity *, Entity *);

	/* Variáveis customizadas para entidade */
	EntVars v;
};

#endif
