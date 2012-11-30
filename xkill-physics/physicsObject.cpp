#include "physicsObject.h"

#include <xkill-utilities/AttributeType.h>

PhysicsObject::PhysicsObject()
{
	rigidBody_ = nullptr;
}

PhysicsObject::~PhysicsObject()
{
	delete rigidBody_->getMotionState();
	delete rigidBody_->getCollisionShape();
	delete rigidBody_;
	rigidBody_ = nullptr;
}

void PhysicsObject::Init(PhysicsAttribute* physicsAttribute, btDiscreteDynamicsWorld* dynamicsWorld)
{
	/*rigidBody_ = new btRigidBody(physicsAttribute->mass,
								 new btDefaultMotionState(btVector3(physicsAttribute->sa->pa->position.x,
															        physicsAttribute->sa->pa->position.y,
																	physicsAttribute->sa->pa->position.z))
								 new btSphereShape(1),
								 btVector3(0,0,0));*/
		
	preStep(physicsAttribute);
	dynamicsWorld->addRigidBody(rigidBody_);
}

void PhysicsObject::Clean(btDiscreteDynamicsWorld* dynamicsWorld)
{
	dynamicsWorld->removeRigidBody(rigidBody_);
}



void PhysicsObject::preStep(PhysicsAttribute* physicsAttribute)
{
	
}

void PhysicsObject::postStep(PhysicsAttribute* physicsAttribute)
{
	
}

void PhysicsObject::input(InputAttribute* inputAttribute)
{
	
	btVector3 force(inputAttribute->position[0],0,inputAttribute->position[1]);
	btVector3 torque(0,inputAttribute->rotation[0],0);
	rigidBody_->applyForce(force,btVector3(0,0,0));
	rigidBody_->applyTorque(torque);
}