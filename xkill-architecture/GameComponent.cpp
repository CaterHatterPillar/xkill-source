#include "GameComponent.h"
#include <xkill-utilities/AttributeManager.h>
#include <xkill-utilities/XKILL_Enums.h>
#include <xkill-utilities/MutatorSettings.h>
#include <DirectXMath.h>

#include <iostream>
#include <ctime>

// Iterators
ATTRIBUTES_DECLARE_ALL;


GameComponent::GameComponent(void)
{
	SUBSCRIBE_TO_EVENT(this, EVENT_PHYSICS_ATTRIBUTES_COLLIDING);
	SUBSCRIBE_TO_EVENT(this, EVENT_START_DEATHMATCH);
	SUBSCRIBE_TO_EVENT(this, EVENT_END_DEATHMATCH);
	SUBSCRIBE_TO_EVENT(this, EVENT_TRANSFEREVENTSTOGAME);
	SUBSCRIBE_TO_EVENT(this, EVENT_PLAYERDEATH);
}

GameComponent::~GameComponent(void)
{
	for(unsigned int i = 0; i < levelEvents_.size(); i++)
	{
		delete levelEvents_.at(i);
	}
	levelEvents_.clear();
}

bool GameComponent::init()
{
	//Fetch list of stuff used in logic
	GET_ENTITIES(allEntity);

	ATTRIBUTES_INIT_ALL;
	
	srand((unsigned)time(NULL));

	return true;
}

void GameComponent::onEvent(Event* e)
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_PHYSICS_ATTRIBUTES_COLLIDING:
		event_PhysicsAttributesColliding(static_cast<Event_PhysicsAttributesColliding*>(e));
		break;
	case EVENT_START_DEATHMATCH:
		event_StartDeathmatch(static_cast<Event_StartDeathmatch*>(e));
		break;
	case EVENT_END_DEATHMATCH:
		event_EndDeathmatch(static_cast<Event_EndDeathmatch*>(e));
		break;
	case EVENT_TRANSFEREVENTSTOGAME:
		event_TransferEventsToGame(static_cast<Event_TransferEventsToGame*>(e));
		break;
	case EVENT_PLAYERDEATH:
		event_PlayerDeath(static_cast<Event_PlayerDeath*>(e));
	default:
		break;
	}
}

void GameComponent::onUpdate(float delta)
{
	//
	// Update players
	//

	while(itrPlayer.hasNext())
	{
		// Fetch attributes through iterators
		AttributePtr<Attribute_Player>			player		=	itrPlayer		.getNext();

		AttributePtr<Attribute_Health>			health		=	player	->	ptr_health		;
		AttributePtr<Attribute_Camera>			camera		=	player	->	ptr_camera		;
		AttributePtr<Attribute_Input>			input		=	player	->	ptr_input		;
		AttributePtr<Attribute_Render>			render		=	player	->	ptr_render		;
		AttributePtr<Attribute_WeaponStats>		weaponStats	=	player	->	ptr_weaponStats	;
		AttributePtr<Attribute_Spatial>			spatial		=	render	->	ptr_spatial		;
		AttributePtr<Attribute_Position>		position	=	spatial	->	ptr_position	;
		AttributePtr<Attribute_Physics>			physics		=	input	->	ptr_physics		;

		Ammunition* ammo = &weaponStats->ammunition[weaponStats->currentAmmunitionType];
		FiringMode* firingMode = &weaponStats->firingMode[weaponStats->currentFiringModeType];

		//
		// End of deathmatch logic
		//

		if(player->priority >= 20)
		{
			SEND_EVENT(&Event(EVENT_GAME_OVER));
		}


		//
		// Ammunition logic
		//

		if(input->changeAmmunitionType)
		{
			input->changeAmmunitionType = false;
			weaponStats->currentAmmunitionType = static_cast<XKILL_Enums::AmmunitionType>((weaponStats->currentAmmunitionType + 1) % XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES);
			switchAmmunition(weaponStats);
			ammo = &weaponStats->ammunition[weaponStats->currentAmmunitionType];
			
			DEBUGPRINT(std::endl);
			DEBUGPRINT("Ammunition type: " << weaponStats->getAmmunitionTypeAsString());
			DEBUGPRINT("Firing mode: " << weaponStats->getFiringModeAsString());
		}

		if(input->changeFiringMode)
		{
			input->changeFiringMode = false;
			weaponStats->currentFiringModeType = static_cast<XKILL_Enums::FiringModeType>((weaponStats->currentFiringModeType + 1) % XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES);
			switchFiringMode(weaponStats);
			firingMode = &weaponStats->firingMode[weaponStats->currentFiringModeType];

			DEBUGPRINT(std::endl);
			DEBUGPRINT("Ammunition type: " << weaponStats->getAmmunitionTypeAsString());
			DEBUGPRINT("Firing mode: " << weaponStats->getFiringModeAsString());
		}


		//
		// Firing logic
		//
		if(health->health > 0.0f)
		{
			if((input->fire && firingMode->type == XKILL_Enums::FiringModeType::AUTO) || 
				input->firePressed && (firingMode->type == XKILL_Enums::FiringModeType::SINGLE || firingMode->type == XKILL_Enums::FiringModeType::SEMI))
			{
				input->fire = false;
				input->firePressed = false;

				if(firingMode->cooldownBetweenShots >= 0 && firingMode->cooldownLeft <= 0.0f
					&& firingMode->nrOfShotsLeftInClip > 0)
				{
					if(ammo->totalNrOfShots != -1) // special case: debug machine gun. Unlimited number of shots.
					{
						firingMode->cooldownLeft = firingMode->cooldownBetweenShots;
						ammo->totalNrOfShots--;
						firingMode->nrOfShotsLeftInClip--;
					}

					shootProjectile(position, camera, weaponStats);
					SEND_EVENT(&Event_PlaySound(Event_PlaySound::SOUND_FIRE, position->position, true));
				}
				else if(firingMode->nrOfShotsLeftInClip <= 0)
				{
					if(ammo->totalNrOfShots <= 0)
					{
						DEBUGPRINT("Cannot shoot: Out of ammo.");
					}
					else
					{
						DEBUGPRINT("Cannot shoot: Out of ammo in current clip.");
					}
				}
				else if(firingMode->cooldownLeft > 0)
				{
					DEBUGPRINT("Cannot shoot: weapon cooldown. Be patient.");
				}
			}
		}
		if(input->killPlayer)
		{
			health->health = 0.0f;
			input->killPlayer = false;
			player->detectedAsDead = true;
			player->currentRespawnDelay = 0.0f;
		}

		if(input->sprint && player->canSprint)
		{
			player->currentSprintTime -= delta;
			if(player->currentSprintTime < 0)
				player->canSprint = false;

			player->currentSpeed = player->sprintSpeed;
			input->sprint = false;
		}
		else
		{
			player->currentSprintTime += delta * player->sprintRechargeRate;
	
			if(player->currentSprintTime > player->sprintTime)
			{
				player->currentSprintTime = player->sprintTime;
				player->canSprint = true;
			}

			player->currentSpeed = player->walkSpeed;
		}

		//
		// Health and respawn logic
		//

		// Detect player death
		if(health->health <= 0.0f && !player->detectedAsDead) 
		{
			SEND_EVENT(&Event_PlayerDeath(itrPlayer.storageIndex()));
		}

		//Handle dead players
		if(player->detectedAsDead)
		{
			if(player->currentRespawnDelay > 0.0f)
			{
				player->currentRespawnDelay -= delta;

				float alive = 3.14f/4.0f;
				float dead = 3.14f/3.0f;
				float slerp = (1 - player->currentRespawnDelay/player->respawnDelay);
				float fov = slerp*dead + (1-slerp)*alive;
				player->ptr_camera->fieldOfView = fov;
			}
			else
			{
				player->ptr_camera->fieldOfView =3.14f/4.0f;
				// If an appropriate spawnpoint was found: spawn at it; otherwise: spawn at origo.
				AttributePtr<Attribute_PlayerSpawnPoint> ptr_spawnPoint = findUnoccupiedSpawnPoint();
				if(ptr_spawnPoint.isNotEmpty())
				{
					AttributePtr<Attribute_Position> ptr_spawnPoint_position = ptr_spawnPoint->ptr_position;
					position->position = ptr_spawnPoint_position->position; // set player position attribute
					DEBUGPRINT("Player entity " << itrPlayer.ownerId() << " spawned at " << position->position.x << " " << position->position.y << " " << position->position.z << std::endl);
				}
				else
				{
					position->position = Float3(0.0f, 0.0f, 0.0f);
					DEBUGPRINT("No spawn point was found. Player entity " << itrPlayer.ownerId() << " spawned at " << position->position.x << " " << position->position.y << " " << position->position.z << std::endl);
				}

				player->currentRespawnDelay = player->respawnDelay;

				physics->gravity = Float3(0.0f, -10.0f, 0.0f);
				physics->collisionFilterMask = Attribute_Physics::EVERYTHING;
				physics->collisionResponse = true;
				physics->meshID = player->meshIDWhenAlive;

				spatial->rotation = Float4(0.0f, 0.0f, 0.0f, 1.0f);
				camera->up = Float3(0.0f, 1.0f, 0.0f);
				camera->right = Float3(1.0f, 0.0f, 0.0f);
				camera->look = Float3(0.0f, 0.0f, 1.0f);
				//camera->reset = true; //Reset player rotation.
				physics->reloadDataIntoBulletPhysics = true;

				player->detectedAsDead = false;

				health->health = health->startHealth; // restores player health
				SEND_EVENT(&Event_PlaySound(Event_PlaySound::SOUND_RESPAWN, position->position, true));
			}
		}

		player->timeSinceLastJump += delta;
	}


	//
	// Update projectiles
	//

	while(itrProjectile.hasNext())
	{
		// Fetch attributes
		AttributePtr<Attribute_Projectile> projectile = itrProjectile.getNext();

		//
		// Update projectile lifetime
		//

		projectile->currentLifeTimeLeft -= delta;
		if(projectile->currentLifeTimeLeft <= 0)
		{
			DEBUGPRINT("Projectile entity " << itrProjectile.ownerId() << " has no lifetime left");

			// remove projectile entity
			SEND_EVENT(&Event_RemoveEntity(itrProjectile.ownerId()));
		}
	}

	//
	// Update player spawn points
	//

	
	while(itrPlayerSpawnPoint.hasNext())
	{
		AttributePtr<Attribute_PlayerSpawnPoint> ptr_spawnPoint	= itrPlayerSpawnPoint.getNext();
		ptr_spawnPoint->secondsSinceLastSpawn += delta;
	}

	//
	// Update pickupables spawn points
	//

	while(itrPickupablesSpawnPoint.hasNext())
	{
		AttributePtr<Attribute_PickupablesSpawnPoint> ptr_pickupablesSpawnPoint = itrPickupablesSpawnPoint.getNext();
		
		//Timer incrementation
		ptr_pickupablesSpawnPoint->secondsSinceLastPickup += delta;
		ptr_pickupablesSpawnPoint->secondsSinceLastSpawn += delta;

		//Spawn new pickupable, if secondsSinceLastSpawn and secondsSinceLastPickup are enough incremented
		if(ptr_pickupablesSpawnPoint->secondsSinceLastSpawn > ptr_pickupablesSpawnPoint->spawnDelayInSeconds && ptr_pickupablesSpawnPoint->secondsSinceLastPickup > ptr_pickupablesSpawnPoint->spawnDelayInSeconds)
		{
			if(ptr_pickupablesSpawnPoint->currentNrOfExistingSpawnedPickupables < ptr_pickupablesSpawnPoint->maxNrOfExistingSpawnedPickupables)
			{
				AttributePtr<Attribute_Position> pickupablesSpawnPointPosition = ptr_pickupablesSpawnPoint->ptr_position;

				int amount;
				switch(ptr_pickupablesSpawnPoint->spawnPickupableType)
				{
				case XKILL_Enums::PickupableType::MEDKIT:
					amount = 1;
						break;
				case XKILL_Enums::PickupableType::AMMUNITION_BULLET:
					amount = 100;
						break;
				case XKILL_Enums::PickupableType::AMMUNITION_SCATTER:
					amount = 50;
						break;
				case XKILL_Enums::PickupableType::AMMUNITION_EXPLOSIVE:
					amount = 10;
					break;
				}

				//Each pickupable knows it pickupablesSpawnPoint creator
				AttributePtr<Attribute_PickupablesSpawnPoint> ptr_creator_pickupablesSpawnPoint = ptr_pickupablesSpawnPoint;
				SEND_EVENT(&Event_CreatePickupable(pickupablesSpawnPointPosition->position, ptr_pickupablesSpawnPoint->spawnPickupableType, ptr_creator_pickupablesSpawnPoint, amount));
				ptr_pickupablesSpawnPoint->secondsSinceLastSpawn = 0.0f;
			}
		}
	}

	//check
	//while(itrPickupable.hasNext())
	//{
	//	Attribute_Pickupable* pickupable = itrPickupable.getNext();
	//	pickupable;
	//}

	//
	// Update weapons stats
	//

	while(itrWeaponStats.hasNext())
	{
		// Fetch attribute
		AttributePtr<Attribute_WeaponStats> weaponStats = itrWeaponStats.getNext();

		Ammunition* ammo = &weaponStats->ammunition[weaponStats->currentAmmunitionType];
		FiringMode* firingMode = &weaponStats->firingMode[weaponStats->currentFiringModeType];

		//
		// Weapon cooldown logic
		//

		firingMode->cooldownLeft -= delta;


		//
		// Weapon reload logic
		//

		if(ammo->totalNrOfShots > 0 && firingMode->nrOfShotsLeftInClip <= 0)
		{
			firingMode->reloadTimeLeft -= delta;
			if(firingMode->reloadTimeLeft <= 0)
			{
				firingMode->reloadTimeLeft = firingMode->reloadTime;

				if(firingMode->clipSize > ammo->totalNrOfShots)
				{
					firingMode->nrOfShotsLeftInClip = ammo->totalNrOfShots;
				}
				else
				{
					firingMode->nrOfShotsLeftInClip = firingMode->clipSize;
				}

				DEBUGPRINT("Weapon was automatically reloaded.");
				DEBUGPRINT("Ammo in current clip: " << firingMode->nrOfShotsLeftInClip);
				DEBUGPRINT("Total number of shots left: " << ammo->totalNrOfShots);
			}
		}

	}


	//
	// Update explosion sphere
	//

	while(itrExplosionSphere.hasNext())
	{
		// Fetch attributes
		AttributePtr<Attribute_ExplosionSphere> ptr_explosionSphere = itrExplosionSphere.getNext();


		//
		// Lifetime logic
		//

		ptr_explosionSphere->currentLifeTimeLeft -= delta;
		if(ptr_explosionSphere->currentLifeTimeLeft <= 0.0f)
		{
			SEND_EVENT(&Event_RemoveEntity(itrExplosionSphere.ownerId()));
		}
	}

	//
	// Update offset
	//

	while(itrCamera.hasNext())
	{
		AttributePtr<Attribute_Camera> ptr_camera = itrCamera.getNext();
		AttributePtr<Attribute_Spatial> ptr_spatial = ptr_camera->ptr_spatial;

		// Update rotation quaternion so other can read from it
		{
			DirectX::XMMATRIX xm_view = DirectX::XMLoadFloat4x4((DirectX::XMFLOAT4X4*)&ptr_camera->mat_view);
			xm_view = DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(xm_view), xm_view);
			DirectX::XMVECTOR xv_rot = DirectX::XMQuaternionRotationMatrix(xm_view);
			DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)&ptr_spatial->rotation, xv_rot);
		}
	}

	while(itrOffset.hasNext())
	{
		AttributePtr<Behavior_Offset> ptr_offset = itrOffset.getNext();
		ptr_offset->updateOffset();
	}
}

void GameComponent::event_PhysicsAttributesColliding(Event_PhysicsAttributesColliding* e)
{
	// Fetch Entities so we can inspect their attributes
	Entity* entity1 = &allEntity->at(itrPhysics.ownerIdAt(e->attribute1_index));
	Entity* entity2 = &allEntity->at(itrPhysics.ownerIdAt(e->attribute2_index));
	
	// Handle hit reaction on entity 1
	// when colliding with entity 2

	// health
	if(entity1->hasAttribute(ATTRIBUTE_HEALTH))
	{
		//
		// colliding with...
		//

		// damage
		if(entity2->hasAttribute(ATTRIBUTE_DAMAGE))
		{
			// fetch damage from entity 2
			std::vector<int> damageId = entity2->getAttributes(ATTRIBUTE_DAMAGE);

			// fetch health from entity 1
			std::vector<int> healthId = entity1->getAttributes(ATTRIBUTE_HEALTH);

			for(unsigned i=0; i<damageId.size(); i++)
			{
				AttributePtr<Attribute_Damage> damage = itrDamage.at(damageId[i]);

				// avoid damage to self
				if(entity1->getID() != damage->owner_entityID || entity2->hasAttribute(ATTRIBUTE_EXPLOSIONSPHERE))
				{
					Float3 position;
					bool use3DAudio = false;

					if(entity1->hasAttribute(ATTRIBUTE_POSITION))
					{
						std::vector<int> positionID = entity1->getAttributes(ATTRIBUTE_POSITION);

						for(unsigned int i = 0; i < positionID.size(); i++)
						{
							Attribute_Position* pos = itrPosition.at(positionID[i]);

							position = pos->position;
							use3DAudio = true;
						}
					}

					// Apply damage to all Health attributes
					for(unsigned j=0; j<healthId.size(); j++)
					{
						AttributePtr<Attribute_Health> health = itrHealth.at(healthId[j]);
						health->health -= damage->damage;
						
						// If a player was killed by the collision, give priority (score) to the player that created the DamageAttribute
						if(health->health <= 0)
						{
							Entity* creatorOfProjectilePlayerEntity = &allEntity->at(damage->owner_entityID);
							std::vector<int> playerId = creatorOfProjectilePlayerEntity->getAttributes(ATTRIBUTE_PLAYER);

							for(unsigned k=0;k<playerId.size();k++)
							{
								AttributePtr<Attribute_Player> ptr_player = itrPlayer.at(playerId.at(k));
								
								//Award player
								if(entity1->getID() != damage->owner_entityID)
								{
									ptr_player->priority++;
								}
								else //Punish player for blowing himself up
								{
									ptr_player->priority--;
								}
								
								DEBUGPRINT("Player with entity id " << damage->owner_entityID << " killed player with entity id " << entity1->getID());
							}

							Entity* playerThatDied = &allEntity->at(itrHealth.ownerIdAt(healthId[j]));
							playerId = playerThatDied->getAttributes(ATTRIBUTE_PLAYER);

							for(unsigned int k = 0; k < playerId.size(); k++)
							{
								SEND_EVENT(&Event_PlayerDeath(playerId[k]));
								SEND_EVENT(&Event_PlaySound(Event_PlaySound::SOUND_DEATH, position, use3DAudio));
							}
						}
						else
						{
							SEND_EVENT(&Event_PlaySound(Event_PlaySound::SOUND_HIT, position, use3DAudio));
						}

						SEND_EVENT(&Event_Rumble(entity1->getID(), true, 0.2f, 1.0f, 1.0f));

						DEBUGPRINT("DAMAGEEVENT Entity " << entity2->getID() << " damage: " <<  damage->damage << " Entity " << entity1->getID() << " health " << health->health);
					}

					if(entity2->hasAttribute(ATTRIBUTE_PROJECTILE))
					{
						SEND_EVENT(&Event_RemoveEntity(entity2->getID()));
					}
				}
			}
		}
	}
	// projectile
	else if(entity1->hasAttribute(ATTRIBUTE_PROJECTILE))
	{
		//
		// colliding with...
		//

		if(entity2->hasAttribute(ATTRIBUTE_PHYSICS)) //May not be needed
		{
			//Set gravity on projectiles colliding with physics objects
			std::vector<int> physicsId = entity1->getAttributes(ATTRIBUTE_PHYSICS);
			for(unsigned i=0;i<physicsId.size();i++)
			{
				AttributePtr<Attribute_Physics> ptr_physics = itrPhysics.at(physicsId.at(i));
				SEND_EVENT(&Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData::GRAVITY, static_cast<void*>(&Float3(0.0f, -10.0f, 0.0f)), physicsId.at(i)));
				//physicsAttribute->gravity = Float3(0.0f, -10.0f, 0.0f);
				//physicsAttribute->linearVelocity = Float3(0.0f, 0.0f, 0.0f);
				//physicsAttribute->reloadDataIntoBulletPhysics = true;
			}

			//Handle PhysicsAttribute of a projectile colliding with another PhysicsAttribute
			std::vector<int> projectileId = entity1->getAttributes(ATTRIBUTE_PROJECTILE);
			for(unsigned i=0;i<projectileId.size();i++)
			{
				AttributePtr<Attribute_Projectile> ptr_projectile = itrProjectile.at(projectileId.at(i));

				//Shorten lifetime of projectile colliding with physics objects
				switch(ptr_projectile->ammunitionType)
				{
				case XKILL_Enums::AmmunitionType::BULLET:
					if(ptr_projectile->currentLifeTimeLeft > 0.15f)
					{
						ptr_projectile->currentLifeTimeLeft = 0.15f;
					}
					break;
				case XKILL_Enums::AmmunitionType::SCATTER:
					if(ptr_projectile->currentLifeTimeLeft > 1.00f)
					{
						ptr_projectile->currentLifeTimeLeft = 1.00f;
						//SEND_EVENT(&Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData::VELOCTIY, static_cast<void*>(&Float3(0.0f, 0.0f, 0.0f)), physicsId.at(i)));
					}
					break;
				}

				//Explosion handling.
				if(ptr_projectile->ammunitionType == XKILL_Enums::AmmunitionType::EXPLOSIVE)
				{
					//Get damage from projectile.
					AttributePtr<Attribute_Damage> ptr_projectileDamage;
					if(entity1->hasAttribute(ATTRIBUTE_DAMAGE))
					{
						std::vector<int> damageId = entity1->getAttributes(ATTRIBUTE_DAMAGE);
						for(unsigned i=0;i<damageId.size();i++)
						{
							ptr_projectileDamage = itrDamage.at(damageId.at(i));
						}
					}

					//Kill the projectile that caused the explosion
 					ptr_projectile->currentLifeTimeLeft = 0.0f;

					//Extract projectile position.
					AttributePtr<Attribute_Physics> ptr_projectile_physics	 = ptr_projectile->ptr_physics;
					AttributePtr<Attribute_Spatial> ptr_projectile_spatial	 = ptr_projectile_physics->ptr_spatial;
					AttributePtr<Attribute_Position> ptr_projectile_position	 = ptr_projectile_spatial->ptr_position;

					//Creates an explosion sphere. Init information is taken from the impacting projectile.
					SEND_EVENT(&Event_CreateExplosionSphere(ptr_projectile_position->position, ptr_projectileDamage->damage, ptr_projectile->entityIdOfCreator, ptr_projectile->ammunitionType, ptr_projectile->firingModeType));
				}
			}
			//SEND_EVENT(&Event_RemoveEntity(entity1->getID())); //Crashes sometimes if removed here
		}
	}
	//Pickupables
	else if(entity1->hasAttribute(ATTRIBUTE_PICKUPABLE))
	{
		AttributePtr<Attribute_Pickupable> ptr_pickupable;
		if(entity2->hasAttribute(ATTRIBUTE_PLAYER))
		{
			//Retrieve player attribute
			std::vector<int> playerId = entity2->getAttributes(ATTRIBUTE_PLAYER);
			for(unsigned i=0;i<playerId.size();i++)
			{
				AttributePtr<Attribute_Player> ptr_player = itrPlayer.at(playerId.at(i));

				//Retrieve pickupable attribute
				std::vector<int> pickupablesId = entity1->getAttributes(ATTRIBUTE_PICKUPABLE);
				for(unsigned i=0;i<pickupablesId.size();i++)
				{
					ptr_pickupable = itrPickupable.at(pickupablesId.at(i));
					switch(ptr_pickupable->pickupableType)
					{
						case XKILL_Enums::PickupableType::MEDKIT:
						{
								AttributePtr<Attribute_Health> ptr_health = ptr_player->ptr_health;
								ptr_health->health += ptr_pickupable->amount;
								break;
						}
						case XKILL_Enums::PickupableType::AMMUNITION_BULLET:
						{
								AttributePtr<Attribute_WeaponStats> weaponStatsAttribute = ptr_player->ptr_weaponStats;
								weaponStatsAttribute->ammunition[XKILL_Enums::AmmunitionType::BULLET].totalNrOfShots += ptr_pickupable->amount;
								break;
						}
						case XKILL_Enums::PickupableType::AMMUNITION_EXPLOSIVE:
						{
								AttributePtr<Attribute_WeaponStats> weaponStatsAttribute = ptr_player->ptr_weaponStats;
								weaponStatsAttribute->ammunition[XKILL_Enums::AmmunitionType::EXPLOSIVE].totalNrOfShots += ptr_pickupable->amount;
								break;
						}
						case XKILL_Enums::PickupableType::AMMUNITION_SCATTER:
						{
								AttributePtr<Attribute_WeaponStats> weaponStatsAttribute = ptr_player->ptr_weaponStats;
								weaponStatsAttribute->ammunition[XKILL_Enums::AmmunitionType::SCATTER].totalNrOfShots += ptr_pickupable->amount;
								break;
						}
					}
				}
			}

			// Decrement number of spawned pickupables for the spawnpoint that spawned the pickupable that the player picked up. Also remove it.
			AttributePtr<Attribute_PickupablesSpawnPoint> ptr_pickupablesSpawnPoint = ptr_pickupable->ptr_pickupablesSpawnPoint_creator;
			ptr_pickupablesSpawnPoint->currentNrOfExistingSpawnedPickupables--;
			ptr_pickupablesSpawnPoint->secondsSinceLastPickup = 0;
			
			SEND_EVENT(&Event_RemoveEntity(entity1->getID()));
		}
	}
	else if(entity1->hasAttribute(ATTRIBUTE_PHYSICS))
	{
		//Player colliding with world
		if(entity2->hasAttribute(ATTRIBUTE_PLAYER))
		{
			std::vector<int> playerId = entity2->getAttributes(ATTRIBUTE_PLAYER);
			for(int i=0; i<(int)playerId.size(); i++)
			{
				AttributePtr<Attribute_Player> ptr_player = itrPlayer.at(playerId.at(i));
				ptr_player->collidingWithWorld = true;
			}
		}
	}
}

void GameComponent::event_EndDeathmatch(Event_EndDeathmatch* e)
{
	// Delete players
	while(itrPlayer.hasNext())
	{
		itrPlayer.getNext();
		SEND_EVENT(&Event_RemoveEntity(itrPlayer.ownerId()));
	}

	while(itrPhysics.hasNext())
	{
		itrPhysics.getNext();
		SEND_EVENT(&Event_RemoveEntity(itrPhysics.ownerId()));
	}
	while(itrPlayerSpawnPoint.hasNext())
	{
		itrPlayerSpawnPoint.getNext();
		SEND_EVENT(&Event_RemoveEntity(itrPlayerSpawnPoint.ownerId()));
	}

	while(itrLightDir.hasNext())
	{
		itrLightDir.getNext();
		SEND_EVENT(&Event_RemoveEntity(itrLightDir.ownerId()));
	}

	while(itrLightPoint.hasNext())
	{
		itrLightPoint.getNext();
		SEND_EVENT(&Event_RemoveEntity(itrLightPoint.ownerId()));
	}

	while(itrLightSpot.hasNext())
	{
		itrLightSpot.getNext();
		SEND_EVENT(&Event_RemoveEntity(itrLightSpot.ownerId()));
	}

	DEBUGPRINT("x--------------x");
	DEBUGPRINT("-x------------x-");
	DEBUGPRINT("DEATHMATCH ENDED");
	DEBUGPRINT("-x------------x-");
	DEBUGPRINT("x--------------x");
}

AttributePtr<Attribute_PlayerSpawnPoint> GameComponent::findUnoccupiedSpawnPoint()
{
	AttributePtr<Attribute_PlayerSpawnPoint> ptr_found_spawnPoint;
	std::vector<AttributePtr<Attribute_PlayerSpawnPoint>> unoccupiedSpawnPoints;
	
	// Special cases: *no player spawn point, return nullptr.
	int numSpawnPoints = itrPlayerSpawnPoint.storageSize();
	if(numSpawnPoints < 1)
	{
		DEBUGPRINT("GameComponent::findUnoccupiedSpawnPoint - No spawn point found.");
		return ptr_found_spawnPoint;
	}

	//
	// Find all unoccupied player spawn points.
	//

	AttributeIterator<Attribute_PlayerSpawnPoint> itrPlayerSpawnPoint = ATTRIBUTE_MANAGER->playerSpawnPoint.getIterator();
	while(itrPlayerSpawnPoint.hasNext())
	{
		// Fetch attributes
		ptr_found_spawnPoint = itrPlayerSpawnPoint.getNext();
		AttributePtr<Attribute_Position> position_spawnPoint = ptr_found_spawnPoint->ptr_position;
		
		// To prevent spawncamping, check if player spawnpoint is occupied
		bool isUnoccupied = true;
		AttributeIterator<Attribute_Player> itrPlayer = ATTRIBUTE_MANAGER->player.getIterator();
		while(itrPlayer.hasNext())
		{
			AttributePtr<Attribute_Player> player		= itrPlayer.getNext();
			AttributePtr<Attribute_Health> ptr_health	= player->ptr_health;

			// If player is detectedAsDead
			if(ptr_health->health > 0)
			{
				AttributePtr<Attribute_Render>		render	= player->ptr_render;
				AttributePtr<Attribute_Spatial>		spatial	= render->ptr_spatial;
				AttributePtr<Attribute_Position>	position_player	= spatial->ptr_position;

				// calculate distance to player spawn point
				float distanceToSpawnPoint =  position_player->position.distanceTo(position_spawnPoint->position);

				// if a player is within player spawn point radius
				if(distanceToSpawnPoint < ptr_found_spawnPoint->spawnArea)
				{
					// spawn point is occupied, abort further testing
					isUnoccupied = false;
					break;
				}
			}
		}
		// TRUE: Player spawn point is unoccupied, add to vector
		if(isUnoccupied)
		{
			unoccupiedSpawnPoints.push_back(ptr_found_spawnPoint); // this vector will be iterated below.
		}
	}


	// Iterate through all unoccupied player spawn points 
	// (found in the above loop) to find the player spawn 
	// point that least recently spawned a player.

	int nrOfUnoccupiedSpawnPoints = unoccupiedSpawnPoints.size();
	
	// Find least recently used player spawn point
	int longestTimeSinceLastSpawnIndex = -1;
	float longestTimeSinceLastSpawn = 0.0f;
	for(int i=0; i<nrOfUnoccupiedSpawnPoints; i++)
	{
		ptr_found_spawnPoint = unoccupiedSpawnPoints.at(i);
  		if(ptr_found_spawnPoint->secondsSinceLastSpawn >= longestTimeSinceLastSpawn)
		{
			longestTimeSinceLastSpawn = ptr_found_spawnPoint->secondsSinceLastSpawn;
 			longestTimeSinceLastSpawnIndex = i;
		}
	}
	if(longestTimeSinceLastSpawnIndex > -1)
	{
		ptr_found_spawnPoint = unoccupiedSpawnPoints.at(longestTimeSinceLastSpawnIndex);
	}

	// If all player spawn points are occupied, pick one at random.
	if(nrOfUnoccupiedSpawnPoints < 1)
	{
		int randomSpawnPointId = Math::randomInt(numSpawnPoints);
		ptr_found_spawnPoint = itrPlayerSpawnPoint.at(randomSpawnPointId);
	}

	// Reset player spawn point timer.
	if(ptr_found_spawnPoint.isNotEmpty())
		ptr_found_spawnPoint->secondsSinceLastSpawn = 0.0f;

	return ptr_found_spawnPoint;
}

void GameComponent::event_StartDeathmatch( Event_StartDeathmatch* e )
{
	//while(itrPickupablesSpawnPoint.hasNext())
	//{
	//	itrPickupablesSpawnPoint.getNext();
	//	SEND_EVENT(&Event_RemoveEntity(itrPickupablesSpawnPoint.ownerId()));
	//}

	/*
	while(itrPickupable.hasNext())
	{
		itrPickupable.getNext();
		SEND_EVENT(&Event_RemoveEntity(itrPickupable.ownerId()));
	}
	*/


	//
	// Create level entities
	//

	for(unsigned int i = 0; i < levelEvents_.size(); i++)
	{
		SEND_EVENT(levelEvents_.at(i));
	}

	// Create new players
	for(int i=0; i<e->num_players; i++)
	{
		SEND_EVENT(&Event_CreateEntity(PLAYER));
	}

	// Get window resolution so we can tell renderer to recalculate and resize split screens
	Event_GetWindowResolution event_getWindowResolution;
	SEND_EVENT(&event_getWindowResolution);
	int width = event_getWindowResolution.width;
	int height = event_getWindowResolution.height;
	SEND_EVENT(&Event_WindowResize(width,height));

	//Continue
	for(int i=0;i<10;i++)
	{
		SEND_EVENT(&Event_CreatePickupablesSpawnPoint(Float3(2.0f, 5.0f, i-6.0f), XKILL_Enums::PickupableType::MEDKIT));
	}

	//SEND_EVENT(&Event_CreatePickupable(Float3(2.0f, 2.0f, 0.0f), Attribute_Pickupable::MEDKIT, 5));

	// Set state to deathmatch
	GET_STATE() =  STATE_DEATHMATCH;
}

void GameComponent::event_TransferEventsToGame(Event_TransferEventsToGame* e)
{
	levelEvents_ = e->events;
}

void GameComponent::event_PlayerDeath(Event_PlayerDeath* e)
{
	AttributePtr<Attribute_Player> ptr_player = itrPlayer.at(e->playerIndex);
	AttributePtr<Attribute_Physics> ptr_physics = ptr_player->ptr_input->ptr_physics;
	AttributePtr<Attribute_Health> ptr_health = ptr_player->ptr_health;
	ptr_health->health = 0;

	ptr_physics->angularVelocity = Float3(0.0f, 0.0f, 0.0f);
	ptr_physics->linearVelocity = Float3(0.0f, 0.0f, 0.0f);
	ptr_physics->gravity = Float3(0.0f, -1.0f, 0.0f);
	ptr_physics->collisionFilterMask = ptr_physics->WORLD;
	ptr_physics->collisionResponse = true;
	ptr_physics->meshID = ptr_player->meshIDWhenDead;
	ptr_physics->reloadDataIntoBulletPhysics = true;

	ptr_player->currentRespawnDelay = ptr_player->respawnDelay;
	ptr_player->detectedAsDead = true;
}

bool GameComponent::switchAmmunition(AttributePtr<Attribute_WeaponStats> weaponStats)
{
	bool switchedAmmunition = false;
	FiringMode* firingMode = &weaponStats->firingMode[weaponStats->currentFiringModeType];

	for(int i = 0; i < XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES; i++)
	{
		weaponStats->currentAmmunitionType = static_cast<XKILL_Enums::AmmunitionType>((weaponStats->currentAmmunitionType + 1) % XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES);

		if(weaponStats->currentAmmunitionType == XKILL_Enums::AmmunitionType::BULLET && firingMode->canShootBullet || 
			weaponStats->currentAmmunitionType == XKILL_Enums::AmmunitionType::SCATTER && firingMode->canShootScatter || 
			weaponStats->currentAmmunitionType == XKILL_Enums::AmmunitionType::EXPLOSIVE && firingMode->canShootExplosive)
		{
			switchedAmmunition = true;
			break;
		}
	}

	return switchedAmmunition;
}

bool GameComponent::switchFiringMode(AttributePtr<Attribute_WeaponStats> ptr_weaponStats)
{
	bool switchedFiringMode = false;

	FiringMode* firingMode;

	for(int i = 0; i < XKILL_Enums::FiringModeType::NROFFIRINGMODETYPES; i++)
	{
		ptr_weaponStats->currentFiringModeType = static_cast<XKILL_Enums::FiringModeType>((ptr_weaponStats->currentFiringModeType + 1) % XKILL_Enums::FiringModeType::NROFFIRINGMODETYPES);

		firingMode = &ptr_weaponStats->firingMode[ptr_weaponStats->currentFiringModeType];

		if((ptr_weaponStats->currentAmmunitionType == XKILL_Enums::AmmunitionType::BULLET && firingMode->canShootBullet) ||
			(ptr_weaponStats->currentAmmunitionType == XKILL_Enums::AmmunitionType::SCATTER && firingMode->canShootScatter) ||
			(ptr_weaponStats->currentAmmunitionType == XKILL_Enums::AmmunitionType::EXPLOSIVE && firingMode->canShootExplosive))
		{
			switchedFiringMode = true;
			break;
		}
		else if(switchAmmunition(ptr_weaponStats))
		{
			switchedFiringMode = true;
			break;
		}
	}

	return switchedFiringMode;
}

void GameComponent::shootProjectile(AttributePtr<Attribute_Position> ptr_position, AttributePtr<Attribute_Camera> ptr_camera, AttributePtr<Attribute_WeaponStats> ptr_weaponStats)
{
	
	Ammunition* ammo = &ptr_weaponStats->ammunition[ptr_weaponStats->currentAmmunitionType];
	FiringMode* firingMode = &ptr_weaponStats->firingMode[ptr_weaponStats->currentFiringModeType];

	// Position
	Float3 pos = ptr_position->position;

	// extract camera orientation to determine velocity
	DirectX::XMFLOAT3 lookAtXMFloat3((float*)&ptr_camera->mat_view.getLookAt());

	DirectX::XMVECTOR lookAt = DirectX::XMLoadFloat3(&lookAtXMFloat3);
	lookAt = DirectX::XMVector3Normalize(lookAt);

	// Rotation
	ptr_camera->mat_view.getRotationOnly();
	//DirectX::XMMATRIX rotationMatrix((float*)&camera->mat_view);
	DirectX::XMMATRIX rotationMatrix(
		ptr_camera->mat_view._11,	ptr_camera->mat_view._21,	ptr_camera->mat_view._31,	0.0f,
		ptr_camera->mat_view._12,	ptr_camera->mat_view._22,	ptr_camera->mat_view._32,	0.0f, 
		ptr_camera->mat_view._13,	ptr_camera->mat_view._23,	ptr_camera->mat_view._33,	0.0f,
		0.0f,					0.0f,					0.0f,					1.0f);

	DirectX::XMVECTOR orientationQuaternion = DirectX::XMQuaternionRotationMatrix(rotationMatrix);
	float orientationQuaternionX = DirectX::XMVectorGetX(orientationQuaternion);
	float orientationQuaternionY = DirectX::XMVectorGetY(orientationQuaternion);
	float orientationQuaternionZ = DirectX::XMVectorGetZ(orientationQuaternion);
	float orientationQuaternionW = DirectX::XMVectorGetW(orientationQuaternion);

	Float4 rotation = Float4(orientationQuaternionX, orientationQuaternionY, orientationQuaternionZ, orientationQuaternionW);

	DirectX::XMVECTOR newLookAt;
	float randomLO;
	float randomHI;

	// Send "Event_CreateProjectile" for each projectile in a shot. Scatter has more than one projectile per shot.
	for(unsigned int j = 0; j < ammo->nrOfProjectiles; j++)
	{
		Float3 scatterPos = pos;

		lookAtXMFloat3.x = DirectX::XMVectorGetX(lookAt);
		lookAtXMFloat3.y = DirectX::XMVectorGetY(lookAt);
		lookAtXMFloat3.z = DirectX::XMVectorGetZ(lookAt);

		// randomize spread cone values (direction of velocity)
		if(ammo->spread != 0.0f)
		{
			randomLO = -ammo->spread*0.5f;
			randomHI = ammo->spread*0.5f;
			lookAtXMFloat3.x += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
			lookAtXMFloat3.y += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
			lookAtXMFloat3.z += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
		}

		newLookAt = DirectX::XMLoadFloat3(&lookAtXMFloat3);
		newLookAt = DirectX::XMVector3Normalize(newLookAt);
		lookAtXMFloat3.x = DirectX::XMVectorGetX(newLookAt);
		lookAtXMFloat3.y = DirectX::XMVectorGetY(newLookAt);
		lookAtXMFloat3.z = DirectX::XMVectorGetZ(newLookAt);

		Float3 velocity(lookAtXMFloat3.x, lookAtXMFloat3.y, lookAtXMFloat3.z);
		velocity = velocity * ammo->speed;

		//Randomize velocity for each consecutive projectile
		if(ammo->velocityVariation != 0.0f)
		{
			randomLO = 1 - ammo->velocityVariation*0.5f;
			randomHI = 1 + ammo->velocityVariation*0.5f;
			float randomVelocityDifference = randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
			velocity.x *= randomVelocityDifference;
 			velocity.y *= randomVelocityDifference;
			velocity.z *= randomVelocityDifference;
		}

		// add displacement on position (this should be based on the collision shape of the player model)
		float d = 0.5f;
		scatterPos.x += lookAtXMFloat3.x*d;
		scatterPos.y += lookAtXMFloat3.y*d;
		scatterPos.z += lookAtXMFloat3.z*d;

		// randomize displacement of each projectile preventing them from spawning at the same position
		if(ammo->spawnVariation != 0.0f)
		{
			randomLO = -ammo->spawnVariation *0.5f;
			randomHI = ammo->spawnVariation *0.5f;
			scatterPos.x += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
			scatterPos.y += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
			scatterPos.z += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
		}

		SEND_EVENT(&Event_CreateProjectile(scatterPos, velocity, rotation, itrPlayer.ownerId(), ammo->type, firingMode->type, ammo->damage));
	}
}
