#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void DrawColliders();

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	PhysBody* perimeter_;
	PhysBody* water_slide_;
	PhysBody* mr_crabs_;
	PhysBody* top_right_;
	PhysBody* left_triangle_;
	PhysBody* right_triangle_;
	PhysBody*top_left_pill_;
	PhysBody* top_right_pill_;
	PhysBody* kicker_;
	PhysBody* left_triangle_bouncer_;
	PhysBody* right_triangle_bouncer_;
	PhysBody* bottom_water_slide_;
	PhysBody* down_left_;
	PhysBody* down_right_;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	uint bonus_fx;
};
