#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"

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

	// Called before quitting
	bool CleanUp();
	void SetA_Black()
	{
		a = 255;
	}
	bool NextLevel(int level);
	bool FadeFromBlack(int level);
	bool FadeToBlack(int level);
	bool ReturnStartScreen();

public:

	//L02: DONE 3: Declare a Player attribute 
	Player* player;
	SDL_Texture* title_screen;
	int current_lvl = 0;

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