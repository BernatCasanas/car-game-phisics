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
	float z = 0, x = -7;
	for (z; z <= 500; z += 1.5) {
		if (par % 2 == 0) {
			color = "Red";
		}
		else color = "White";
		createRectangle({ 8, 0, z }, { 1, 2, 1 }, color);
		createRectangle({ -8, 0, z }, { 1, 2, 1 }, color);
		par++;
	}

	//Top Bottom Limits
	for (x; x < 8; x += 1.5) {
		if (par % 2 == 0) {
			color = "Red";
		}
		else color = "White";
		createRectangle({ x, 0, 0 }, { 1, 2, 1 }, color);
		par++;
	}
	x = -7;
	for (x; x < 8; x += 1.5) {
		if (par % 2 == 0) {
			color = "Red";
		}
		else color = "White";
		createRectangle({ x, 0, z }, { 1, 2, 1 }, color);
		par++;
	}

	//Obstacles Randomly setted
	for (float i = 6; i <= 500; i += 9) {
		r = rand() % 14 - 7;
		createRectangle({ r, 0, i }, { 1, 2, 1 }, color);
	}

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void ModuleSceneIntro::createRectangle(vec3 pos, vec3 size, char* color)
{
	Cube* object = new Cube(size.x, size.y, size.z);
	if (pos.y == 0) {
		pos.y += size.y * 0.5;
	}
	if(color =="White")
		object->color = White;
	else if (color == "Red")
		object->color = Red;
	object->SetPos(pos.x, pos.y, pos.z);
	object->SetRotation(1, vec3(0, 1, 0));
	cube_list.add(object);

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
}

