#include "EntityManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Life.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	//L02: DONE 2: Instantiate entity according to the type and add the new entoty it to the list of Entities

	switch (type)
	{

	case EntityType::PLAYER:
		entity = new Player();
		break;

	case EntityType::ITEM:
		entity = new Item();
		break;

	case EntityType::LIFE:
		entity = new Life();
		break;

	case EntityType::GEM:
		entity = new Gem();
		break;

	case EntityType::CHECKPOINT:
		entity = new Checkpoint();
		break;

	case EntityType::FLOOR_ENEMY:
		entity = new Floor_Enemy();
		break;

	case EntityType::AIR_ENEMY:
		entity = new Air_Enemy();
		break;

	default: break;
	}

	// Created entities are added to the list
	AddEntity(entity);

	return entity;
}
void EntityManager::KillEnemy(fPoint pos)
{
	ListItem<Entity*>* item;
	Entity* entity = NULL;

	for (item = entities.start; item != NULL; item = item->next)
	{
		entity = item->data;

		if (entity->type == EntityType::FLOOR_ENEMY || entity->type == EntityType::AIR_ENEMY)
		{
			if (pos.x + 1.5f > entity->position.x && pos.x - 1.5f < entity->position.x && pos.y + 2.0f > entity->position.y && pos.y - 2.0f < entity->position.y)
			{
				entity->DeleteEntity();

				break;
			}
		}
	}
}
void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update(dt);
	}

	return ret;
}

void EntityManager::PickLife(fPoint pos)
{
	ListItem<Entity*>* item;
	Entity* entity = NULL;

	for (item = entities.start; item != NULL; item = item->next)
	{
		entity = item->data;

		if (pos.x + 1.5f > entity->position.x && pos.x - 1.5f < entity->position.x && pos.y + 2.0f > entity->position.y && pos.y - 2.0f < entity->position.y && entity->type == EntityType::LIFE)
		{
			entity->DeleteEntity();

			break;
		}
	}
}

bool EntityManager::PreUpdate()
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->PreUpdate();
	}

	return ret;
}

Entity* EntityManager::GetPlayer()
{
	ListItem<Entity*>* item;
	Entity* entity = NULL;

	for (item = entities.start; item != NULL; item = item->next)
	{
		entity = item->data;

		if (entity->type == EntityType::PLAYER)
		{
			return entity;
		}
	}

	return NULL;
}
bool Entity::Draw()
{
	return true;
}
void Entity::SwitchDirection()
{

}
fPoint Entity::GetPlayerPosition()
{
	return { 0, 0 };
}

bool Entity::DeleteEntity()
{
	return true;
}