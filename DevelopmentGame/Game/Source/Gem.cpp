#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Map.h"
#include "Gem.h"
#include "Defs.h"
#include "Log.h"

Gem::Gem() : Entity(EntityType::GEM)
{
	name.Create("gems_1");

	jewel.PushBack({ 0, 0, 54, 36 });
	jewel.speed = 0.1f;
}

// Destructor
Gem::~Gem()
{}

void Gem::InitCustomEntity()
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
	bodyFixture->SetUserData((void*)8);
}

bool Gem::Awake() {

	texturePath = parameters.attribute("texturepath").as_string();
	return true;
}

bool Gem::Start() {

	gemTexture = app->tex->Load(texturePath);

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	currentGemAnim = &jewel;

	// Add a physics to a gem- initialize the physics body
	pbody = app->physics->CreateRectangle(position.x, position.y, 20, 20, bodyType::KINEMATIC);

	// Assign collider type
	pbody->ctype = ColliderType::GEM;

	return true;
}

// Called each loop iteration
bool Gem::PreUpdate()
{
	position.x = pbody->body->GetPosition().x;
	position.y = pbody->body->GetPosition().y;

	return true;
}

// Called each loop iteration
bool Gem::Update()
{
	currentGemAnim->Update();

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	Draw();

	return true;
}

// Called each loop iteration
bool Gem::Draw()
{
	bool ret = true;

	SDL_Rect rect = currentGemAnim->GetCurrentFrame();

	if (plan_to_delete)
	{
		app->physics->world->DestroyBody(body);
		plan_to_delete = false;
	}

	if (!picked)
	{
		SDL_Rect rect = currentGemAnim->GetCurrentFrame();
		app->render->DrawTexture(gemTexture, position.x - 10, position.y + 2, &rect);
	}

	return ret;
}

bool Gem::DeleteEntity()
{
	picked = true;
	app->physics->world->DestroyBody(body);
	pbody->body->SetActive(false);

	return true;
}

bool Gem::Load(pugi::xml_node& data)
{
	std::string p = "position";
	std::string s = std::to_string(p_in_array);
	std::string t = p + s;
	const char* c = t.c_str();

	if (data.child("gems_1").child(c).attribute("state").as_int() == 0)
	{
		if (picked)
		{
			ReloadGem();
		}
	}
	else
	{
		picked = true;
	}

	return true;
}

bool Gem::Save(pugi::xml_node& data)
{
	std::string p = "position";
	std::string s = std::to_string(p_in_array);
	std::string t = p + s;
	const char* c = t.c_str();

	if (!picked)
	{
		data.child("gems_1").child(c).attribute("state").set_value("0");
	}
	else
	{
		data.child("gems_1").child(c).attribute("state").set_value("1");
	}

	if (!picked)
	{
		data.child("gems_2").child(c).attribute("state").set_value("0");
	}
	else
	{
		data.child("gems_2").child(c).attribute("state").set_value("1");
	}

	return true;
}

void Gem::ReloadGem()
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
	bodyFixture->SetUserData((void*)8);

	picked = false;
}