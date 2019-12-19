#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "time.h"
#include "Color.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	srand(time(NULL));
	float r;
	int par = 1;
	char* color;

	//Horizontal Limits
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	float z = 0, x = -11;
	for (z; z <= 1000; z += 1.5) {
		if (par % 2 == 0) {
			color = "Red";
		}
		else color = "White";
		if (z == 499.5) {
			createRectangleWithConstraint({ 12, 0, z }, { 1, 5, 1 }, { 6, 0, z }, { 11, 5, 1 }, color, PhysBody3D::Sensor_Type::WALL, PhysBody3D::Sensor_Type::CHECKPOINT);
			createRectangleWithConstraint({ -12, 0, z }, { 1, 5, 1 }, { -6, 0, z }, { 11, 5, 1 }, color, PhysBody3D::Sensor_Type::WALL, PhysBody3D::Sensor_Type::CHECKPOINT);
		}
		else {
			createRectangle({ 12, 0, z }, { 1, 5, 1 }, color, PhysBody3D::Sensor_Type::WALL);
			createRectangle({ -12, 0, z }, { 1, 5, 1 }, color, PhysBody3D::Sensor_Type::WALL);
		}
		par++;
	}

	//Top Bottom Limits
	for (x; x < 12; x += 1.5) {
		if (par % 2 == 0) {
			color = "Red";
		}
		else color = "White";
		createRectangle({ x, 0, 0 }, { 1, 5, 1 }, color, PhysBody3D::Sensor_Type::WALL);
		par++;
	}
	x = -7;
	for (x; x < 12; x += 1.5) {
		if (par % 2 == 0) {
			color = "Red";
		}
		else color = "White";
		createRectangle({ x, 0, z }, { 1, 5, 1 }, color, PhysBody3D::Sensor_Type::WALL);
		par++;
	}

	int random_color;

	//Obstacles Randomly setted
	for (float i = 18; i <= 1000; i += 10.5) {
		if (i < 480 || i>520) {
			r = rand() % 24 - 12;
			random_color = rand() % 5 + 1;
			switch (random_color)
			{
			case 1:
				color = "Green";
				break;
			case 2:
				color = "Blue";
				break;
			case 3:
				color = "Pink";
				break;
			case 4:
				color = "Yellow";
				break;
			case 5:
				color = "Cian";
				break;
			default:
				LOG("something went wrong", "%s");
				break;
			}
			createRectangle({ r, 0, i }, { 1, 3, 1 }, color, PhysBody3D::Sensor_Type::OBSTACLE);
		}
	}

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void ModuleSceneIntro::createRectangle(vec3 pos, vec3 size, char* color, PhysBody3D::Sensor_Type type)
{
	Cube* object = new Cube(size.x, size.y, size.z);
	if (pos.y == 0) {
		pos.y += size.y * 0.5;
	}
	if (color == "White")
		object->color = White;
	else if (color == "Red")
		object->color = Red;
	else if (color == "Cian")
		object->color = Cian;
	else if (color == "Green")
		object->color = Green;
	else if (color == "Pink")
		object->color = Pink;
	else if (color == "Blue")
		object->color = Blue;
	else if (color == "Yellow")
		object->color = Yellow;
	else if (color == "Black")
		object->color = Black;

	object->SetPos(pos.x, pos.y, pos.z);
	object->SetRotation(1, vec3(0, 1, 0));
	cube_list.add(object);
	PhysBody3D* pobject = App->physics->AddBody(*object, type, 0.0f);
	pobject->collision_listeners.add(this);
	pobject->SetAsSensor(false);
	sensors.add(pobject);
}

void ModuleSceneIntro::createRectangleWithConstraint(vec3 pos, vec3 size,vec3 pos2, vec3 size2, char* color, PhysBody3D::Sensor_Type type, PhysBody3D::Sensor_Type type2) {
	Cube* object = new Cube(size.x, size.y, size.z);
	if (pos.y == 0) {
		pos.y += size.y * 0.5;
	}
	if (color == "White")
		object->color = White;
	else if (color == "Red")
		object->color = Red;
	object->SetPos(pos.x, pos.y, pos.z);
	object->SetRotation(1, vec3(0, 1, 0));
	cube_list.add(object);
	PhysBody3D* pobject = App->physics->AddBody(*object, type, 0.0f);
	pobject->collision_listeners.add(this);
	pobject->SetAsSensor(false);
	sensors.add(pobject);
	Cube* object2 = new Cube(size2.x, size2.y, size2.z);
	if (pos2.y == 0) {
		pos2.y += size2.y * 0.5;
	}
	object2->color = Green;
	object2->SetPos(pos2.x, pos2.y, pos2.z);
	object2->SetRotation(1, vec3(0, 1, 0));
	cube_list.add(object2);
	PhysBody3D* pobject2 = App->physics->AddBody(*object2, type2, 1.0f);
	pobject2->collision_listeners.add(this);
	pobject2->SetAsSensor(false);
	sensors.add(pobject2);

	vec3 v = { 0,1,0 };
	if (pos.x > 0) {
		App->physics->AddConstraintHinge(*pobject, *pobject2, { (size.x / 2)-1,0,0 }, { -(size2.x / 2),0,0 }, v, v);
	}
	else {
		App->physics->AddConstraintHinge(*pobject, *pobject2, { -(size.x / 2)+1,0,0 }, { -(size2.x / 2),0,0 }, v, v);
	}
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();
	
	p2List_item<Cube*>* cube = cube_list.getFirst();

	for (cube; cube != nullptr;cube = cube->next) {
		cube->data->Render();
	}


	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1->type == PhysBody3D::Sensor_Type::OBSTACLE) {
		LOG("%s", "hola");
	}
}

