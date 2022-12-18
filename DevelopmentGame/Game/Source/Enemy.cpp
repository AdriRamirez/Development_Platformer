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
#include "Entity.h"


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

	//texturePath = parameters.attribute("texturepath").as_string();

	texRight = parameters.attribute("texRight").as_string();
	texLeft = parameters.attribute("texLeft").as_string();

	return true;
}

bool Floor_Enemy::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	origin_x = position.x;
	origin_y = position.y;

	lookLeft = true;

	//initilize textures
	//texture = app->tex->Load(texturePath);
	textureRight = app->tex->Load(texRight);
	textureLeft = app->tex->Load(texLeft);
	speed = 1.0f;

	detectionRange = 500.00f;

	state = ENEMY_STATE::IDLE;
	obLeft = false;

	currentFloorEnemyAnimation = &walkAnimL;

	// Add a physics to an enemy - initialize the physics body
	pbody = app->physics->CreateRectangle(position.x, position.y, 40, 30, bodyType::KINEMATIC);

	// Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	return true;
}

bool Floor_Enemy::PreUpdate() {

	if (state != ENEMY_STATE::DEATH)
	{
		//position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
		//position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
		position.x = pbody->body->GetPosition().x;
		position.y = pbody->body->GetPosition().y;
		
		if (obLeft)
		{
			idleOb_x = origin_x - (32 * 10);
		}
		else
		{
			idleOb_x = origin_x + (32 * 10);
		}
	}

	return true;
}

bool Floor_Enemy::Update()
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	currentFloorEnemyAnimation->Update();

	// update path
	switch (state)
	{
	case ENEMY_STATE::IDLE:
		MoveGroundEnemy();
		break;
	case ENEMY_STATE::HUNT:
		//EnemyHunting();
		break;
	case ENEMY_STATE::RETURN:
		//EnemyReturning(dt);
		break;
	default:
		break;
	}

	if (pbody->body->GetLinearVelocity().x >= 0)
	{
		lookLeft = false;
	}
	else
	{
		lookLeft = true;
	}

	if (state != ENEMY_STATE::DEATH)
	{
		//CheckPlayer();
	}
	
	Draw();

	return true;
}

bool Floor_Enemy::Draw() {


	SDL_Rect rect = currentFloorEnemyAnimation->GetCurrentFrame();

	if (delete_enemy)
	{
		//app->physics->world->DestroyBody(pbody->body);
		delete_enemy = false;
	}

	if (state != ENEMY_STATE::DEATH)
	{
		if (lookLeft)
		{
			app->render->DrawTexture(textureLeft, position.x + -10, position.y, &rect);
		}
		else 
		{
			app->render->DrawTexture(textureRight, position.x + -10, position.y, &rect);
		}
		if (state == ENEMY_STATE::HUNT || state == ENEMY_STATE::RETURN)
		{
			if (app->physics->debug && path_save)
			{
				app->pathfinding->DrawPath(path_save, position, EntityType::FLOOR_ENEMY);
			}
		}
	}
	else
	{
		alive = false;
	}

	return true;
}
void Floor_Enemy::MoveGroundEnemy()
{
	if (!obLeft)
	{
		if (position.x < idleOb_x)
		{
			pbody->body->SetLinearVelocity({ speed, pbody->body->GetLinearVelocity().y });
		}
		else
		{
			obLeft = true;
		}

	}
	else if (obLeft)
	{
		if (position.x > idleOb_x)
		{
			pbody->body->SetLinearVelocity({ -speed, pbody->body->GetLinearVelocity().y });
		}
		else
		{
			obLeft = false;
		}
	}
}

bool Floor_Enemy::DeleteEntity()
{
	state = ENEMY_STATE::DEATH;
	delete_enemy = true;

	return true;
}

void Floor_Enemy::CheckPlayer()
{
	Entity* player = app->entityManager->GetPlayer();

	if (position.x + detectionRange > player->GetPlayerPosition().x && position.x - detectionRange < player->GetPlayerPosition().x
		&& position.y + detectionRange > player->GetPlayerPosition().y && position.y - detectionRange < player->GetPlayerPosition().y)
	{
		if (state != ENEMY_STATE::HUNT)
		{
			state = ENEMY_STATE::HUNT;
		}
	}
	else
	{
		if (state == ENEMY_STATE::HUNT)
		{
			state = ENEMY_STATE::RETURN;
		}
	}
}
void Floor_Enemy::EnemyHunting()
{
	PathFinding* path = new PathFinding();
	float dist;

	Entity* player = app->entityManager->GetPlayer();
	path->CreatePath({ (int)position.x, 0 }, { (int)player->GetPlayerPosition().x, 0 });
	int ob_x = path->GetLastPath()->At(path->GetLastPath()->Count() - 1)->x;

	if ((ob_x - position.x) > 0)
	{
		dist = 1.5f;
	}
	else
	{
		dist = -1.5f;
	}
	pbody->body->SetLinearVelocity({ dist * speed, pbody->body->GetLinearVelocity().y });

	path_save = path;
}
void Floor_Enemy::EnemyReturning()
{
	PathFinding* path = new PathFinding();

	path->CreatePath({ (int)position.x, 0 }, { (int)origin_x, 0 });
	int ob_x = path->GetLastPath()->At(path->GetLastPath()->Count() - 1)->x;

	pbody->body->SetLinearVelocity({ (ob_x - position.x) * speed, pbody->body->GetLinearVelocity().y });

	if (position.x + PIXEL_TO_METERS(1.5f) > origin_x && position.x - PIXEL_TO_METERS(1.5f) < origin_x)
	{
		state = ENEMY_STATE::IDLE;
	}

	if (position.y + PIXEL_TO_METERS(1.5f) > origin_y || position.y - PIXEL_TO_METERS(1.5f) < origin_y)
	{
		origin_x = position.x;
		origin_y = position.y;
	}

	path_save = path;
}
void Floor_Enemy::SwitchDirection()
{
	obLeft = !obLeft;
}


bool Floor_Enemy::CleanUp()
{
	return true;
}

//
//
//
//
//
// AIR ENEMY
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


	//texturePath = parameters.attribute("texturepath").as_string();

	texRight = parameters.attribute("texRight").as_string();
	texLeft = parameters.attribute("texLeft").as_string();

	return true;
}

bool Air_Enemy::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	origin_x = position.x;
	origin_y = position.y;


	textureRight = app->tex->Load(texRight);
	textureLeft = app->tex->Load(texLeft);

	speed = 1.00f;
	lookLeft = true;

	state = ENEMY_STATE::IDLE;

	currentAirEnemyAnimation = &flyAnimL;

	// L07 DONE 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircle(position.x, position.y, 25, bodyType::KINEMATIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ENEMYAIR;

	return true;
}

bool Air_Enemy::Update()
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x, position.y);

	currentAirEnemyAnimation->Update();

	// update path
	switch (state)
	{
	case ENEMY_STATE::IDLE:
		if (cd_air_enemy <= 0)
		{
			MoveAirEnemy();
			int u = 0;
		}
		else
		{
			cd_air_enemy -= 100;
		}

		CheckAirEnemy();
		break;
	}
	if (pbody->body->GetLinearVelocity().x >= 0)
	{
		lookLeft = false;
	}
	else
	{
		lookLeft = true;
	}

	SDL_Rect rect = currentAirEnemyAnimation->GetCurrentFrame();

	if (lookLeft)
	{
		app->render->DrawTexture(textureLeft, position.x  -20, position.y -20, &rect);
	}
	else
	{
		app->render->DrawTexture(textureRight, position.x  -20, position.y -20, &rect);
	}
	

	return true;
}

void Air_Enemy::MoveAirEnemy() {
	int mov = rand() % 4;

	switch (mov) {
	case 0:
		pbody->body->SetLinearVelocity({ 0, -speed });
		break;
	case 1: // down
		pbody->body->SetLinearVelocity({ 0, speed });
		break;
	case 2: // left
		pbody->body->SetLinearVelocity({ -speed, 0 });
		break;
	case 3: // right
		pbody->body->SetLinearVelocity({ speed, 0 });
		break;
	default:
		break;
	}	
	cd_air_enemy = 3200;
}
void Air_Enemy::CheckAirEnemy()
{
	int mov = 0;//rand() % 3;

	if (position.y + (32 * 10) < origin_y) // up
	{
		pbody->body->SetTransform({ pbody->body->GetPosition().x, origin_y - (32 * 10) }, 0);

		switch (mov)
		{
		case 0: // down
			pbody->body->SetLinearVelocity({ 0, speed });
			break;
		case 1: // left
			pbody->body->SetLinearVelocity({ -speed, 0 });
			break;
		case 2: // right
			pbody->body->SetLinearVelocity({ speed, 0 });
			break;
		default:
			break;
		}
	}
	else if (position.y -(32 * 10) > origin_y) // down
	{
		pbody->body->SetTransform({ pbody->body->GetPosition().x, origin_y + (32 * 10) }, 0);

		switch (mov)
		{
		case 0: // up
			pbody->body->SetLinearVelocity({ 0, -speed});
			break;
		case 1: // left
			pbody->body->SetLinearVelocity({ -speed, 0 });
			break;
		case 2: // right
			pbody->body->SetLinearVelocity({ speed, 0 });
			break;
		default:
			break;
		}
	}
	else if (position.x + (32 * 10) < origin_x) // left
	{
		pbody->body->SetTransform({ origin_x - (32 * 10), pbody->body->GetPosition().y }, 0);

		switch (mov)
		{
		case 0: // up
			pbody->body->SetLinearVelocity({ 0, -speed });
			break;
		case 1: // down
			pbody->body->SetLinearVelocity({ 0, speed });
			break;
		case 2: // right
			pbody->body->SetLinearVelocity({ speed, 0 });
			break;
		default:
			break;
		}
	}
	else if (position.x - (32 * 10) > origin_x) // right
	{
		pbody->body->SetTransform({ origin_x + (32 * 10), pbody->body->GetPosition().y }, 0);

		switch (mov)
		{
		case 0: // up
			pbody->body->SetLinearVelocity({ 0, -speed  });
			break;
		case 1: // down
			pbody->body->SetLinearVelocity({ 0, speed });
			break;
		case 2: // left
			pbody->body->SetLinearVelocity({ -speed, 0 });
			break;
		default:
			break;
		}
	}
}

bool Air_Enemy::PostUpdate() {


	return true;
}

bool Air_Enemy::CleanUp()
{
	return true;
}