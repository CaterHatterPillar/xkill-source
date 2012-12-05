#pragma once

#include "EntityFactory.h"

#include <vector>
#include <iostream>

/// Manages multiple Entities in a uniform way.
/** 
\ingroup ARCHITECTURE
*/

enum ENTITYTYPE
{
	PLAYER,
	PROJECTILE
};

class EntityStorage
{
private:
	std::vector<Entity> entities;
	std::queue<int> deleted;
	int index;

public:
	EntityStorage()
	{
		// Creates Entity 0.
		// IMPORTANT: Entity 0 is used to mark deleted
		// Attributes and shouldn't be used in the game.
		createEntity();
	}

	~EntityStorage()
	{
		for(unsigned i=0; i<entities.size(); i++)
		{
			entities[i].deleteAttributes();
		}
	}

	Entity* createEntity()
	{
		// TRUE: Reuse attribute
		if(deleted.size() > 0)
		{
			index = deleted.front();
			deleted.pop();
		}
		// ELSE: Create new attribute
		else
		{
			index = (int)entities.size();
			entities.push_back(Entity(index));
		}

		// Get attribute
		return &entities[index];
	}

	void deleteEntity(int id)
	{
		// TRUE: Make sure no one is trying to delete "Entity 0"
		if(id == 0)
		{
			std::string message = "Trying to delete 'Entity 0'. Entity 0 is used to mark deleted Attributes and is not allowed to be deleted.";
			SHOW_MESSAGEBOX(message);
		}
		// ELSE: Delete Entity
		else
		{
			entities[id].deleteAttributes();
			deleted.push(id);
			std::cout << "ENTITYMANAGER: Removed Entity" << id << std::endl;
		}
	}
};

class EntityManager: public IObserver
{
private:
	EntityStorage entities;
	EntityFactory entityFactory;

	Entity* createEntity()
	{
		return entities.createEntity();
	}

public:
	EntityManager()
	{
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATEPROJECTILE);
	}

	/**
	Handles Events for EntityManager.
	*/
	void onEvent(Event* e)
	{
		Entity* entity;
		EventType type = e->getType();
		switch (type) 
		{
		case EVENT_CREATEPROJECTILE:
			event_CreateProjectile(static_cast<Event_createProjectile*>(e));
			break;
		default:
			break;
		}
	}

	void update(float delta)
	{
	}

	void createEntity(ENTITYTYPE entityType)
	{
		Entity* entity = createEntity();
		switch(entityType)
		{
		case PLAYER:
			entityFactory.createPlayerEntity(entity);
			std::cout << "ENTITYMANAGER: Created player entity " << entity->getID() << std::endl;
			break;
		}
	}

	void event_CreateProjectile(Event_createProjectile* e)
	{
		Entity* entity = createEntity();
		entityFactory.createProjectileEntity(entity, e);
		std::cout << "ENTITYMANAGER: Created projectile entity " << entity->getID() << std::endl;
	}
};