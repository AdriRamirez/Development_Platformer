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
#include "Menu.h"

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

	//L02: DONE 3: Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");

	/*
	for (pugi::xml_node floorEnemyNode = config.child("floor_enemy"); floorEnemyNode; floorEnemyNode = floorEnemyNode.next_sibling("floor_enemy"))
	{
		Floor_Enemy* floor_enemy = (Floor_Enemy*)app->entityManager->CreateEntity(EntityType::FLOOR_ENEMY);
		floor_enemy->parameters = floorEnemyNode;
	}
	*/

	//Instantiate the enemies using the entity manager
	floor_enemy = (Floor_Enemy*)app->entityManager->CreateEntity(EntityType::FLOOR_ENEMY);
	floor_enemy->parameters = config.child("floor_enemy");

	air_enemy = (Air_Enemy*)app->entityManager->CreateEntity(EntityType::AIR_ENEMY);
	air_enemy->parameters = config.child("air_enemy");

	//Instantiate the lifes using the entity manager
	life = (Life*)app->entityManager->CreateEntity(EntityType::LIFE);
	life->parameters = config.child("lifes");

	life_2 = (Life*)app->entityManager->CreateEntity(EntityType::LIFE);
	life_2->parameters = config.child("lifes_2");

	//Instantiate the gems using the entity manager
	gem_1 = (Gem*)app->entityManager->CreateEntity(EntityType::GEM);
	gem_1->parameters = config.child("gems_1");

	gem_2 = (Gem*)app->entityManager->CreateEntity(EntityType::GEM);
	gem_2->parameters = config.child("gems_2");

	gem_3 = (Gem*)app->entityManager->CreateEntity(EntityType::GEM);
	gem_3->parameters = config.child("gems_3");

	gem_4 = (Gem*)app->entityManager->CreateEntity(EntityType::GEM);
	gem_4->parameters = config.child("gems_4");

	gem_5 = (Gem*)app->entityManager->CreateEntity(EntityType::GEM);
	gem_5->parameters = config.child("gems_5");

	gem_6 = (Gem*)app->entityManager->CreateEntity(EntityType::GEM);
	gem_6->parameters = config.child("gems_6");

	gem_7 = (Gem*)app->entityManager->CreateEntity(EntityType::GEM);
	gem_7->parameters = config.child("gems_7");

	gem_8 = (Gem*)app->entityManager->CreateEntity(EntityType::GEM);
	gem_8->parameters = config.child("gems_8");

	gem_9 = (Gem*)app->entityManager->CreateEntity(EntityType::GEM);
	gem_9->parameters = config.child("gems_9");

	gem_10 = (Gem*)app->entityManager->CreateEntity(EntityType::GEM);
	gem_10->parameters = config.child("gems_10");

	gem_11 = (Gem*)app->entityManager->CreateEntity(EntityType::GEM);
	gem_11->parameters = config.child("gems_11");

	gem_12 = (Gem*)app->entityManager->CreateEntity(EntityType::GEM);
	gem_12->parameters = config.child("gems_12");

	gem_13 = (Gem*)app->entityManager->CreateEntity(EntityType::GEM);
	gem_13->parameters = config.child("gems_13");

	gem_14 = (Gem*)app->entityManager->CreateEntity(EntityType::GEM);
	gem_14->parameters = config.child("gems_14");

	gem_15 = (Gem*)app->entityManager->CreateEntity(EntityType::GEM);
	gem_15->parameters = config.child("gems_15");
	
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	title_screen = app->tex->Load("Assets/Textures/levels/title_screen.png");
	app->audio->PlayMusic("Assets/Audio/Music/title_theme.ogg");

	a = 0;
	r = { 0, 0, 2560, 1440 };


	//  Set the window title with map/tileset info
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

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN){
		godMode = !godMode;
		if (!godMode) {
			b2Vec2 pos;
			pos.x = PIXEL_TO_METERS(player->position.x);
			pos.y = PIXEL_TO_METERS(player->position.y);
			player->pbody->body->SetTransform(pos, player->pbody->body->GetAngle());
		}
	}
		
	if (godMode) {
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			app->render->camera.y += 10;

		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			app->render->camera.y -= 10;

	}
	
	
	//Camera movement with the player
	uint x, y;
	app->win->GetWindowSize(x, y);
	app->render->camera.x = -app->scene->player->position.x + (x / 2);

	//Camera off map adjustment
	if (app->render->camera.x > 0) {
		app->render->camera.x = 0;
	}
	if (app->render->camera.x < -5600) {
		app->render->camera.x = -5600;
	}
		

	// Draw map
	app->map->Draw();
	
	if (app->scene->enemyDeleted) 
	{
		app->scene->floor_enemy->DeleteEntity();
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	//HUD render
	app->render->DrawTexture(hud, -app->render->camera.x, 0);
	FontDrawScore(score, 4, posicioFont + -app->render->camera.x , posicioFontY, 10, 1);

	//Life points render
	if (player->lifePoints >= 125)
	{
		app->render->DrawTexture(life_points_tex, -app->render->camera.x + 240, 16);
	}

	if (player->lifePoints >= 100)
	{
		app->render->DrawTexture(life_points_tex, -app->render->camera.x + 218, 16);
	}

	if (player->lifePoints >= 75)
	{
		app->render->DrawTexture(life_points_tex, -app->render->camera.x + 196, 16);
	}

	if (player->lifePoints >= 50)
	{
		app->render->DrawTexture(life_points_tex, -app->render->camera.x + 174, 16);
	}

	if (player->lifePoints >= 25)
	{
		app->render->DrawTexture(life_points_tex, -app->render->camera.x + 152, 16);
	}


	//Title screen render
	if (title_screen != NULL)
	{
		app->render->DrawTexture(title_screen, 0, 0);
	}
	

	int c_x = -app->render->camera.x;
	r.x = c_x;
	
	app->render->DrawRectangle(r, 0, 0, 0, a);


	return ret;
}

bool Scene::LoadState(pugi::xml_node& data)
{
	player->position.x = data.child("player").attribute("x").as_int();
	player->position.y = data.child("player").attribute("y").as_int();
	b2Vec2 pos;
	pos.x = PIXEL_TO_METERS(player->position.x);
	pos.y = PIXEL_TO_METERS(player->position.y);
	player->pbody->body->SetTransform(pos, player->pbody->body->GetAngle());
	//player->pbody->body->ApplyForceToCenter({ 0, 200 }, true);

	player->currentAnimation = &player->idleAnimR;
	if (app->menu->dead)
	{
		player->numLives = 2;
		player->lifePoints = 2;
		app->menu->dead = false;
		player->currentAnimation = &player->idleAnimR;
	}

	floor_enemy->position.x = data.child("floor_enemy").attribute("x").as_int();
	floor_enemy->position.y = data.child("floor_enemy").attribute("y").as_int();
	b2Vec2 fepos;
	fepos.x = PIXEL_TO_METERS(floor_enemy->position.x);
	fepos.y = PIXEL_TO_METERS(floor_enemy->position.y);

	air_enemy->position.x = data.child("air_enemy").attribute("x").as_int();
	air_enemy->position.y = data.child("air_enemy").attribute("y").as_int();
	b2Vec2 aepos;
	aepos.x = PIXEL_TO_METERS(air_enemy->position.x);
	aepos.y = PIXEL_TO_METERS(air_enemy->position.y);

	life->position.x = data.child("lifes").attribute("x").as_int();
	life->position.y = data.child("lifes").attribute("y").as_int();
	b2Vec2 lifepos;
	lifepos.x = PIXEL_TO_METERS(life->position.x);
	lifepos.y = PIXEL_TO_METERS(life->position.y);

	life_2->position.x = data.child("lifes_2").attribute("x").as_int();
	life_2->position.y = data.child("lifes_2").attribute("y").as_int();
	b2Vec2 lifepos2;
	lifepos2.x = PIXEL_TO_METERS(life_2->position.x);
	lifepos2.y = PIXEL_TO_METERS(life_2->position.y);

	gem_1->position.x = data.child("gems_1").attribute("x").as_int();
	gem_1->position.y = data.child("gems_1").attribute("y").as_int();
	b2Vec2 gem1pos;
	gem1pos.x = PIXEL_TO_METERS(gem_1->position.x);
	gem1pos.y = PIXEL_TO_METERS(gem_1->position.y);

	return true;
}

bool Scene::SaveState(pugi::xml_node& data)
{
	pugi::xml_node pos = data.append_child("player");
	pugi::xml_node fepos = data.append_child("floor_enemy");
	pugi::xml_node aepos = data.append_child("air_enemy");
	pugi::xml_node lifepos = data.append_child("lifes");
	pugi::xml_node lifepos2 = data.append_child("lifes_2");
	pugi::xml_node gem1pos = data.append_child("gems_1");

	pos.append_attribute("x") = player->position.x;
	pos.append_attribute("y") = player->position.y;

	fepos.append_attribute("x") = floor_enemy->position.x;
	fepos.append_attribute("y") = floor_enemy->position.y;

	aepos.append_attribute("x") = air_enemy->position.x;
	aepos.append_attribute("y") = air_enemy->position.y;

	lifepos.append_attribute("x") = life->position.x;
	lifepos.append_attribute("y") = life->position.y;

	lifepos2.append_attribute("x") = life_2->position.x;
	lifepos2.append_attribute("y") = life_2->position.y;


	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

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

			app->audio->name = false;
			app->SaveGameRequest();
			app->map->Load();
			app->audio->PlayMusic("Assets/Audio/Music/strike_the_earth.ogg");
			hud = app->tex->Load("Assets/Textures/hud/hud_life_and_gold.png");
			life_points_tex = app->tex->Load("Assets/Textures/hud/life_point.png");
			font = app->tex->Load("Assets/Textures/hud/fonts.png");
			
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

void Scene::FontDrawScore(int score, int n, int posX, int posY, int separacio, float scale) {
	int initialPosX = posX;
	int scoreCopia = score;
	int scoreArray[4];
	for (int j = 0; j < n; ++j) {
		scoreArray[j] = scoreCopia % 10;
		scoreCopia /= 10;
	}

	SDL_Rect rect0 = { 10 * 0, 0, 10, 10 };
	SDL_Rect rect1 = { 10 * 1, 0, 10, 10 };
	SDL_Rect rect2 = { 10 * 2, 0, 10, 10 };
	SDL_Rect rect3 = { 10 * 3, 0, 10, 10 };
	SDL_Rect rect4 = { 10 * 4, 0, 10, 10 };
	SDL_Rect rect5 = { 10 * 5, 0, 10, 10 };
	SDL_Rect rect6 = { 10 * 6, 0, 10, 10 };
	SDL_Rect rect7 = { 10 * 7, 0, 10, 10 };
	SDL_Rect rect8 = { 10 * 8, 0, 10, 10 };
	SDL_Rect rect9 = { 10 * 9, 0, 10, 10 };

	for (int k = 0; k < n; ++k) {

		switch (scoreArray[k]) {
		case 0:
			app->render->Blit(font, posX, posY, &rect0, scale);
			break;
		case 1:
			app->render->Blit(font, posX, posY, &rect1, scale);
			break;
		case 2:
			app->render->Blit(font, posX, posY, &rect2, scale);
			break;
		case 3:
			app->render->Blit(font, posX, posY, &rect3, scale);
			break;
		case 4:
			app->render->Blit(font, posX, posY, &rect4, scale);
			break;
		case 5:
			app->render->Blit(font, posX, posY, &rect5, scale);
			break;
		case 6:
			app->render->Blit(font, posX, posY, &rect6, scale);
			break;
		case 7:
			app->render->Blit(font, posX, posY, &rect7, scale);
			break;
		case 8:
			app->render->Blit(font, posX, posY, &rect8, scale);
			break;
		case 9:
			app->render->Blit(font, posX, posY, &rect9, scale);
			break;
		}

		posX -= separacio;
	}
	posX = initialPosX;
}