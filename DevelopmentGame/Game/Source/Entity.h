#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

#include "Box2D/Box2D/Box2D.h"

class PhysBody;

enum class EntityType
{
	PLAYER,
	ITEM,
	FLOOR_ENEMY,
	AIR_ENEMY,
	LIFE,
	GEM,
	CHECKPOINT,
	UNKNOWN
};

class Entity
{
public:

	Entity(EntityType type) : type(type), active(true) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool PreUpdate()
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	void Entity::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Entity::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}
	virtual bool Draw();
	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {
	
	};

	virtual fPoint GetPlayerPosition();
	virtual void SwitchDirection();
	virtual bool DeleteEntity();

public:

	SString name;
	EntityType type;
	bool active = true;
	pugi::xml_node parameters;
	b2Body* body;

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	fPoint position;       
	bool renderable = true;
	int p_in_array;
	bool alive;

};

#endif // __ENTITY_H__