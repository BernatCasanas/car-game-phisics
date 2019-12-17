#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"

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

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	createRectangle((10, 3, 0), (1, 1, 1));
	createRectangle((0, 6, 4), (1, 1, 1));
	createRectangle((0, 7, -4), (1, 1, 1));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void ModuleSceneIntro::createRectangle(vec3 pos, vec3 size)
{
	Cube* object = new Cube(size.x, size.y, size.z);

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

