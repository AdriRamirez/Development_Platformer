#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Menu.h"
#include "EntityManager.h"
#include "Life.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

	//idle animation 
	idleAnimR.PushBack({ 4, 4, 68, 64 });
	idleAnimR.PushBack({ 76, 4, 68, 64 });
	idleAnimR.speed = 0.025f;

	idleAnimL.PushBack({ 4, 4, 68, 64 });
	idleAnimL.PushBack({ 76, 4, 68, 64 });
	idleAnimL.speed = 0.025f;

	//walk animation 
	walkAnimR.PushBack({ 4, 140, 80, 70 });
	walkAnimR.PushBack({ 88, 140, 80, 70 });
	walkAnimR.PushBack({ 172, 140, 80, 70 });
	walkAnimR.PushBack({ 256, 140, 80, 70 });
	walkAnimR.PushBack({ 340, 140, 80, 70 });
	walkAnimR.PushBack({ 424, 140, 80, 70 });
	walkAnimR.speed = 0.1f;

	walkAnimL.PushBack({ 4, 140, 80, 70 });
	walkAnimL.PushBack({ 88, 140, 80, 70 });
	walkAnimL.PushBack({ 172, 140, 80, 70 });
	walkAnimL.PushBack({ 256, 140, 80, 70 });
	walkAnimL.PushBack({ 340, 140, 80, 70 });
	walkAnimL.PushBack({ 424, 140, 80, 70 });
	walkAnimL.speed = 0.1f;

	//drift animation
	driftAnimR.PushBack({ 4, 214, 52, 62 });
	driftAnimR.speed = 0.1f;

	driftAnimL.PushBack({ 4, 214, 52, 62 });
	driftAnimL.speed = 0.1f;

	//jump animation
	jumpAnimR.PushBack({ 4, 282, 62, 68 });
	jumpAnimR.speed = 0.1f;

	jumpAnimL.PushBack({ 4, 282, 62, 68 });
	jumpAnimL.speed = 0.1f;

	//fall animation
	fallAnimR.PushBack({ 4, 354, 67, 68 });
	fallAnimR.speed = 0.1f;

	fallAnimL.PushBack({ 4, 354, 67, 68 });
	fallAnimL.speed = 0.1f;

	//double jump animation
	doublejumpAnimR.PushBack({ 4, 426, 96, 92 });
	doublejumpAnimR.PushBack({ 104, 426, 96, 92 });
	doublejumpAnimR.PushBack({ 204, 426, 96, 92 });
	doublejumpAnimR.PushBack({ 304, 426, 96, 92 });
	doublejumpAnimR.PushBack({ 404, 426, 96, 92 });
	doublejumpAnimR.PushBack({ 504, 426, 96, 92 });
	doublejumpAnimR.PushBack({ 604, 426, 96, 92 });
	doublejumpAnimR.speed = 0.1f;
	//doublejumpAnimR.loop = false;

	doublejumpAnimL.PushBack({ 4, 426, 96, 92 });
	doublejumpAnimL.PushBack({ 104, 426, 96, 92 });
	doublejumpAnimL.PushBack({ 204, 426, 96, 92 });
	doublejumpAnimL.PushBack({ 304, 426, 96, 92 });
	doublejumpAnimL.PushBack({ 404, 426, 96, 92 });
	doublejumpAnimL.PushBack({ 504, 426, 96, 92 });
	doublejumpAnimL.PushBack({ 604, 426, 96, 92 });
	doublejumpAnimL.speed = 0.07f;
	//doublejumpAnimL.loop = false;

	//death animation
	deathAnimR.PushBack({ 4, 1666, 74, 78 });
	deathAnimR.PushBack({ 82, 1666, 74, 78 });
	deathAnimR.PushBack({ 160, 1666, 74, 78 });
	deathAnimR.speed = 0.1f;

	deathAnimL.PushBack({ 4, 1666, 74, 78 });
	deathAnimL.PushBack({ 82, 1666, 74, 78 });
	deathAnimL.PushBack({ 160, 1666, 74, 78 });
	deathAnimL.speed = 0.1f;

	//dive animation
	diveAnimR.PushBack({ 4, 1006, 48, 72 });

	diveAnimL.PushBack({ 4, 1006, 48, 72 });

	//hit animation
	hitAnimR.PushBack({ 4, 1584, 70, 78 });
	hitAnimR.PushBack({ 74, 1584, 70, 78 });
	hitAnimR.PushBack({ 4, 1584, 70, 78 });
	hitAnimR.PushBack({ 74, 1584, 70, 78 });
	hitAnimR.PushBack({ 4, 1584, 70, 78 });
	hitAnimR.PushBack({ 74, 1584, 70, 78 });
	hitAnimR.speed = 0.4f;

	hitAnimL.PushBack({ 4, 1584, 70, 78 });
	hitAnimL.PushBack({ 74, 1584, 70, 78 });
	hitAnimL.PushBack({ 4, 1584, 70, 78 });
	hitAnimL.PushBack({ 74, 1584, 70, 78 });
	hitAnimL.PushBack({ 4, 1584, 70, 78 });
	hitAnimL.PushBack({ 74, 1584, 70, 78 });
	hitAnimL.speed = 0.4f;
}

Player::~Player() {

}

bool Player::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";

	//L02: DONE 5: Get Player parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	//texturePath = parameters.attribute("texturepath").as_string();

	//Animation textures from XML
	texRight = parameters.attribute("texRight").as_string();
	texLeft = parameters.attribute("texLeft").as_string();

	//Player Fx from XML
	jumpFx = parameters.attribute("jumpFx").as_string();
	landFx = parameters.attribute("landFx").as_string();
	hitFx = parameters.attribute("hitFx").as_string();

	return true;
}

bool Player::Start() {

	//texture = app->tex->Load(texturePath);
	textureRight = app->tex->Load(texRight);
	textureLeft = app->tex->Load(texLeft);
	lookLeft = false;

	jumpSound = app->audio->LoadFx(jumpFx);
	landSound = app->audio->LoadFx(landFx);
	hitSound = app->audio->LoadFx(hitFx);

	currentAnimation = &idleAnimR;

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x+16, position.y+16, 32, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this; 

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
	foodSound = app->audio->LoadFx("Assets/Audio/Fx/food.ogg");
	gemSound = app->audio->LoadFx("Assets/Audio/Fx/gem_01.ogg");
	enemySound = app->audio->LoadFx("Assets/Audio/Fx/character_poof_01.ogg");
	diamondSound = app->audio->LoadFx("Assets/Audio/Fx/diamond.ogg");

	inAir = false; //Check if player is on the ground or not

	return true;
}

// Called each loop iteration
bool Player::PreUpdate()
{
	//position.x = pbody->body->GetPosition().x;
	//position.y = pbody->body->GetPosition().y;


	return true;
}

bool Player::Update(float dt)
{

	// Add physics to the player - updated player position using physics

	float speed = 0.22f*dt;

	//Move left
	if (!app->menu->GetGameState()) {
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && hit == false)
		{
			if (!app->scene->godMode)
				pbody->body->SetLinearVelocity(b2Vec2(-speed, pbody->body->GetLinearVelocity().y));
			else if (app->scene->godMode && !app->menu->GetGameState())
				position.x -= 0.6f*dt;

			lookLeft = true;

			if (currentAnimation != &walkAnimL && !inAir)
			{
				walkAnimL.Reset();
				currentAnimation = &walkAnimL;
			}
		}

		else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP && hit == false)
		{

			pbody->body->SetLinearVelocity(b2Vec2(0, pbody->body->GetLinearVelocity().y));

			if (currentAnimation != &idleAnimL && !inAir)
			{
				idleAnimL.Reset();
				currentAnimation = &idleAnimL;
			}
		}
	}

	//Move right
	if (!app->menu->GetGameState()) {
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && hit == false)
		{
			if (!app->scene->godMode)
				pbody->body->SetLinearVelocity(b2Vec2(speed, pbody->body->GetLinearVelocity().y));
			else if (app->scene->godMode && !app->menu->GetGameState())
				position.x += 0.6f * dt;

			lookLeft = false;

			if (currentAnimation != &walkAnimR && !inAir)
			{
				walkAnimR.Reset();
				currentAnimation = &walkAnimR;
			}
		}

		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP && hit == false)
		{
			pbody->body->SetLinearVelocity(b2Vec2(0, pbody->body->GetLinearVelocity().y));

			if (currentAnimation != &idleAnimR && !inAir)
			{
				idleAnimR.Reset();
				currentAnimation = &idleAnimR;
			}
		}
	}

	//God Mode vertical movement
	if (app->scene->godMode && !app->menu->GetGameState()) {
		//Move Up
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			position.y -= 0.6f * dt;

			lookLeft = false;

			if (currentAnimation != &walkAnimR && !inAir)
			{
				walkAnimR.Reset();
				currentAnimation = &walkAnimR;
			}
		}
		//Move Down
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			position.y += 0.6f * dt;

			lookLeft = false;

			if (currentAnimation != &walkAnimR && !inAir)
			{
				walkAnimR.Reset();
				currentAnimation = &walkAnimR;
			}
		}
	}

	//Jump
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && hit == false)
	{
		if (!app->scene->godMode && !app->menu->GetGameState()) {
			if (!inAir)
			{
				pbody->body->SetLinearVelocity(b2Vec2(pbody->body->GetLinearVelocity().x, 0));
				pbody->body->ApplyForceToCenter({ 0, -400 }, true);
				app->audio->PlayFx(jumpSound);
				inAir = true;
				djump = true;
				rollin = false;
			}
			//Double jump
			else if (djump)
			{
				rollin = true;
				pbody->body->SetLinearVelocity(b2Vec2(pbody->body->GetLinearVelocity().x, 0));
				pbody->body->ApplyForceToCenter({ 0, -400 }, true);
				app->audio->PlayFx(jumpSound);
				djump = false;
			}
		}
	}

	//Dive
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && hit == false)
	{
		if (!app->scene->godMode && !app->menu->GetGameState()) {
			if (inAir)
			{
				pbody->body->ApplyForceToCenter({ 0, 400 }, true);
				dive = true;
				djump = false;
			}
		}
	}

	if (dive == true)
	{
		if (lookLeft)
		{
			if (currentAnimation != &diveAnimL)
			{
				diveAnimL.Reset();
				currentAnimation = &diveAnimL;
			}
		}
		else
		{
			if (currentAnimation != &diveAnimR)
			{
				diveAnimR.Reset();
				currentAnimation = &diveAnimR;
			}
		}
	}

	//Idle animation if player is not in the air and walking
	if (currentAnimation != &walkAnimL && currentAnimation != &walkAnimR && !inAir)
	{
		if (lookLeft)
		{
			if (currentAnimation != &idleAnimL)
			{
				idleAnimL.Reset();
				currentAnimation = &idleAnimL;
			}
		}
		else
		{
			if (currentAnimation != &idleAnimR)
			{
				idleAnimR.Reset();
				currentAnimation = &idleAnimR;
			}
		}
	}

	if (pbody->body->GetLinearVelocity().y < 0 && inAir)
	{
		if (rollin == false)
		{
			if (lookLeft)
			{
				if (currentAnimation != &jumpAnimL)
				{
					jumpAnimL.Reset();
					currentAnimation = &jumpAnimL;
				}
			}

			else
			{
				if (currentAnimation != &jumpAnimR)
				{
					jumpAnimR.Reset();
					currentAnimation = &jumpAnimR;
				}
			}
		}
		else
		{
			if (lookLeft)
			{
				if (currentAnimation != &doublejumpAnimL)
				{
					doublejumpAnimL.Reset();
					currentAnimation = &doublejumpAnimL;
				}
			}
			else
			{
				if (currentAnimation != &doublejumpAnimR)
				{
					doublejumpAnimR.Reset();
					currentAnimation = &doublejumpAnimR;
				}
			}
		}

	}
	else if (pbody->body->GetLinearVelocity().y > 3 && inAir && dive != true)
	{
		if (lookLeft)
		{
			if (currentAnimation != &fallAnimL)
			{
				fallAnimL.Reset();
				currentAnimation = &fallAnimL;
			}
		}
		else
		{
			if (currentAnimation != &fallAnimR)
			{
				fallAnimR.Reset();
				currentAnimation = &fallAnimR;
			}
		}
	}
	else if (pbody->body->GetLinearVelocity().x == 0 && pbody->body->GetLinearVelocity().y == 0)
	{
		if (lookLeft)
		{
			if (currentAnimation != &idleAnimL)
			{
				idleAnimL.Reset();
				currentAnimation = &idleAnimL;
			}
		}
		else
		{
			if (currentAnimation != &idleAnimR)
			{
				idleAnimR.Reset();
				currentAnimation = &idleAnimR;
			}
		}
	}

	currentAnimation->Update();


	if (pbody->body->GetLinearVelocity().x > -0.5f && pbody->body->GetLinearVelocity().x < 0.5f) {
		pbody->body->SetLinearVelocity(b2Vec2(0, pbody->body->GetLinearVelocity().y));
	}

	//Set the velocity of the pbody of the player
	//pbody->body->SetLinearVelocity(vel);

	if (!app->scene->godMode) {
		//Update player position in pixels
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 32;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 32;

	}


	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	if (lookLeft)
	{
		app->render->DrawTexture(textureLeft, position.x, position.y, &rect);
	}
	else
	{
		app->render->DrawTexture(textureRight, position.x, position.y, &rect);
	}

	//Player death conditions
	if (lifePoints < 25)
	{
		app->menu->dead = true;
	}

	//Player life limit
	if (lifePoints > 125)
	{
		lifePoints = 125;
	}

	
	//Player hit event
	if (hit)
	{
		lifePoints--;

			if (lookLeft)
			{
				if (currentAnimation != &hitAnimL)
				{
					hitAnimL.Reset();
					currentAnimation = &hitAnimL;
					pbody->body->SetLinearVelocity(b2Vec2(3, pbody->body->GetLinearVelocity().y));
				}
			}
			else
			{
				if (currentAnimation != &hitAnimR)
				{
					hitAnimR.Reset();
					currentAnimation = &hitAnimR;
					pbody->body->SetLinearVelocity(b2Vec2(-3, pbody->body->GetLinearVelocity().y));
				}
			}
	}

	return true;
	
}

bool Player::PostUpdate() {

	return true;
}

bool Player::CleanUp()
{
	return true;
}

fPoint Player::GetPlayerPosition()
{
	return position;
}



// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {


	switch (physB->ctype)
	{
		case ColliderType::ITEM:
			LOG("Collision ITEM");
			app->audio->PlayFx(pickCoinFxId);
			break;
		case ColliderType:: CHECKPOINT:
			if(!check1)
			{
				if (app->scene->player->position.x > 2000 && app->scene->player->position.x < 2200) {
					app->scene->checkpoint_1->DeleteEntity();
					app->SaveGameRequest();
					check1 = true;
				}
			}
			if (!check2)
			{
				if (app->scene->player->position.x > 3600 && app->scene->player->position.x < 3900) {
					app->scene->checkpoint_2->DeleteEntity();
					app->SaveGameRequest();
					check2 = true;
				}
			}
			break;
		case ColliderType::LIFE:
			LOG("Collision LIFE");
			if (!lifePicked)
			{
				if (app->scene->player->position.x > 1100 && app->scene->player->position.x < 1400)
				{
					app->audio->PlayFx(foodSound);
					lifePoints = lifePoints + 25;
					app->scene->life->DeleteEntity();
					lifePicked = true;
				}
			}	
			if (!lifePicked_2)
			{
				if (app->scene->player->position.x > 2180 && app->scene->player->position.x < 2580)
				{
					app->audio->PlayFx(foodSound);
					lifePoints = lifePoints + 25;
					app->scene->life_2->DeleteEntity();
					lifePicked_2 = true;
				}
			}
			break;
		case ColliderType::GEM:
			LOG("Collision GEM");
			if (!gem_1_Picked)
			{
				if (app->scene->player->position.x > 1036 && app->scene->player->position.x < 1236)
				{
					app->audio->PlayFx(gemSound);
					app->scene->gem_1->DeleteEntity();
					app->scene->score = app->scene->score + 100;
					gem_1_Picked = true;
				}
			}
			if (!gem_2_Picked)
			{
				if (app->scene->player->position.x > 1164 && app->scene->player->position.x < 1364)
				{
					app->audio->PlayFx(gemSound);
					app->scene->gem_2->DeleteEntity();
					app->scene->score = app->scene->score + 100;
					gem_2_Picked = true;
				}
			}
			if (!gem_3_Picked)
			{
				if (app->scene->player->position.x > 2220 && app->scene->player->position.x < 2420)
				{
					app->audio->PlayFx(gemSound);
					app->scene->gem_3->DeleteEntity();
					app->scene->score = app->scene->score + 100;
					gem_3_Picked = true;
				}
			}
			if (!gem_4_Picked)
			{
				if (app->scene->player->position.x > 2332 && app->scene->player->position.x < 2532)
				{
					app->audio->PlayFx(gemSound);
					app->scene->gem_4->DeleteEntity();
					app->scene->score = app->scene->score + 100;
					gem_4_Picked = true;
				}
			}
			if (!gem_5_Picked)
			{
				if (app->scene->player->position.x > 3372 && app->scene->player->position.x < 3572)
				{
					app->audio->PlayFx(gemSound);
					app->scene->gem_5->DeleteEntity();
					app->scene->score = app->scene->score + 100;
					gem_5_Picked = true;
				}
			}
			if (!gem_6_Picked)
			{
				if (app->scene->player->position.x > 3500 && app->scene->player->position.x < 3700)
				{
					app->audio->PlayFx(gemSound);
					app->scene->gem_6->DeleteEntity();
					app->scene->score = app->scene->score + 100;
					gem_6_Picked = true;
				}
			}
			if (!gem_7_Picked)
			{
				if (app->scene->player->position.x > 4940 && app->scene->player->position.x < 5140)
				{
					app->audio->PlayFx(gemSound);
					app->scene->gem_7->DeleteEntity();
					app->scene->score = app->scene->score + 100;
					gem_7_Picked = true;
				}
			}
			if (!gem_8_Picked)
			{
				if (app->scene->player->position.x > 5068 && app->scene->player->position.x < 5298)
				{
					app->audio->PlayFx(gemSound);
					app->scene->gem_8->DeleteEntity();
					app->scene->score = app->scene->score + 100;
					gem_8_Picked = true;
				}
			}
			if (!gem_9_Picked)
			{
				if (app->scene->player->position.x > 1100 && app->scene->player->position.x < 1300)
				{
					app->audio->PlayFx(gemSound);
					app->scene->gem_9->DeleteEntity();
					app->scene->score = app->scene->score + 200;
					gem_9_Picked = true;
				}
			}
			if (!gem_10_Picked)
			{
				if (app->scene->player->position.x > 2284 && app->scene->player->position.x < 2484)
				{
					app->audio->PlayFx(gemSound);
					app->scene->gem_10->DeleteEntity();
					app->scene->score = app->scene->score + 300;
					gem_10_Picked = true;
				}
			}
			if (!gem_11_Picked)
			{
				if (app->scene->player->position.x > 3436 && app->scene->player->position.x < 3636)
				{
					app->audio->PlayFx(gemSound);
					app->scene->gem_11->DeleteEntity();
					app->scene->score = app->scene->score + 400;
					gem_11_Picked = true;
				}
			}
			if (!gem_12_Picked)
			{
				if (app->scene->player->position.x > 5004 && app->scene->player->position.x < 5204)
				{
					app->audio->PlayFx(gemSound);
					app->scene->gem_12->DeleteEntity();
					app->scene->score = app->scene->score + 500;
					gem_12_Picked = true;
				}
			}
			if (!gem_13_Picked)
			{
				if (app->scene->player->position.x > 2700 && app->scene->player->position.x < 2900)
				{
					app->audio->PlayFx(diamondSound);
					app->scene->gem_13->DeleteEntity();
					app->scene->score = app->scene->score + 1000;
					gem_13_Picked = true;
				}
			}
			if (!gem_14_Picked)
			{
				if (app->scene->player->position.x > 3212 && app->scene->player->position.x < 3412)
				{
					app->audio->PlayFx(diamondSound);
					app->scene->gem_14->DeleteEntity();
					app->scene->score = app->scene->score + 1000;
					gem_14_Picked = true;
				}
			}
			if (!gem_15_Picked)
			{
				if (app->scene->player->position.x > 5932 && app->scene->player->position.x < 6132)
				{
					app->audio->PlayFx(diamondSound);
					app->scene->gem_15->DeleteEntity();
					app->scene->score = app->scene->score + 1000;
					gem_15_Picked = true;
				}
			}

			break;
		case ColliderType::PLATFORM:
			LOG("Collision PLATFORM");
			app->audio->PlayFx(landSound);
			inAir = false;
			dive = false;
			hit = false;
			break;
		case ColliderType::DEATH:
			LOG("Collision DEATH");
			app->menu->dead = true;
			break;
		case ColliderType::ENEMY:
			if (dive == true)
			{
				pbody->body->SetLinearVelocity(b2Vec2(pbody->body->GetLinearVelocity().x, -8));
				dive = false;
				app->scene->enemyDeleted = true;
				app->scene->floor_enemy->DeleteEntity();
				app->audio->PlayFx(enemySound);
			}
			else
			{
				app->audio->PlayFx(hitSound);
				hit = true;
			}
			break;
		case ColliderType::ENEMYAIR:
			if (dive == true)
			{
				pbody->body->SetLinearVelocity(b2Vec2(pbody->body->GetLinearVelocity().x, -8));
				dive = false;
				app->scene->enemyDeleted = true;
				app->entityManager->DestroyEntity(app->scene->air_enemy);
				app->scene->air_enemy->DeleteEntity();
				app->audio->PlayFx(enemySound);
			}
			else
			{
				app->audio->PlayFx(hitSound);
				hit = true;
			}
			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
	}
	

	
}
