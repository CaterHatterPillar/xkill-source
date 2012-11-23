#pragma once

#include <xkill-utilities/AttributeType.h>
#include "AttributeType.h"

class AttributeManager
{
public:
	AttributeStorage<PositionAttribute> positionAttributes;
	AttributeStorage<SpatialAttribute> spatialAttributes;
	AttributeStorage<RenderAttribute>	renderAttributes;

	static AttributeManager* getInstance()
	{
		static AttributeManager instance;
		return &instance;
	}
};