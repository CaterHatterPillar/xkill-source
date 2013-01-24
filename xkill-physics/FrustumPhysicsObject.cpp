#include "FrustumPhysicsObject.h"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
//#include <BulletCollision/CollisionShapes/btSphereShape.h>

//#include "collisionShapes.h"
#include <xkill-utilities/EventManager.h>
#include "CollisionShapes.h"
#include "physicsUtilities.h"
AttributeIterator<Attribute_Camera> itrCamera_3;
AttributeIterator<Attribute_Spatial> itrSpatial_3;
AttributeIterator<Attribute_Position> itrPosition_3;


FrustumPhysicsObject::FrustumPhysicsObject()
	: PhysicsObject()
{
	itrCamera_3 = ATTRIBUTE_MANAGER->camera.getIterator();
	itrSpatial_3 = ATTRIBUTE_MANAGER->spatial.getIterator();
	itrPosition_3 = ATTRIBUTE_MANAGER->position.getIterator();
}

FrustumPhysicsObject::~FrustumPhysicsObject()
{
}

void FrustumPhysicsObject::onUpdate(float delta)
{
	btMatrix3x3 view = convert(itrCamera_3.at(attributeIndex_)->mat_view);
	btVector3 pos = convert(itrPosition_3.at(itrSpatial_3.at(itrCamera_3.at(attributeIndex_)->ptr_spatial)->ptr_position)->position);
	btQuaternion q;
	view.getRotation(q);
	btTransform world = getWorldTransform();
	world.setRotation(q);
	world.setOrigin(pos);
	setWorldTransform(world);
	
	setCollisionShape(CollisionShapes::Instance()->getFrustrumShape(attributeIndex_));
}

bool FrustumPhysicsObject::frustumInit(unsigned int attributeIndex,unsigned int collisionFilterGroup)
{
	if(attributeIndex < 0)
	{
		return false;
	}
	attributeIndex_ = attributeIndex;
	collisionFilterGroup_ = collisionFilterGroup;
	Attribute_Camera* cameraAttribute = itrCamera_3.at(attributeIndex);
	btVector3 localInertia;
	localInertia.setZero();
	btCollisionShape* collisionShape = CollisionShapes::Instance()->getFrustrumShape(attributeIndex_);
	btScalar mass = static_cast<btScalar>(1);
	setMassProps(mass, localInertia);
	setCollisionShape(collisionShape);
	btTransform world;
	Attribute_Spatial* spatialAttribute = itrSpatial_3.at(itrCamera_3.at(attributeIndex_)->ptr_spatial);
 	Attribute_Position* positionAttribute = itrPosition_3.at(spatialAttribute->ptr_position);
 	world.setOrigin(convert(positionAttribute->position));
	world.setRotation(convert(spatialAttribute->rotation));
	setWorldTransform(world);
	setCollisionFlags(getCollisionFlags() | CF_NO_CONTACT_RESPONSE); 
	forceActivationState(DISABLE_DEACTIVATION);
	return true;
}