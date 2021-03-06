#ifndef XKILL_PHYSICS_PHYSICSOBJECT
#define XKILL_PHYSICS_PHYSICSOBJECT

#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <xkill-utilities\XKILL_Enums.h>

#include <string>

class btDiscreteDynamicsWorld;
class debugDrawDispatcher;

class PhysicsObject
	: public btRigidBody
{
private:
	virtual btVector3 subClassCalculateLocalInertiaHook(btScalar mass);		//! May be overridden by subclasses. Is called from init.
	virtual bool subClassSpecificInitHook();								//! May be overridden by subclasses. Is called from the end of init.
protected:
		unsigned int attributeIndex_;										//!< Specifies which PhysicsObject is synchronized with which physics attribute.
		short collisionFilterGroup_;										//!< The filter group the object belongs to, ex: XKILL_Enums::PhysicsAttributeType::WORLD.
		static btDiscreteDynamicsWorld*	dynamicsWorld_;						//!< Gives all physics objects access to the dynamics world. Deallocation is handled in the destructor of PhysicsComponent.
		static debugDrawDispatcher*	debugDrawer_;							//!< Gives all physics objects access to the debug drawer. Deallocation is handled in the destructor of PhysicsComponent.

		btVector3 localInertiaBasedOnCollisionShapeAndMass(btScalar mass);	//!< Called from "subClassCalculateLocalInertiaHook" by some subclasses.
		btVector3 zeroLocalInertia();										//!< Called from "subClassCalculateLocalInertiaHook" by some subclasses.
		void removePhysicsAttributeCorrespondingToThisPhysicsObject();		//!< Called from "handleOutOfBounds" by some subclasses.
		virtual btCollisionShape* subClassSpecificCollisionShape();			//!< May be overridden by subclasses. Is called from the end of init.
		virtual void hover(float delta, float hoverHeight);					//!< Hover above ground by sending a ray straight down from the position of the Physics Object.
		
public:
	PhysicsObject();
	virtual ~PhysicsObject();
	//! Init PhysicsObject and call subClassSpecificInitHook()
	/*!
	\param attributeIndex The physics attribute index.
	*/
	bool init(unsigned int attributeIndex, short collisionFilterGroup);
	unsigned int getAttributeIndex() const;  //!< Returns the attribute index that the physicsobject maps to
	short getCollisionFilterGroup() const;  //!< Returns the filter group the object belongs to, ex: XKILL_Enums::PhysicsAttributeType::WORLD
	void writeNonSynchronizedPhysicsObjectDataToPhysicsAttribute();

	virtual void onUpdate(float delta);
	virtual void handleOutOfBounds(); //!< Standard out of bounds handling: move object to a new position

	static void setDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld){dynamicsWorld_=dynamicsWorld;}
	static void setDebugDrawer(debugDrawDispatcher* debugDrawer){debugDrawer_=debugDrawer;}
};

#endif