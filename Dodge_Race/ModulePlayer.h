#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 3000.0f
#define TURN_DEGREES 10.0f * DEGTORAD
#define BRAKE_POWER 100.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	void SetPos(float x, float y, float z);
	void Restart(bool check_checkpoint=false);

public:

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
	int lives;
	int max_lives;
	bool checkpoint;
};