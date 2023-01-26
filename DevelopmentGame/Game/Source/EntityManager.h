#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "List.h"
#include "Point.h"

class EntityManager : public Module
{
public:

	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type);

	void DestroyEntity(Entity* entity);
	void KillEnemy(fPoint pos);
	void AddEntity(Entity* entity);
	Entity* GetPlayer();
public:

	List<Entity*> entities;
	int lifes = 0;
	char numlifes[4] = { "\0" };
	int gold = 2;
	char numgold[3] = { "\0" };

};

#endif // __ENTITYMANAGER_H__
