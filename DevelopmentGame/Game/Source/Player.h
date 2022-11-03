#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);


public:

private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	SDL_Texture* texRight = NULL;
	SDL_Texture* texLeft = NULL;
	const char* texturePath;
	//const char* texturerightPath;


	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;

	bool lookLeft;
	bool inAir = true;
	bool djump = true;

	float jumpForce = 20.0f;

	int pickCoinFxId;

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation = nullptr;

	// A set of animations
	Animation idleAnimR;
	Animation idleAnimL;
	Animation walkAnimR;
	Animation walkAnimL;
	Animation driftAnimR;
	Animation driftAnimL;
	Animation jumpAnimR;
	Animation jumpAnimL;
	Animation fallAnimR;
	Animation fallAnimL;
	Animation doublejumpAnimR;
	Animation doublejumpAnimL;
	Animation deathAnimR;
	Animation deathAnimL;

};

#endif // __PLAYER_H__