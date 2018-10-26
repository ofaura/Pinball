#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

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
	void create_kickers(int* kicker1, int* kicker2, int* kicker3);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	int kickers[3];
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
	PhysBody* top_left_wall_;
	PhysBody* top_right_wall_;

	PhysBody* pivot_body1;
	PhysBody* pivot_body2;
	PhysBody* pivot_body3;
	PhysBody* l_kicker;
	PhysBody* r_kicker;
	PhysBody* tr_kicker;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	uint bonus_fx;

	int score = -1;
};
