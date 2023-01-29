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

	bool PostUpdate();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);


private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* textureRight;
	SDL_Texture* textureLeft;

	int jumpSound;
	int landSound;
	int hitSound;

	const char* texRight = NULL;
	const char* texLeft = NULL;

	const char* jumpFx = NULL;
	const char* landFx = NULL;
	const char* hitFx = NULL;

	//const char* texturePath;
	//const char* texturerightPath;


	// L07 DONE 5: Add physics to the player - declare a Physics body


	bool lookLeft;
	bool inAir = true;
	bool djump = true;
	bool rollin = false;
	bool dive = false;
	bool hit = false;
	bool lifePicked = false;
	bool lifePicked_2 = false;
	bool gem_1_Picked = false;

	float jumpForce = 20.0f;

	int pickCoinFxId;

	int foodSound;
	int gemSound;
	int enemySound;
	fPoint GetPlayerPosition();

public:
	PhysBody* pbody;
	Entity* entity = NULL;
	float x, y;
	int w;

	int numLives = 2;
	int lifePoints = 125;
	int points = 0;

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
	Animation diveAnimR;
	Animation diveAnimL;
	Animation hitAnimR;
	Animation hitAnimL;

};

#endif // __PLAYER_H__
