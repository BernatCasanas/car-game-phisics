#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render(int lives)
{
	Cylinder wheel;

	wheel.color = Green;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}
	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	if (lives == 3) {
		chassis.color = White;
	}
	else if (lives == 2) {
		chassis.color = Blue;
	}
	else if (lives == 1) {
		chassis.color = Red;
	}
	else if (lives == 0) {
		chassis.color = Black;
	}
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();


	Cube back_left(info.back_left_size.x, info.back_left_size.y, info.back_left_size.z);
	back_left.color = chassis.color;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&back_left.transform);
	btVector3 offset2(info.back_left_offset.x, info.back_left_offset.y, info.back_left_offset.z);
	offset2 = offset2.rotate(q.getAxis(), q.getAngle());

	back_left.transform.M[12] += offset2.getX();
	back_left.transform.M[13] += offset2.getY();
	back_left.transform.M[14] += offset2.getZ();

	Cube back_right(info.back_right_size.x, info.back_right_size.y, info.back_right_size.z);
	back_right.color = chassis.color;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&back_right.transform);
	btVector3 offset3(info.back_right_offset.x, info.back_right_offset.y, info.back_right_offset.z);
	offset3 = offset3.rotate(q.getAxis(), q.getAngle());

	back_right.transform.M[12] += offset3.getX();
	back_right.transform.M[13] += offset3.getY();
	back_right.transform.M[14] += offset3.getZ();

	Cube chassis_up(info.chassis_up_size.x, info.chassis_up_size.y, info.chassis_up_size.z);
	chassis_up.color = chassis.color;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis_up.transform);
	btVector3 offset4(info.chassis_up_offset.x, info.chassis_up_offset.y, info.chassis_up_offset.z);
	offset4 = offset4.rotate(q.getAxis(), q.getAngle());

	chassis_up.transform.M[12] += offset4.getX();
	chassis_up.transform.M[13] += offset4.getY();
	chassis_up.transform.M[14] += offset4.getZ();

	chassis.Render();
	back_left.Render();
	back_right.Render();
	chassis_up.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}


void PhysVehicle3D::ResetVelocityAndRotation() {
	vehicle->getRigidBody()->setAngularVelocity({ 0,0,0 });
	vehicle->getRigidBody()->setLinearVelocity({ 0,0,0 });
	btTransform t=vehicle->getRigidBody()->getWorldTransform();
	t.setRotation({ 0,0,0,1 });
	vehicle->getRigidBody()->setWorldTransform(t);
}