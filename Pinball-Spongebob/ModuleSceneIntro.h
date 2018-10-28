#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"
#include "ModuleTextures.h"

class PhysBody;
enum layers {
	bottom_layer,
	rail_layer,
	green_tube_exit,
	green_tube_entrance
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void sensorAction(PhysBody * sensor);
	void DrawColliders();
	void create_kickers(int* kicker1, int* kicker2, int* kicker3);
	void create_sensors();

	void resetLayers();

public:
	p2List<PhysBody*> sensors;

	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	p2List<PhysBody*> base_layer;
	p2List<PhysBody*> rail;
	p2List<PhysBody*> lgreen_tube_exit;
	p2List<PhysBody*> lgreen_tube_entrance;
	int activeLayers[4];



	int kickers[3];
	PhysBody* perimeter_init_;
	PhysBody* perimeter_final_;
	PhysBody* door_;
	PhysBody* water_slide_;
	PhysBody* mr_crabs_;
	PhysBody* top_right_;
	PhysBody* left_triangle_;
	PhysBody* right_triangle_;
	PhysBody* top_left_pill_;
	PhysBody* top_right_pill_;
	PhysBody* kicker_;
	PhysBody* left_triangle_bouncer_;
	PhysBody* right_triangle_bouncer_;
	PhysBody* bottom_water_slide_;
	PhysBody* down_left_;
	PhysBody* down_right_;
	PhysBody* top_left_wall_;
	PhysBody* top_right_wall_;
	PhysBody* right_limit_;
	PhysBody* left_limit_;
	PhysBody* pivot_body1;
	PhysBody* pivot_body2;
	PhysBody* pivot_body3;
	PhysBody* l_kicker;
	PhysBody* r_kicker;
	PhysBody* tr_kicker;

	PhysBody* m_box;
	PhysBody* s_box;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	uint bonus_fx;

	SDL_Texture* base_map;
	SDL_Texture* guides;
	SDL_Texture* overlay_down;
	SDL_Texture* bounce_hamburger;
	SDL_Texture* overlay;
	SDL_Texture* right_flipper;
	SDL_Texture* left_flipper;
	SDL_Texture* top_right_flipper;
	SDL_Texture* ball;
	SDL_Texture* slide;
	SDL_Texture* ball_text;
	SDL_Texture* sound;
	SDL_Texture* spring_tex;

	iPoint position;
	bool tube_teleport;
	bool crown_teleport;
	bool blit_under;
	char player_lives[10];
	int score = -1;
	int lives_font = -1; 
	unsigned int lastTime = 0, currentTime;
};
