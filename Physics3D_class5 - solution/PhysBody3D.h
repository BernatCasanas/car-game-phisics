#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"

class btRigidBody;
class Module;

// =================================================
enum class SensorType {

};

struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);

protected:
	btRigidBody* body = nullptr;

public:
	p2List<Module*> collision_listeners;
protected:
	bool is_sensor;

};


struct PhysSensor3D : public PhysBody3D
{
public:
	PhysSensor3D(btRigidBody* body, SensorType type);
	~PhysSensor3D() {};
	void SetAsSensor(bool is_sensor);
	SensorType type;
	bool isEnabled;
};


#endif // __PhysBody3D_H__