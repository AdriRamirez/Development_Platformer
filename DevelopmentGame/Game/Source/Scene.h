#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Life.h"
#include "Gem.h"

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

	void FontDrawScore(int score, int n, int posX, int posY, int separacio, float scale);

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

	Gem* gem_1;
	Gem* gem_2;
	Gem* gem_3;
	Gem* gem_4;
	Gem* gem_5;
	Gem* gem_6;
	Gem* gem_7;
	Gem* gem_8;
	Gem* gem_9;
	Gem* gem_10;
	Gem* gem_11;
	Gem* gem_12;
	Gem* gem_13;
	Gem* gem_14;
	Gem* gem_15;


	SDL_Texture* font;

	bool enemyDeleted = false;

	//Score
	int score = 000;
	int posicioFont = 60;
	int posicioFontY = 18;

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