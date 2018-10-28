#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"
#include "ModuleTextures.h"
#include "Animation.h"

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
	void Blit_Bottom_Lights();
	void Blit_Bouncer();
	void Blit_Middle_Lights();
	void Blit_Top_Lights();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void sensorAction(PhysBody * sensor);
	void DrawColliders();
	void create_kickers(int* kicker1, int* kicker2, int* kicker3);
	void create_sensors();
	void SetScores();
	void UpdateScores();

	void resetLayers();

	bool Sort(bool arr[], int n);

public:
	p2List<PhysBody*> sensors;
	p2List<PhysBody*> score_list;
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
	uint bouncer_fx;
	uint kicker_fx;
	uint flipper_down_fx;
	uint flipper_up_fx;
	uint hamburger_fx;
	uint green_tube_fx;

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
	SDL_Texture* light_tex;
	SDL_Texture* triangles;
	SDL_Texture* anchors;
	SDL_Texture* anchors_combo;

	iPoint position;
	bool tube_teleport;
	bool crown_teleport;
	bool blit_under;
	char player_lives[10];
	char player_score[10];
	int score = -1;
	int lives_font = -1; 
	unsigned int lastTime = 0, currentTime;
	bool light_bottom[4];
	bool light_middle[5];
	bool light_top[3];
	bool bouncer[2];
	int anchor_c;

	bool down_played = false;
	bool down_played2 = false;
	bool kicker_played = false;

	bool score_hamburgers = false;
	bool score_hole = false;
	bool score_green_tube = false;
	bool score_water_slide = false; 
	
	Animation anchor_anim;
	Animation anchor_combo_anim;
	
	Animation bouncer1;
	Animation bouncer2;

	Animation bottom_light1;
	Animation bottom_light2;
	Animation bottom_light3;
	Animation bottom_light4;

	Animation middle_light1;
	Animation middle_light2;
	Animation middle_light3;
	Animation middle_light4;
	Animation middle_light5;

	Animation top_light1;
	Animation top_light2;
	Animation top_light3;
	
};
