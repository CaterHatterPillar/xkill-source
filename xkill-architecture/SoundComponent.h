#pragma once

//#include <xkill-utilities/IComponent.h> //check
#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/EventManager.h>


/// Component responsible for playing Sound based on which Event is sent
/** 
If 3D positioning of sound is needed, creating an Entity 
with SoundAttribute and process each Attribute here is 
probably the best approach.

\ingroup components
*/

class SoundComponent : public IObserver
{
private:

public:
	SoundComponent()
	{
		
	}

	void onEvent(Event* e)
	{
		// Subscribes to all events, used for testing purpose
		EventManager::getInstance()->addObserverToAll(this);

		EventType type = e->getType();
		
		// This is just to demonstrate Enum can be converted to a int 
		int index = type;

		// TODO: Play sound based on index
	};

	void onUpdate(float delta)
	{
		// Update FMOD
	}
};