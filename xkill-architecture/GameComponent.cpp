#include "GameComponent.h"
#include "AttributeManager.h"

#include <iostream>

GameComponent::GameComponent(void)
{
}

GameComponent::~GameComponent(void)
{
}

bool GameComponent::init()
{
	return true;
}

void GameComponent::onEvent(Event* e)
{
}

void GameComponent::onUpdate(float delta)
{

	std::vector<PlayerAttribute>* attributesOfPlayer = AttributeManager::getInstance()->playerAttributes_.getAllAttributes();
	std::vector<int>* attributesOfPlayerOwners = AttributeManager::getInstance()->playerAttributes_.getAllOwners();


	for(int i=0; i<static_cast<int>(attributesOfPlayer->size()); i++)
	{
		if(attributesOfPlayerOwners->at(i)!=0)
		{
			PlayerAttribute* playerAttribute = &attributesOfPlayer->at(i);
			RenderAttribute* renderAttribute = ATTRIBUTE_CAST(RenderAttribute, renderAttribute, playerAttribute); //Extract render attribute from player attribute
			SpatialAttribute* spatialAttribute = ATTRIBUTE_CAST(SpatialAttribute, spatialAttribute, renderAttribute);
			PositionAttribute* positionAttribute = ATTRIBUTE_CAST(PositionAttribute, positionAttribute, spatialAttribute);
			
			if(playerAttribute->id == 0) //Handle player 1 (test)
			{
				//positionAttribute->position[0] = positionAttribute->position[0]+1.0f;
			}
			else if(playerAttribute->id == 1) //Handle player 2 (test)
			{
				//positionAttribute->position[2] = positionAttribute->position[2]+1.0f; 
			}

			std::cout 
				<< playerAttribute->id						<< "\t"
				<< attributesOfPlayerOwners->at(i)			<< "\t"
				<< playerAttribute->name					<< "\t"
				<< positionAttribute->position[0]			<< "\t"
				<< positionAttribute->position[1]			<< "\t"
				<< positionAttribute->position[2]			<< "\t"
				<< std::endl;
		}
	}
}