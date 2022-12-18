#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Pathfinding.h"
#include "Animation.h"

struct SDL_Texture;

enum class ENEMY_STATE
{
	IDLE,
	HUNT,
	RETURN,
	DEATH
};

class Floor_Enemy : public Entity
{
public:

	Floor_Enemy();

	virtual ~Floor_Enemy();

	bool Awake();

	bool Start();

	bool Update();

	bool PreUpdate();

	bool Draw();

	bool CleanUp();

	void ReviveGroundEnemy();

	void MoveGroundEnemy();
	void CheckPlayer();

	void EnemyHunting();
	void EnemyReturning(float dt);

	bool DeleteEntity();

	// load-save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

	void SwitchDirection();

public:
	float origin_x, origin_y;
	float speed;

	// The pointer to the current enemy animation
	// It will be switched depending on the enemy's movement direction
	Animation* currentFloorEnemyAnimation = nullptr;

	float detectionRange;

	ENEMY_STATE state;

	bool lookLeft;
	float idleOb_x;
	bool obLeft;

	PathFinding* path_save = NULL;

	// A set of animations
	Animation walkAnimR;
	Animation walkAnimL;

private:

	//SDL_Texture* texture;

	SDL_Texture* textureRight;
	SDL_Texture* textureLeft;

	//const char* texturePath;

	const char* texRight = NULL;
	const char* texLeft = NULL;

	PhysBody* pbody;
};



class Air_Enemy : public Entity
{
public:

	Air_Enemy();
	virtual ~Air_Enemy();

	bool Awake();

	bool Start();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

public:

	// The pointer to the current enemy animation
	// It will be switched depending on the enemy's movement direction
	Animation *currentAirEnemyAnimation = nullptr;

	// A set of animations
	Animation flyAnimR;
	Animation flyAnimL;

private:

	SDL_Texture* texture;

	SDL_Texture* textureRight;
	SDL_Texture* textureLeft;

	const char* texturePath;

	const char* texRight = NULL;
	const char* texLeft = NULL;

	PhysBody* pbody;
};

#endif // __ENEMY_H__
