#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;
	base_map = App->textures->Load("Assets/base.png");
	guides = App->textures->Load("Assets/guides.png");
	right_flipper = App->textures->Load("Assets/flipper.png");
	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	score = App->fonts->Load("Assets/Fonts/font_score2.png", "0123456789", 1);
	DrawColliders();

	
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(base_map);
	App->fonts->Unload(score);
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(base_map, 0, 0, NULL, 1.0f);
	App->renderer->Blit(guides, 0, 0, NULL, 1.0f);

	//Blit score
	App->fonts->BlitText(385, 190, score, "000", 0.5f);


	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25, b2_dynamicBody));
		circles.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		//Activate the motor ESSENTIAL STEP
		p2List_item<b2RevoluteJoint*>* item = App->physics->joints.getFirst()->next;
		
		item->data->SetMaxMotorTorque(100.0f);
		item->data->SetMotorSpeed(50.0f);
		
		item = item->next; 

		item->data->SetMaxMotorTorque(100.0f);
		item->data->SetMotorSpeed(50.0f);

	}
	else if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE){
		p2List_item<b2RevoluteJoint*>* item = App->physics->joints.getFirst()->next;
		
		item->data->SetMaxMotorTorque(100.0f);
		item->data->SetMotorSpeed(-50.0f);

		item = item->next;

		item->data->SetMaxMotorTorque(100.0f);
		item->data->SetMotorSpeed(-50.0f);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		//Activate the motor ESSENTIAL STEP
		p2List_item<b2RevoluteJoint*>* item = App->physics->joints.getFirst();

		item->data->SetMaxMotorTorque(100.0f);
		item->data->SetMotorSpeed(-50.0f);

	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE) {
		p2List_item<b2RevoluteJoint*>* item = App->physics->joints.getFirst();

		item->data->SetMaxMotorTorque(100.0f);
		item->data->SetMotorSpeed(50.0f);
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	App->audio->PlayFx(bonus_fx);
}

void ModuleSceneIntro::DrawColliders()
{

	int perimeter[78] = {

		110,	500,
		16,		439,
		16,		355,
		51,		315,
		51,		294,
		23,		255,
		15,		240,
		9,		224,
		5,		207,
		3,		190,
		0,		158,
		5,		127,
		14,		100,
		28,		74,
		46,		49,
		69,		27,
		93,		12,
		106,	8,
		118,	6,
		130,	4,
		142,	3,
		154,	3,
		167,	3,
		185,	4,
		213,	11,
		247,	29,
		270,	47,
		286,	69,
		299,	96,
		308,	120,
		308,	193,
		300,	223,
		291,	246,
		280,	260,
		288,	272,
		266,	300,
		289,	354,
		289,	438,
		194,	500

	};
	perimeter_ = App->physics->CreateChain(0, 0, perimeter, 78);

	int water_slide[40] = {

		34,		192,
		45,		217,
		68,		252,
		64,		256,
		39,		220,
		29,		193,
		24,		164,
		26,		133,
		33,		105,
		45,		81,
		66,		57,
		84,		40,
		99,		32,
		99,		52,
		90,		57,
		82,		143,
		83,		151,
		96,		163,
		43,		132,
		29,		116
	};

	water_slide_ = App->physics->CreateChain(0, 0, water_slide, 40);

	int mr_crabs[18] = {

		137,	116,
		112,	147,
		114,	156,
		121,	168,
		181,	159,
		186,	152,
		186,	142,
		184,	135,
		145,	114
	};

	mr_crabs_ = App->physics->CreateChain(0, 0, mr_crabs, 18);

	int top_right[68] = {

		280,	118,
		276,	119,
		266,	130,
		268,	166,
		265,	191,
		259,	209,
		261,	219,
		280,	190,
		284,	162,
		283,	132,
		274,	83,
		256,	58,
		235,	41,
		220,	33,
		201,	27,
		178,	23,
		170,	29,
		169,	52,
		174,	93,
		184,	101,
		204,	112,
		209,	124,
		210,	140,
		198,	177,
		199,	190,
		206,	194,
		213,	194,
		234,	162,
		235,	148,
		235,	134,
		232,	122,
		237,	113,
		258,	97,
		277,	97
	};

	top_right_ = App->physics->CreateChain(0, 0, top_right, 68);

	int left_triangle[6] = {

		61,		365,
		61,		404,
		92,		419
	};

	left_triangle_ = App->physics->CreateChain(0, 0, left_triangle, 6);


	int left_triangle_bouncer[4] = {

		61,		365,
		92,		419
	};

	left_triangle_bouncer_ = App->physics->CreateChain(0, 0, left_triangle_bouncer, 4);

	int right_triangle[6] = {

		217,	419,
		247,	403,
		247,	365
	};

	right_triangle_ = App->physics->CreateChain(0, 0, right_triangle, 6);

	int right_triangle_bouncer[4] = {

		217,	419,
		247,	365
	};

	right_triangle_bouncer_ = App->physics->CreateChain(0, 0, right_triangle_bouncer, 4);

	int top_left_pill[14] = {

		121,	28,
		115,	32,
		115,	51,
		120,	56,
		126,	51,
		126,	32,
		121,	28
	};

	top_left_pill_ = App->physics->CreateChain(0, 0, top_left_pill, 14);

	int top_right_pill[14] = {

		147,	24,
		142,	29,
		142,	48,
		147,	53,
		152,	48,
		152,	29,
		147,	24
	};

	top_right_pill_ = App->physics->CreateChain(0, 0, top_right_pill, 14);

	int bottom_water_slide[18] = {

		74,		196,
		100,	225,
		98,		228,
		72,		198,
		72,		194,
		45,		173,
		36,		168,
		13,		143,
		6,		120
	};

	bottom_water_slide_ = App->physics->CreateChain(0, 0, bottom_water_slide, 18);

	int down_left[12] = {

		119,	467,
		38,		417,
		38,		366,
		40,		366,
		40,		410,
		97,		447
	};

	down_left_ = App->physics->CreateChain(0, 0, down_left, 12);

	int down_right[12] = {

		180,	475,
		271,	415,
		271,	366,
		268,	366,
		268,	408,
		212,	445
	};

	down_right_ = App->physics->CreateChain(0, 0, down_right, 12);

	int top_left_wall[4] = {

		97,		34,
		91,		13
	};

	top_left_wall_ = App->physics->CreateChain(0, 0, top_left_wall, 4);

	int top_right_wall[4] = {

		179,	24,
		181,	4
	};

	top_right_wall_ = App->physics->CreateChain(0, 0, top_right_wall, 4);

	int right_limit[4] = {

		303,	116,
		303,	500
	};

	right_limit_ = App->physics->CreateChain(0, 0, right_limit, 4);

	int ball_base[4] = {

		288,	436,
		311,	436
	};

	ball_base_ = App->physics->CreateChain(0, 0, ball_base, 4);

	//------CREATING KICKERS
	int kicker_left[14] = {
		1, 6,
		5, 0,
		56, 4,
		59, 7,
		55, 10,
		5, 12,
		1, 6
	};
	
	int kicker_right[14] = {
		1, 6,
		5, 4,
		56, 0,
		59, 7,
		55, 12,
		5, 10,
		1, 6
	};

	int kicker_topright[14] = {
		0, 6,
		3, 3,
		45, 1,
		48, 6,
		45, 11,
		3, 8,
		0, 6
	};

	create_kickers(kicker_left, kicker_right, kicker_topright);

}

void ModuleSceneIntro::create_kickers(int* kicker1, int* kicker2, int* kicker3)
{
	l_kicker = App->physics->CreateKickers(98, 443, kicker1,14); //dyn
	r_kicker = App->physics->CreateKickers(152, 443, kicker2, 14); //dyn
	tr_kicker = App->physics->CreateKickers(236, 265, kicker3, 14); //dyn

	pivot_body1 = App->physics->CreatePivots(105,448,9);
	pivot_body2 = App->physics->CreatePivots(205, 448, 9);
	pivot_body3 = App->physics->CreatePivots(278, 270, 9);

	
	App->physics->createJoint(pivot_body1->body,l_kicker->body, -0.16f, 0.25f, true);
	App->physics->createJoint(pivot_body2->body, r_kicker->body, -0.25f, 0.16f);
	App->physics->createJoint(pivot_body3->body, tr_kicker->body, -0.25f, 0.0f);

	//l_kicker->GetPosition(position.x, position.y);
	App->renderer->Blit(right_flipper, 0, 0, NULL, 1.0f);

}
