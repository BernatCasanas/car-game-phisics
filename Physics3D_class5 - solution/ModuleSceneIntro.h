#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "PhysBody3D.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	void createRectangle(vec3 pos, vec3 size, char* color, PhysBody3D::Sensor_Type type);
	void createRectangleWithConstraint(vec3 pos, vec3 size, vec3 pos2, vec3 size2, char* color, PhysBody3D::Sensor_Type type, PhysBody3D::Sensor_Type type2);

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:

	p2List<Cube*> cube_list;

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	p2List<PhysBody3D*> sensors;
};
