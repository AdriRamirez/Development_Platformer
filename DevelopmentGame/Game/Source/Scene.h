#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Life.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();
	void SetA_Black()
	{
		a = 255;
	}

	bool FadeFromBlack(int level);
	bool FadeToBlack(int level);
	bool ReturnStartScreen();

	bool godMode=false;

public:

	//L02: DONE 3: Declare a Player attribute 
	Player* player;
	SDL_Texture* title_screen;
	SDL_Texture* hud;
	SDL_Texture* life_points_tex;
	int current_lvl = 0;
	float x = 50, y = 500;

	Floor_Enemy* floor_enemy;

	Air_Enemy* air_enemy;

	Life* life;
	Life* life_2;

	bool enemyDeleted = false;

private:
	SDL_Rect r;
	Uint8 a;

	bool go_black = false;
	bool return_black = false;
	int fade_speed = 2;

	int destination_level = -1;
	pugi::xml_node node;
};

#endif // __SCENE_H__