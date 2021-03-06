#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModuleSceneIntro.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
	lives = 3;
	max_lives = 3;
	hits = 0;
	checkpoint = false;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(3,0.5, 4);
	car.chassis_offset.Set(0, 1, 0);
	car.chassis_up_size.Set(3, 0.25, 4);
	car.chassis_up_offset.Set(0, 3.25, 0);
	car.back_left_size.Set(0.5, 2, 1);
	car.back_left_offset.Set(1.25, 2.25, -1.5);
	car.back_right_size.Set(0.5, 2, 1);
	car.back_right_offset.Set(-1.25, 2.25, -1.5);
	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 3;
	car.wheels = new Wheel[3];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(0, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// BACK-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = false;
	car.wheels[1].drive = false;
	car.wheels[1].brake = true;
	car.wheels[1].steering = false;

	// BACK-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;
	
	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 0, 10);

	for (p2List_item<PhysBody3D*>* item = App->scene_intro->sensors.getFirst() ; item != App->scene_intro->sensors.getLast(); item = item->next)
	{
		item->data->sensor_enabled = false;
	}
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION;
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		brake = BRAKE_POWER;
	}
	
	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		Restart(true);
	}
	vehicle->Render(lives);
	int level;
	if (max_lives == 3)level = 1;
	else if (max_lives == 2)level = 2;
	else if (max_lives == 1)level = 3;
	char title[80];
	sprintf_s(title, "Score:          Level  %d          Hits  %d", level, hits);
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

void ModulePlayer::SetPos(float x, float y, float z) {
	vehicle->SetPos(x, y, z);
}

void ModulePlayer::Restart(bool check_checkpoint)
{
	App->player->lives = max_lives;
	App->player->acceleration = 0;
	App->player->brake = 0;
	App->player->turn = 0;
	if (check_checkpoint == true && checkpoint == true && App->scene_intro->win==false) {
		App->player->SetPos(0, 0, 510);
	}
	else {
		App->player->SetPos(0, 0, 10);
	}
	App->player->vehicle->ResetVelocityAndRotation();
}

