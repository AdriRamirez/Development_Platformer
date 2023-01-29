#ifndef __GEM_H__
#define __GEM_H__

#include "Module.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Physics.h"
#include "DynArray.h"
#include "Animation.h"

class Gem : public Entity
{
public:
	Gem();

	virtual ~Gem();

	bool DeleteEntity();

private:

	void InitCustomEntity();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool Draw();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

	void ReloadGem();

	bool picked = false;

	bool plan_to_delete = false;

	int w = 16, h = 16;

	const char* texturePath;
	SDL_Texture* gemTexture;

	Animation jewel;
	Animation* currentGemAnim = nullptr;

	PhysBody* pbody;

};

#endif

