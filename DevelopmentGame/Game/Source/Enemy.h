#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Floor_Enemy : public Entity
{
public:

	Floor_Enemy();
	virtual ~Floor_Enemy();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

public:

	// The pointer to the current enemy animation
	// It will be switched depending on the enemy's movement direction
	Animation* currentEnemyAnimation = nullptr;

	// A set of animations
	Animation walkAnimR;
	Animation walkAnimL;

private:

	SDL_Texture* texture;
	const char* texturePath;

	PhysBody* pbody;
};

#endif // __ENEMY_H__
