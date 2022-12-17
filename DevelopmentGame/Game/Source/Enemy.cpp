#include "Enemy.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Floor_Enemy::Floor_Enemy() : Entity(EntityType::FLOOR_ENEMY)
{
	name.Create("floor_enemy");

	//walk animation 
	
	walkAnimR.PushBack({ 4, 4, 52, 32 });
	walkAnimR.PushBack({ 60, 4, 52, 32 });
	walkAnimR.PushBack({ 116, 4, 52, 32 });
	walkAnimR.PushBack({ 172, 4, 52, 32 });
	walkAnimR.speed = 0.1f;

	walkAnimL.PushBack({ 4, 4, 52, 32 });
	walkAnimL.PushBack({ 60, 4, 52, 32 });
	walkAnimL.PushBack({ 116, 4, 52, 32 });
	walkAnimL.PushBack({ 172, 4, 52, 32 });
	walkAnimL.speed = 0.1f;
	
}

Floor_Enemy::~Floor_Enemy() {}

bool Floor_Enemy::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	//texturePath = parameters.attribute("texturepath").as_string();

	texRight = parameters.attribute("texRight").as_string();
	texLeft = parameters.attribute("texLeft").as_string();

	return true;
}

bool Floor_Enemy::Start() {

	//initilize textures
	//texture = app->tex->Load(texturePath);
	textureRight = app->tex->Load(texRight);
	textureLeft = app->tex->Load(texLeft);

	currentFloorEnemyAnimation = &walkAnimL;

	// L07 DONE 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	return true;
}

bool Floor_Enemy::Update()
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	//app->render->DrawTexture(texture, position.x, position.y);

	currentFloorEnemyAnimation->Update();

	SDL_Rect rect = currentFloorEnemyAnimation->GetCurrentFrame();
	app->render->DrawTexture(textureLeft, position.x, position.y, &rect);

	return true;
}

bool Floor_Enemy::PostUpdate() {



	return true;
}

bool Floor_Enemy::CleanUp()
{
	return true;
}


Air_Enemy::Air_Enemy() : Entity(EntityType::AIR_ENEMY)
{
	name.Create("air_enemy");

	//walk animation 
	flyAnimR.PushBack({ 4, 4, 68, 64 });
	flyAnimR.PushBack({ 76, 4, 68, 64 });
	flyAnimR.PushBack({ 148, 4, 68, 64 });
	flyAnimR.speed = 0.1f;

	flyAnimL.PushBack({ 4, 4, 68, 64 });
	flyAnimL.PushBack({ 76, 4, 68, 64 });
	flyAnimL.PushBack({ 148, 4, 68, 64 });
	flyAnimL.speed = 0.1f;

}

Air_Enemy::~Air_Enemy() {}

bool Air_Enemy::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	//texturePath = parameters.attribute("texturepath").as_string();

	texRight = parameters.attribute("texRight").as_string();
	texLeft = parameters.attribute("texLeft").as_string();

	return true;
}

bool Air_Enemy::Start() {

	//initilize textures
	//texture = app->tex->Load(texturePath);
	textureRight = app->tex->Load(texRight);
	textureLeft = app->tex->Load(texLeft);

	currentAirEnemyAnimation = &flyAnimL;

	// L07 DONE 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::STATIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLATFORM;

	return true;
}

bool Air_Enemy::Update()
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x, position.y);

	currentAirEnemyAnimation->Update();

	SDL_Rect rect = currentAirEnemyAnimation->GetCurrentFrame();
	app->render->DrawTexture(textureLeft, position.x, position.y, &rect);

	return true;
}

bool Air_Enemy::PostUpdate() {



	return true;
}

bool Air_Enemy::CleanUp()
{
	return true;
}