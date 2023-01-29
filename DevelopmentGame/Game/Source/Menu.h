#ifndef __MENU_H__
#define __MENU_H__

#include "Module.h"

#define NUM_PAUSE_BUTTONS 4
#define NUM_DEAD_BUTTONS 2
#define NUM_MENU_BUTTONS 4
#define NUM_SETTINGS_BUTTONS 4

struct Button {
	SDL_Rect rect = { 0, 0, 110, 35 };
	SDL_Texture* tex = NULL;
	SDL_Texture* alt_tex = NULL;

	int state = 0; // 0->idle, 1->above
};

class Menu : public Module
{
public:
	Menu();

	virtual ~Menu();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	bool GetGameState();

	bool dead;

private:
	SDL_Rect r;
	bool paused;
	bool intro;
	bool settings;
	bool fullscreen;
	bool credits;

	int slider_bar1 = 395;
	int slider_bar2 = 395;
	bool slider;
	bool slider2;

	int chosed;
	int chosed_menu;
	int chosed_settings;
	Button pause_buttons[NUM_PAUSE_BUTTONS];
	Button dead_buttons[NUM_DEAD_BUTTONS];
	Button menu_buttons[NUM_MENU_BUTTONS];
	Button settings_buttons[NUM_SETTINGS_BUTTONS];

	SDL_Texture* gameOver = NULL;
	SDL_Texture* cat = NULL;

	Uint8 idleColorR = 112;
	Uint8 idleColorG = 128;
	Uint8 idleColorB = 144;
	Uint8 inColorR = 219;
	Uint8 inColorG = 233;
	Uint8 inColorB = 244;

	int load_cd = 120;
	bool loading = false;

	bool started = false;
	int save_cd = 50;
	bool saving = false;

	uint win_w, win_h;
	
};

#endif