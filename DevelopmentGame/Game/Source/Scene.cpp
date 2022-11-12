#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	node = config;
	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	//L02: DONE 3: Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");
	
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	title_screen = app->tex->Load("Assets/Textures/levels/title_screen.png");

	a = 0;
	r = { 0, 0, 2560, 1440 };
	// L03: DONE: Load map
	//app->map->Load();

	// L04: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());
	app->win->SetTitle(title.GetString());
	
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	if (title_screen != NULL && app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		//NextLevel(1);
		destination_level = 1;
		go_black = true;
	}

	if (go_black)
	{
		if (a < 256 - fade_speed)
		{
			a += fade_speed;
		}
		else if (a < 255)
		{
			a++;
		}
	}
	else if (return_black)
	{
		if (a > fade_speed)
		{
			a -= fade_speed;
		}
		else if (a > 0)
		{
			a--;
		}
	}
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (a >= 255)
	{
		go_black = false;
		FadeFromBlack(destination_level);
		title_screen = NULL;
	}
	else if (a <= 0)
	{
		return_black = false;
	}

	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 10;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 10;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 10;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 10;

	//Camera movement with the player
	uint x, y;
	app->win->GetWindowSize(x, y);
	app->render->camera.x = -app->scene->player->position.x + (x / 2);

	

	//Camera off map adjustment
	if (app->render->camera.x > 0)
		app->render->camera.x = 0;

	if (app->render->camera.x > 6001)
		app->render->camera.x = 6001;

	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

	// Draw map
	app->map->Draw();

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;
	
	if (title_screen != NULL)
	{
		app->render->DrawTexture(title_screen, 0, 0);
	}
	

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	int c_x = -app->render->camera.x;
	r.x = c_x;
	
	app->render->DrawRectangle(r, 0, 0, 0, a);


	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool Scene::NextLevel(int level) {

	if (title_screen != NULL) {
		SetA_Black();
		FadeFromBlack(level);
	}
	else {
		FadeToBlack(level);
	}

	title_screen = NULL;

	return true;
}

bool Scene::ReturnStartScreen()
{
	title_screen = app->tex->Load("Assets/textures/Start_screen.png");

	return true;
}

bool Scene::FadeToBlack(int level)
{
	go_black = true;
	if (level != -1) destination_level = level;

	return true;
}

bool Scene::FadeFromBlack(int level)
{
	return_black = true;
	if (level != -1)
	{

		switch (level)
		{
		case 0:
			ReturnStartScreen();
			break;
		case 1:

			app->SaveGameRequest();
			app->map->Load();
			app->audio->PlayMusic("Assets/Audio/Music/strike_the_earth.ogg");
			
			SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
				app->map->mapData.width,
				app->map->mapData.height,
				app->map->mapData.tileWidth,
				app->map->mapData.tileHeight,
				app->map->mapData.tilesets.Count());

			app->win->SetTitle(title.GetString());

			current_lvl = 1;
			break;

		}
	}

	return true;
}