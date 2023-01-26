#ifndef __LIFE_H__
#define __LIFE_H__

#include "Module.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Physics.h"
#include "DynArray.h"
#include "Animation.h"

class Life : public Entity
{
public:
	Life();

	virtual ~Life();

private:

	void InitCustomEntity();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update();

	bool Draw();

	bool DeleteEntity();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

	void ReloadLife();

	bool picked = false;

	bool plan_to_delete = false;

	int w = 16, h = 16;

	const char* texturePath;
	SDL_Texture* lifeTexture;

	Animation chicken;
	Animation* currentLifeAnim = nullptr;

	PhysBody* pbody;

};

#endif
