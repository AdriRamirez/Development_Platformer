#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Input.h"
#include "Scene.h"
#include "Menu.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

Menu::Menu() : Module()
{
	name.Create("menu");
}

// Destructor
Menu::~Menu()
{}

// Called before render is available
bool Menu::Awake()
{

	return true;
}

// Called before the first frame
bool Menu::Start()
{
	r = { 0, 0, 1920, 1080 };

	paused = false;
	dead = false;
	settings = false;
	fullscreen = false;
	app->vsync = false;
	credits = false;

	pause_buttons[0].state = 1;
	dead_buttons[0].state = 1;
	menu_buttons[0].state = 1;
	settings_buttons[0].state = 1;
	chosed = 0;

	app->win->GetWindowSize(win_w, win_h);

	for (size_t i = 0; i < NUM_PAUSE_BUTTONS; i++)
	{
		pause_buttons[i].rect.x = ((int)win_w / 2) - (pause_buttons[i].rect.w / 2);
		pause_buttons[i].rect.y = ((int)win_h / (NUM_PAUSE_BUTTONS + 1)) * (i + 1);
	}

	for (size_t i = 0; i < NUM_DEAD_BUTTONS; i++)
	{
		dead_buttons[i].rect.x = ((int)win_w / 2) - (dead_buttons[i].rect.w / 2);
		dead_buttons[i].rect.y = ((int)win_h / (NUM_PAUSE_BUTTONS + 3)) * (i + 2.5f);
	}

	for (size_t i = 0; i < NUM_MENU_BUTTONS; i++)
	{
		menu_buttons[i].rect.x = ((int)win_w / 2) - (menu_buttons[i].rect.w / 2);
		menu_buttons[i].rect.y = ((int)win_h / (NUM_MENU_BUTTONS + 1)) * (i + 1);
	}

	for (size_t i = 0; i < NUM_SETTINGS_BUTTONS; i++)
	{
		settings_buttons[i].rect.x = ((int)win_w / 2) - (settings_buttons[i].rect.w / 2);
		settings_buttons[i].rect.y = ((int)win_h / (NUM_PAUSE_BUTTONS + 1)) * (i + 1);
	}

	pause_buttons[0].tex = app->tex->Load("Assets/textures/Continue.png"); // continue
	pause_buttons[1].tex = app->tex->Load("Assets/textures/Save.png"); // save
	pause_buttons[2].tex = dead_buttons[0].tex = app->tex->Load("Assets/textures/Load.png"); // load
	pause_buttons[3].tex = dead_buttons[1].tex = app->tex->Load("Assets/textures/Exit.png"); // quit
	
	menu_buttons[0].tex = app->tex->Load("Assets/textures/Play.png"); // Play
	menu_buttons[0].alt_tex = app->tex->Load("Assets/textures/Continue.png"); // Continue
	menu_buttons[1].tex = app->tex->Load("Assets/textures/Settings.png"); // Settings
	menu_buttons[2].tex = app->tex->Load("Assets/textures/Credits.png"); // Credits
	menu_buttons[2].alt_tex = app->tex->Load("Assets/textures/CreditsImage.png"); // Credits
	menu_buttons[3].tex = app->tex->Load("Assets/textures/Exit.png"); // Exit

	settings_buttons[0].alt_tex = settings_buttons[1].alt_tex = app->tex->Load("Assets/textures/Slider.png"); // Slider
	settings_buttons[0].tex = app->tex->Load("Assets/textures/Sound.png"); // Audio
	settings_buttons[1].tex = app->tex->Load("Assets/textures/Fx.png"); // Fx slider
	settings_buttons[2].tex = app->tex->Load("Assets/textures/Fullscreen_off.png"); // Fullscreen
	settings_buttons[2].alt_tex = app->tex->Load("Assets/textures/Fullscreen_on.png"); // Fullscreen
	settings_buttons[3].tex = app->tex->Load("Assets/textures/Vsync_off.png"); // Vsync
	settings_buttons[3].alt_tex = app->tex->Load("Assets/textures/Vsync_on.png"); // Vsync

	gameOver = app->tex->Load("Assets/textures/game_over_screen.png"); 


	return true;
}

// Called each loop iteration
bool Menu::PreUpdate()
{
	intro = app->scene->GetStartScreenState();

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN  && !dead 
		&& app->scene->player->currentAnimation != &app->scene->player->deathAnimL && app->scene->player->currentAnimation != &app->scene->player->deathAnimR)
	{
		paused = !paused;

		pause_buttons[chosed = 0].state = 1;
		for (size_t i = 1; i < NUM_PAUSE_BUTTONS; i++)
		{
			pause_buttons[i].state = 0;
		}
	}

	if (settings && app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		settings = false;
	}

	/*if (paused && !loading && !dead)
	{
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && chosed < (NUM_PAUSE_BUTTONS - 1))
		{
			pause_buttons[chosed].state = 0;
			chosed++;
			pause_buttons[chosed].state = 1;
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && chosed > 0)
		{
			pause_buttons[chosed].state = 0;
			chosed--;
			pause_buttons[chosed].state = 1;
		}
	}
	if (intro && !loading && !dead && !paused) 
	{
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && chosed_menu < (NUM_MENU_BUTTONS - 1))
		{
			menu_buttons[chosed_menu].state = 0;
			chosed_menu++;
			menu_buttons[chosed_menu].state = 1;
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && chosed_menu > 0)
		{
			menu_buttons[chosed_menu].state = 0;
			chosed_menu--;
			menu_buttons[chosed_menu].state = 1;
		}
	}

	if (settings && !loading && !dead && !paused) 
	{
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && chosed_settings < (NUM_SETTINGS_BUTTONS - 1))
		{
			settings_buttons[chosed_settings].state = 0;
			chosed_settings++;
			settings_buttons[chosed_settings].state = 1;
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && chosed_settings > 0)
		{
			settings_buttons[chosed_settings].state = 0;
			chosed_settings--;
			settings_buttons[chosed_settings].state = 1;
		}
	}

	if (dead && !loading)
	{
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && chosed < (NUM_DEAD_BUTTONS - 1))
		{
			dead_buttons[chosed].state = 0;
			chosed++;
			dead_buttons[chosed].state = 1;
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && chosed > 0)
		{
			dead_buttons[chosed].state = 0;
			chosed--;
			dead_buttons[chosed].state = 1;
		}
	}*/

	int x, y;
	app->input->GetMousePosition(x, y);
	float c = -app->render->camera.x;
	for (size_t i = 0; i < NUM_PAUSE_BUTTONS; i++)
	{
		SDL_Rect rect = pause_buttons[i].rect;
		if (x + c > rect.x && x + c < rect.x + rect.w && y > rect.y && y < rect.y + rect.h)
		{
			if (paused)
			{
				//app->audio->PlayFx(hover_sound);
			}
			chosed = i;
			pause_buttons[i].state = 1;
		}
		else
		{
			pause_buttons[i].state = 0;
		}
	}

	for (size_t i = 0; i < NUM_MENU_BUTTONS; i++)
	{
		SDL_Rect rect = menu_buttons[i].rect;
		if (x + c > rect.x && x + c < rect.x + rect.w && y > rect.y && y < rect.y + rect.h)
		{
			if (intro)
			{
				//app->audio->PlayFx(hover_sound);
			}
			chosed = i;
			menu_buttons[i].state = 1;
		}
		else
		{
			menu_buttons[i].state = 0;
		}
	}

	for (size_t i = 0; i < NUM_SETTINGS_BUTTONS; i++)
	{
		SDL_Rect rect = settings_buttons[i].rect;
		if (x + c > rect.x && x + c < rect.x + rect.w && y > rect.y && y < rect.y + rect.h)
		{
			if (settings)
			{
				//app->audio->PlayFx(hover_sound);
			}
			chosed = i;
			settings_buttons[i].state = 1;
		}
		else
		{
			settings_buttons[i].state = 0;
		}
	}

	for (size_t i = 0; i < NUM_DEAD_BUTTONS; i++)
	{
		SDL_Rect rect = dead_buttons[i].rect;
		if (x + c > rect.x && x + c < rect.x + rect.w && y > rect.y && y < rect.y + rect.h)
		{
			if (dead)
			{
				//app->audio->PlayFx(hover_sound);
			}
			chosed = i;
			dead_buttons[i].state = 1;
		}
		else
		{
			dead_buttons[i].state = 0;
		}
	}

	return true;
}

// Called each loop iteration
bool Menu::Update(float dt)
{
	// pause buttons
	if (paused && !intro && !settings)
	{
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_PRESSED && pause_buttons[chosed].state == 1)
		{
			switch (chosed)
			{
			case 0: 
				paused = false;
				break;
			case 1:
				app->SaveGameRequest();
				break;
			case 2:
				app->scene->FadeToBlack(-1);
				loading = true;
				break;
			case 3:
				return false;
				break;
			}

			pause_buttons[chosed].state = 1;
		}
	}

	//menu buttons
	if (intro && !settings) 
	{
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_PRESSED && menu_buttons[chosed].state == 1)
		{
			switch (chosed)
			{
			case 0:
				if (!started)
				{
					app->scene->destination_level = 1;
					app->scene->go_black = true;
					saving = true;
					intro = false;
					paused = false;
					started = true;
				}
				else
				{
					app->scene->QuitStartScreen();
					app->LoadGameRequest();
				}
				break;
			case 1:
				settings = true;
			case 2:
				credits = !credits;
				break;
			case 3:
				return false;
				break;
			}

			menu_buttons[chosed].state = 2;
		}
	}

	//settings buttons
	if (settings)
	{
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_PRESSED && settings_buttons[chosed].state == 1)
		{
			switch (chosed)
			{
			case 0:
				slider = !slider;
				break;
			case 1:
				slider2 = !slider2;
				break;
			case 2:
				fullscreen = !fullscreen;
				if (fullscreen)
				{
					SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				}
				else
				{
					SDL_SetWindowFullscreen(app->win->window, 0);
				}
				break;
			case 3:
				app->vsync = !app->vsync;
				if (app->vsync)
				{
					// 1 to activate, 0 to deactivate
					SDL_GL_SetSwapInterval(1);
				}
				else
				{
					SDL_GL_SetSwapInterval(0);
				}
				break;
			}

			settings_buttons[chosed].state = 2;
		}
	}

	// fade at load
	if (loading)
	{
		load_cd--;
		if (load_cd <= 0)
		{
			app->LoadGameRequest();
			load_cd = 120;
			loading = false;
			paused = false;
		}
	}

	
	// dead buttons
	if (dead && !loading)
	{
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_PRESSED && dead_buttons[chosed].state == 1)
		{
			switch (chosed)
			{
			case 0:
				app->scene->FadeToBlack(-1);
				loading = true;
				break;
			case 1:
				return false;
				break;
			}

			dead_buttons[chosed].state = 1;
		}
	}

	return true;
}

// Called each loop iteration
bool Menu::PostUpdate()
{
	int c_x = -app->render->camera.x;
	r.x = c_x;

	if (paused && !intro && !settings)
	{
		app->render->DrawRectangle(r, 0, 0, 0, 200);

		for (size_t i = 0; i < NUM_PAUSE_BUTTONS; i++)
		{
			pause_buttons[i].rect.x = ((int)win_w / 2) - (pause_buttons[i].rect.w / 2) + c_x;

			if (pause_buttons[i].state == 0)
			{
				app->render->DrawRectangle(pause_buttons[i].rect, idleColorR, idleColorG, idleColorB);
			}
			else
			{
				app->render->DrawRectangle(pause_buttons[i].rect, inColorR, inColorG, inColorB);
			}

			app->render->DrawTexture(pause_buttons[i].tex, pause_buttons[i].rect.x, pause_buttons[i].rect.y);
		}
	}

	if (intro && !settings)
	{
		for (size_t i = 0; i < NUM_MENU_BUTTONS; i++)
		{
			menu_buttons[i].rect.x = ((int)win_w / 2) - (menu_buttons[i].rect.w / 2) + c_x;


			if (menu_buttons[i].state == 0)
			{
				app->render->DrawRectangle(menu_buttons[i].rect, idleColorR, idleColorG, idleColorB);
			}
			else 
			{
				app->render->DrawRectangle(menu_buttons[i].rect, inColorR, inColorG, inColorB);
			}
			

			if (credits)
			{
				app->render->DrawTexture(menu_buttons[i].alt_tex, 0, 0);
			}

			if (i == 0)
			{
				if (started)
				{
					app->render->DrawTexture(menu_buttons[i].alt_tex, menu_buttons[i].rect.x, menu_buttons[i].rect.y);
				}
				else
				{
					app->render->DrawTexture(menu_buttons[i].tex, menu_buttons[i].rect.x, menu_buttons[i].rect.y);
				}

			}
			else
			{
				app->render->DrawTexture(menu_buttons[i].tex, menu_buttons[i].rect.x, menu_buttons[i].rect.y);
			}
		}
	}

	if (settings)
	{
		int z, w;
		app->input->GetMousePosition(z, w);

		if (intro == false)
		{
			app->render->DrawRectangle(r, 0, 0, 0, 200);
		}

		for (size_t i = 0; i < NUM_SETTINGS_BUTTONS; i++)
		{
			settings_buttons[i].rect.x = ((int)win_w / 2) - (settings_buttons[i].rect.w / 2) + c_x;

			if (settings_buttons[i].state == 0)
			{
				app->render->DrawRectangle(settings_buttons[i].rect, idleColorR, idleColorG, idleColorB);
			}
			else 
			{
				app->render->DrawRectangle(settings_buttons[i].rect, inColorR, inColorG, inColorB);
			}
			

			if (slider)
			{
				if (z < 345)
				{
					z = 345;
				}
				else if (z > 447)
				{
					z = 447;
				}
				slider_bar1 = z;
				app->render->DrawTexture(settings_buttons[0].alt_tex, z + c_x, settings_buttons[0].rect.y);
				//app->audio->SetMusic((z - 540) / 2);
			}
			else
			{
				app->render->DrawTexture(settings_buttons[0].alt_tex, slider_bar1 + c_x, settings_buttons[0].rect.y);
			}

			if (slider2)
			{
				if (z < 345)
				{
					z = 345;
				}
				else if (z > 447)
				{
					z = 447;
				}
				slider_bar2 = z;
				app->render->DrawTexture(settings_buttons[1].alt_tex, z + c_x, settings_buttons[1].rect.y);
				//app->audio->SetFX((z - 540) / 2);
			}
			else
			{
				app->render->DrawTexture(settings_buttons[1].alt_tex, slider_bar2 + c_x, settings_buttons[1].rect.y);
			}

			if (app->vsync && i == 3)
			{
				app->render->DrawTexture(settings_buttons[i].alt_tex, settings_buttons[i].rect.x, settings_buttons[i].rect.y);
			}

			if (fullscreen && i == 2)
			{
				app->render->DrawTexture(settings_buttons[i].alt_tex, settings_buttons[i].rect.x, settings_buttons[i].rect.y);
			}

			app->render->DrawTexture(settings_buttons[i].tex, settings_buttons[i].rect.x, settings_buttons[i].rect.y);

		}
	}

	if (dead)
	{
		app->render->DrawRectangle(r, 0, 0, 0, 200);

		app->render->DrawTexture(gameOver, c_x, -20);
		

		for (size_t i = 0; i < NUM_DEAD_BUTTONS; i++)
		{
			dead_buttons[i].rect.x = ((int)win_w / 2) - (dead_buttons[i].rect.w / 2) + c_x;

			if (dead_buttons[i].state == 0)
			{
				app->render->DrawRectangle(dead_buttons[i].rect, idleColorR, idleColorG, idleColorB);
			}
			else
			{
				app->render->DrawRectangle(dead_buttons[i].rect, inColorR, inColorG, inColorB);
			}

			app->render->DrawTexture(dead_buttons[i].tex, dead_buttons[i].rect.x, dead_buttons[i].rect.y);
		}
	}

	return true;
}

// Called before quitting
bool Menu::CleanUp()
{

	return true;
}

bool Menu::GetGameState()
{
	return paused;
}