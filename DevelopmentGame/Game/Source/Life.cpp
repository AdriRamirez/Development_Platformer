#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Map.h"
#include "Life.h"
#include "Defs.h"
#include "Log.h"

Life::Life() : Entity(EntityType::LIFE)
{
	name.Create("lifes");
	name.Create("lifes_2");

	chicken.PushBack({ 0, 0, 54, 36 });
	chicken.speed = 0.1f;
}

// Destructor
Life::~Life()
{}

void Life::InitCustomEntity()
{

	b2BodyDef c_body;
	c_body.type = b2_staticBody;
	c_body.position.Set(position.x, position.y);

	body = app->physics->world->CreateBody(&c_body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(w), PIXEL_TO_METERS(h), b2Vec2(PIXEL_TO_METERS(16), PIXEL_TO_METERS(16)), 0);

	b2FixtureDef fixture;
	fixture.shape = &box;
	b2Fixture* bodyFixture = body->CreateFixture(&fixture);
	bodyFixture->SetSensor(true);
	bodyFixture->SetUserData((void*)8); // coin collision
}

bool Life::Awake() {

	texturePath = parameters.attribute("texturepath").as_string();
	return true;
}

bool Life::Start() {


	lifeTexture = app->tex->Load(texturePath);

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	currentLifeAnim = &chicken;

	// Add a physics to an enemy - initialize the physics body
	pbody = app->physics->CreateRectangle(position.x, position.y, 40, 30, bodyType::KINEMATIC);

	// Assign collider type
	pbody->ctype = ColliderType::LIFE;

	return true;
}

// Called each loop iteration
bool Life::PreUpdate()
{
	position.x = pbody->body->GetPosition().x;
	position.y = pbody->body->GetPosition().y;

	return true;
}

// Called each loop iteration
bool Life::Update()
{
	currentLifeAnim->Update();

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	Draw();

	return true;
}

// Called each loop iteration
bool Life::Draw()
{
	bool ret = true;

	SDL_Rect rect = currentLifeAnim->GetCurrentFrame();

	if (plan_to_delete)
	{
		app->physics->world->DestroyBody(body);
		plan_to_delete = false;
	}

	if (!picked)
	{
		SDL_Rect rect = currentLifeAnim->GetCurrentFrame();
		app->render->DrawTexture(lifeTexture, position.x - 5, position.y - 2, &rect);
	}

	return ret;
}

bool Life::DeleteEntity()
{
	picked = true;
	//position.x = body->GetPosition().x;
	//position.y = body->GetPosition().y;
	//plan_to_delete = true;
	app->physics->world->DestroyBody(body);
	pbody->body->SetActive(false);

	return true;
}

bool Life::Load(pugi::xml_node& data)
{
	std::string p = "position";
	std::string s = std::to_string(p_in_array);
	std::string t = p + s;
	const char* c = t.c_str();

	if (data.child("lifes").child(c).attribute("state").as_int() == 0)
	{
		if (picked)
		{
			ReloadLife();
			app->entityManager->lifes--;
		}
	}
	else
	{
		picked = true;
	}

	if (data.child("lifes_2").child(c).attribute("state").as_int() == 0)
	{
		if (picked)
		{
			ReloadLife();
			app->entityManager->lifes--;
		}
	}
	else
	{
		picked = true;
	}
	return true;
}

bool Life::Save(pugi::xml_node& data)
{
	std::string p = "position";
	std::string s = std::to_string(p_in_array);
	std::string t = p + s;
	const char* c = t.c_str();

	if (!picked)
	{
		data.child("lifes").child(c).attribute("state").set_value("0");
	}
	else
	{
		data.child("lifes").child(c).attribute("state").set_value("1");
	}

	if (!picked)
	{
		data.child("lifes_2").child(c).attribute("state").set_value("0");
	}
	else
	{
		data.child("lifes_2").child(c).attribute("state").set_value("1");
	}

	return true;
}

void Life::ReloadLife()
{
	b2BodyDef c_body;
	c_body.type = b2_staticBody;
	c_body.position.Set(position.x, position.y);

	body = app->physics->world->CreateBody(&c_body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(w), PIXEL_TO_METERS(h), b2Vec2(PIXEL_TO_METERS(16), PIXEL_TO_METERS(16)), 0);

	b2FixtureDef fixture;
	fixture.shape = &box;
	b2Fixture* bodyFixture = body->CreateFixture(&fixture);
	bodyFixture->SetSensor(true);
	bodyFixture->SetUserData((void*)8); // coin collision

	picked = false;
}