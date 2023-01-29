#include "Checkpoint.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Checkpoint::Checkpoint() : Entity(EntityType::CHECKPOINT)
{
	name.Create("checkpoint");
}

Checkpoint::~Checkpoint() {}

bool Checkpoint::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Checkpoint::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateRectangle(position.x, position.y, 30, 60, bodyType::KINEMATIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::CHECKPOINT;

	return true;
}

bool Checkpoint::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 10;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 10;

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Checkpoint::DeleteEntity()
{
	
	pbody->body->SetActive(false);

	return true;
}
bool Checkpoint::CleanUp()
{
	return true;
}