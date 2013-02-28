#pragma once

#include "dllUtilities.h"

namespace XKILL_Enums
{
	enum DLL_U PickupableType
	{
		MEDKIT,
		AMMUNITION_BULLET,
		AMMUNITION_SCATTER,
		AMMUNITION_EXPLOSIVE,
		HACK_SPEEDHACK,
		HACK_JETHACK,
	
		NROFPICKUPABLETYPES
	};

	enum DLL_U ModifyPhysicsObjectData
	{
		GRAVITY,
		VELOCITY,
		VELOCITYPERCENTAGE,
		FLAG_STATIC,
		GIVE_IMPULSE,
		COLLISIONFILTERMASK,
		IF_TRUE_RECALCULATE_LOCAL_INERTIA_ELSE_SET_TO_ZERO
	};

	enum DLL_U FiringModeType
	{
		SINGLE,
		SEMI,
		AUTO,

		NROFFIRINGMODETYPES
	};
	enum DLL_U AmmunitionType
	{
		BULLET,
		SCATTER,
		EXPLOSIVE,

		NROFAMMUNITIONTYPES
	};

	enum DLL_U HackType
	{
		SPEEDHACK,
		JETHACK,

		NROFHACKTYPES
	};

	enum DLL_U PhysicsAttributeType
	{
		NOTHING = 0,
		WORLD = 1,
		PLAYER = 2,
		PROJECTILE = 4,
		EXPLOSIONSPHERE = 8,
		FRUSTUM = 16,
		PICKUPABLE = 32,
		RAY = 64,
		PROP = 128,
		EVERYTHING = -1
	};

	enum DLL_U ModelId
	{
		PLACEHOLDER					= 0,

		RIFLE						= 2,
		PICKUPABLE_HEALTHPACK		= 3,
		PICKUPABLE_AMMO_BULLET		= 4,
		PICKUPABLE_AMMO_SCATTER		= 5,
		PICKUPABLE_AMMO_EXPLOSIVE	= 6,
		PLAYERCONTROLLEDCHARACTER	= 7,
		PROJECTILE_EXPLOSIVE		= 8,
		PROJECTILE_SCATTER			= 9,
		PROJECTILE_BULLET			= 10,

		LASER						= 13,

		MODEL_LAST
	};
}