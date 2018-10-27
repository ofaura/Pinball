#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"
#include "ModulePlayer.h"

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
	guides = App->textures->Load("Assets/Overlay_Middle.png");
	overlay_down = App->textures->Load("Assets/Overlay_Down.png");
	bounce_hamburger = App->textures->Load("Assets/flippers.png");
	overlay = App->textures->Load("Assets/overlay.png");
	left_flipper = App->textures->Load("Assets/flipper.png");
	right_flipper = App->textures->Load("Assets/flipper_right.png");
	top_right_flipper = App->textures->Load("Assets/flipper_topright.png");
	//circle = App->textures->Load("pinball/wheel.png");
	//box = App->textures->Load("pinball/crate.png");
	//rick = App->textures->Load("pinball/rick_head.png");
	ball = App->textures->Load("Assets/ball.png");
	slide = App->textures->Load("Assets/water_slide.png");
	ball_text = App->textures->Load("Assets/ball_text.png");
	sound = App->textures->Load("Assets/sound.png");

	score = App->fonts->Load("Assets/Fonts/font_score2.png", "0123456789", 1);
	lives_font = App->fonts->Load("Assets/Fonts/lives.png", "0123456789", 1);

	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	DrawColliders();
	create_sensors();		
	resetLayers();


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(base_map);
	App->textures->Unload(guides);
	App->textures->Unload(overlay_down);
	App->textures->Unload(bounce_hamburger);
	App->textures->Unload(overlay);
	App->textures->Unload(left_flipper);
	App->textures->Unload(right_flipper);
	App->textures->Unload(top_right_flipper);
	App->textures->Unload(ball);
	App->textures->Unload(slide);
	App->textures->Unload(ball_text);
	App->textures->Unload(sound);

	App->fonts->Unload(score);
	App->fonts->Unload(lives_font);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 5, b2_dynamicBody));
		circles.getLast()->data->listener = this;
	}
	App->renderer->Blit(base_map, 0, 0, NULL, 1.0f);
	App->renderer->Blit(overlay_down, 0, 0, NULL, 1.0f);
	App->renderer->Blit(bounce_hamburger, 0, 0, NULL, 1.0f);
	App->renderer->Blit(slide, 0, 0, NULL, 1.0f);
	
	App->player->player->GetPosition(App->player->position.x, App->player->position.y);
	App->renderer->Blit(ball, App->player->position.x, App->player->position.y, NULL, 1.0f, 1.0f, App->player->player->GetRotation());
	
	App->renderer->Blit(guides, 0, 0, NULL, 1.0f);
	App->renderer->Blit(overlay, 0, 0, NULL, 1.0f);
	App->renderer->Blit(ball_text, 415, 244, NULL, 1.0f);
	App->renderer->Blit(sound, 325, 244, NULL, 1.0f);

	//Blit score
	sprintf_s(player_lives, 10, "%d", App->player->lives);

	App->fonts->BlitText(385, 190, score, "000", 0.7f);
	App->fonts->BlitText(470, 242, lives_font, player_lives, 0.5f);

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
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
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE) {
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
	
	b2PrismaticJoint* spring = App->physics->spring;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
		spring->SetMaxMotorForce(5.0f);
		spring->SetMotorSpeed(2.0f);

	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP) {
		spring->SetMaxMotorForce((m_box->body->GetPosition().y - PIXEL_TO_METERS(436))*28);//Transforms the position to the collision force with the ball
		spring->SetMotorSpeed(-50.0f);
	}

	if (m_box->body->GetPosition().y < PIXEL_TO_METERS(436)) {
		m_box->body->SetTransform({ PIXEL_TO_METERS(296), PIXEL_TO_METERS(436)}, 0);
		spring->SetMotorSpeed(0.0f);
	}else
		m_box->body->SetTransform({ PIXEL_TO_METERS(296), m_box->body->GetPosition().y }, 0);
	

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
	
	int x, y;

	l_kicker->GetPosition(x, y);
	App->renderer->Blit(left_flipper, x, y, NULL, 1.0f, l_kicker->GetRotation(), PIXEL_TO_METERS(93), PIXEL_TO_METERS(447));

	tr_kicker->GetPosition(x, y);
	App->renderer->Blit(top_right_flipper, x, y, NULL, 1.0f, tr_kicker->GetRotation(), PIXEL_TO_METERS(217), PIXEL_TO_METERS(447));
	
	r_kicker->GetPosition(x, y);
	App->renderer->Blit(right_flipper, x, y, NULL, 1.0f, r_kicker->GetRotation(), PIXEL_TO_METERS(278), PIXEL_TO_METERS(270));

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyB->sensor_type != NONE) {
		p2List_item<PhysBody*>* sensor;
		sensor = App->scene_intro->sensors.getFirst();

		for (sensor; sensor != nullptr; sensor = sensor->next) {
			if (sensor->data == bodyB) {
				sensorAction(sensor->data);
			}
		}
	}

	App->audio->PlayFx(bonus_fx);
}


void ModuleSceneIntro::sensorAction(PhysBody* sensor) {

	p2List_item<PhysBody*>* c;
	switch (sensor->sensor_type) {
	case DISABLE_START_DOOR:
		for (b2Fixture* f = door_->body->GetFixtureList(); f; f = f->GetNext())
		{
			f->SetSensor(true);
		}
		break;
	case ACTIVE_START_DOOR:
		for (b2Fixture* f = door_->body->GetFixtureList(); f; f = f->GetNext())
		{
			f->SetSensor(false);
		}
		break;
	case TOP_LEFT_IN:
		for (b2Fixture* f = top_left_wall_->body->GetFixtureList(); f; f = f->GetNext())
		{
			f->SetSensor(true);
		}

		c = lgreen_tube_entrance.getFirst();
		while (c != NULL)
		{
			for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
			{
				f->SetSensor(false);
			}
			c = c->next;
		}
		break;
	case TOP_LEFT_OUT:
		for (b2Fixture* f = top_left_wall_->body->GetFixtureList(); f; f = f->GetNext())
		{
			f->SetSensor(false);
		}
		break;
	case TOP_RIGHT_IN:
		for (b2Fixture* f = top_right_wall_->body->GetFixtureList(); f; f = f->GetNext())
		{
			f->SetSensor(true);
		}
		break;
	case TOP_RIGHT_OUT:
		for (b2Fixture* f = top_right_wall_->body->GetFixtureList(); f; f = f->GetNext())
		{
			f->SetSensor(false);
		}
		break;
	case RAIL_IN:
		c = rail.getFirst();
		while (c != NULL)
		{
			for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
			{
				f->SetSensor(false);
			}
			c = c->next;
		}

		c = base_layer.getFirst();
		while (c != NULL)
		{
			for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
			{
				f->SetSensor(true);
			}
			c = c->next;
		}

		c = lgreen_tube_entrance.getFirst();
		while (c != NULL)
		{
			for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
			{
				f->SetSensor(true);
			}
			c = c->next;
		}
		break;
		
	case RAIL_END:
		c = rail.getFirst();
		while (c != NULL)
		{
			for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
			{
				f->SetSensor(true);
			}
			c = c->next;
		}

		c = base_layer.getFirst();
		while (c != NULL)
		{
			for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
			{
				f->SetSensor(false);
			}
			c = c->next;
		}

		c = lgreen_tube_entrance.getFirst();
		while (c != NULL)
		{
			for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
			{
				f->SetSensor(false);
			}
			c = c->next;
		}
		break;
	case GREEN_TUBE_IN:

		c = lgreen_tube_entrance.getFirst();
		while (c != NULL)
		{
			for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
			{
				f->SetSensor(false);
			}
			c = c->next;
		}

		c = rail.getFirst();
		while (c != NULL)
		{
			for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
			{
				f->SetSensor(true);
			}
			c = c->next;
		}

		c = base_layer.getFirst();
		while (c != NULL)
		{
			for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
			{
				f->SetSensor(true);
			}
			c = c->next;
		}

		c = lgreen_tube_exit.getFirst();
		while (c != NULL)
		{
			for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
			{
				f->SetSensor(true);
			}
			c = c->next;
		}
		break;
	case GREEN_TUBE_MIDDLE:

		c = lgreen_tube_entrance.getFirst();
		while (c != NULL)
		{
			for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
			{
				f->SetSensor(true);
			}
			c = c->next;
		}

		c = lgreen_tube_exit.getFirst();
		while (c != NULL)
		{
			for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
			{
				f->SetSensor(false);
			}
			c = c->next;
		}

		//ADD CONDITION TO TELEPORT THE BALL

		break;
	case GREEN_TUBE_OUT:

		c = lgreen_tube_entrance.getFirst();
		while (c != NULL)
		{
			for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
			{
				f->SetSensor(false);
			}
			c = c->next;
		}
		
		c = base_layer.getFirst();
		while (c != NULL)
		{
			for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
			{
				f->SetSensor(false);
			}
			c = c->next;
		}

		c = lgreen_tube_exit.getFirst();
		while (c != NULL)
		{
			for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
			{
				f->SetSensor(true);
			}
			c = c->next;
		}
		break;
	case LEFT_PERIMETER:

		c = lgreen_tube_entrance.getFirst();
		while (c != NULL)
		{
			for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
			{
				f->SetSensor(true);
			}
			c = c->next;
		}
	}
}


void ModuleSceneIntro::DrawColliders()
{

	int perimeter_init[64] = {

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
		305,	222
	};

	base_layer.add(App->physics->CreateChain(0, 0, perimeter_init, 64));

	int door[4] = {

		303,	222,
		289,	246
	};

	//door_ = App->physics->CreateChain(0, 0, door, 4);

	int perimeter_final[14] = {
		285,	246,
		280,	260,
		288,	272,
		266,	300,
		288,	354,
		288,	438,
		194,	500
	};

	base_layer.add(App->physics->CreateChain(0, 0, perimeter_final, 14));

	

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

	base_layer.add(App->physics->CreateChain(0, 0, top_right, 68));

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

	base_layer.add(App->physics->CreateChain(0, 0, top_left_pill, 14));

	int top_right_pill[14] = {

		147,	24,
		142,	29,
		142,	48,
		147,	53,
		152,	48,
		152,	29,
		147,	24
	};

	base_layer.add(App->physics->CreateChain(0, 0, top_right_pill, 14));

	int down_left[12] = {

		119,	480,
		37,		420,
		37,		366,
		40,		366,
		40,		408,
		85,		440
	};

	down_left_ = App->physics->CreateChain(0, 0, down_left, 12);

	int down_right[12] = {

		180,	480,
		271,	420,
		271,	366,
		268,	366,
		268,	408,
		220,	440
	};

	down_right_ = App->physics->CreateChain(0, 0, down_right, 12);

	int top_left_wall[4] = {

		97,		24,
		91,		13
	};

	top_left_wall_ = App->physics->CreateChain(0, 0, top_left_wall, 4);

	int top_right_wall[4] = {

		179,	18,
		181,	4
	};

	top_right_wall_ = App->physics->CreateChain(0, 0, top_right_wall, 4);

	int right_limit[4] = {

		303,	436,
		303,	230
	};

	right_limit_ = App->physics->CreateChain(0, 0, right_limit, 4);

	int left_limit[4] = {

		288,	436,
		288,	250
	};

	left_limit_ = App->physics->CreateChain(0, 0, left_limit, 4);

	int rail_right[64] = {
		68,		255,
		45,		220,
		34,		200,
		24,		167,
		21,		135,
		23,		115,
		27,		101,
		34,		84,
		41,		71,
		52,		57,
		66,		45,
		96,		28,
		130,	18,
		166,	17,
		196,	19,
		223,	30,
		248,	48,
		266,	73,
		279,	99,
		287,	127,
		287,	157,
		280,	184,
		273,	204,
		262,	223,
		241,	246,
		233,	259,
		233,	276,
		240,	291,
		264,	313,
		268,	328,
		269,	344,
		269,	394
	};

	rail.add(App->physics->CreateChain(0, 0, rail_right, 64));

	int water_slide_in[74] = {

		104,	237,
		73,		199,
		63,		175,
		55,		149,
		55,		130,
		57,		114,
		62,		97,
		72,		78,
		88,		61,
		104,	47,
		133,	39,
		148,	37,
		160,	37,
		174,	38,
		186,	40,
		200,	44,
		216,	52,
		231,	63,
		243,	76,
		253,	93,
		259,	111,
		264,	130,
		264,	147,
		263,	165,
		260,	181,
		253,	198,
		243,	213,
		230,	227,
		218,	243,
		211,	259,
		209,	275,
		213,	288,
		222,	301,
		233,	314,
		243,	325,
		248,	339,
		248,	385
	};

	rail.add(App->physics->CreateChain(0, 0, water_slide_in, 74));

	int water_slide[36] = {

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
		96,		163
	};

	base_layer.add(App->physics->CreateChain(0, 0, water_slide, 36));
	
	int greentube_bottom[18] = {

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

	lgreen_tube_entrance.add(App->physics->CreateChain(0, 0, greentube_bottom, 18));

	int green_tube_up[20] = {
		42,		33,
		39,		22,
		34,		18,
		29,		18,
		25,		22,
		21,		29,
		21,		99,
		28,		116,
		44,		133,
		92,		163
	};

	lgreen_tube_entrance.add(App->physics->CreateChain(0, 0, green_tube_up, 20));

	int green_tube_out[4] = {

		59,		360,
		59,		27
	};

	lgreen_tube_exit.add(App->physics->CreateChain(0, 0, green_tube_out, 4));

	int green_tube_exit[4]{
		39,		360,
		39,		27
	};

	lgreen_tube_exit.add(App->physics->CreateChain(0, 0, green_tube_exit, 4));

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

	m_box = App->physics->CreateRectangle(296, 436, 10, 10);//290, 436, 14, 10
	m_box->body->SetGravityScale(0);
	//m_box->body->SetLinearDamping(1.7f);

	s_box = App->physics->CreateRectangle(296, 436, 10, 10,b2_staticBody);//290, 460, 14, 10
	s_box->body->SetGravityScale(0);
	//s_box->body->SetLinearDamping(1.7f);

	App->physics->createPrismatic(s_box->body, m_box->body);

}

void ModuleSceneIntro::create_kickers(int* kicker1, int* kicker2, int* kicker3)
{
	l_kicker = App->physics->CreateKickers(87, 440, kicker1,14); //dyn
	r_kicker = App->physics->CreateKickers(165, 440, kicker2, 14); //dyn
	tr_kicker = App->physics->CreateKickers(236, 265, kicker3, 14); //dyn

	pivot_body1 = App->physics->CreatePivots(93, 447, 11);
	pivot_body2 = App->physics->CreatePivots(217, 447, 11);
	pivot_body3 = App->physics->CreatePivots(278, 270, 9);

	App->physics->createJoint(pivot_body1->body,l_kicker->body, -0.16f, 0.25f, true);
	App->physics->createJoint(pivot_body2->body, r_kicker->body, -0.25f, 0.16f);
	App->physics->createJoint(pivot_body3->body, tr_kicker->body, -0.25f, 0.0f);
}

void ModuleSceneIntro::create_sensors()
{
	sensors.add(App->physics->CreateRectangleSensor(298, 312, 17, 3, DISABLE_START_DOOR)); 
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(298, 224, 17, 3, ACTIVE_START_DOOR)); //door
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(295, 198, 23, 3, CROWN_OUT)); 
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(211, 18, 10, 10, TOP_RIGHT_IN));
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(165, 11, 5, 12, TOP_RIGHT_OUT)); // top right
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(110, 21, 5, 12, TOP_LEFT_OUT)); //top left
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(80, 25, 10, 10, TOP_LEFT_IN));
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(80, 230, 40, 4, RAIL_IN)); //water_slide end
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(80, 240, 40, 4, RAIL_END)); //water_slide end
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(258, 375, 17, 4, RAIL_END)); //water_slide beginning
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(78, 175, 12, 25, GREEN_TUBE_IN));
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(10, 85, 15, 14, GREEN_TUBE_MIDDLE));
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(51, 355, 17, 4, GREEN_TUBE_OUT));
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(28, 395, 17, 4, LIGHT_BOTTOM));
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(51, 395, 17, 4, LIGHT_BOTTOM));
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(258, 395, 17, 4, LIGHT_BOTTOM)); //water_slide end
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(281, 395, 17, 4, LIGHT_BOTTOM)); //water_slide end
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(30, 230, 20, 20, LEFT_PERIMETER));
	sensors.getLast()->data->listener = this;
}

void ModuleSceneIntro::resetLayers() {

	p2List_item<PhysBody*>* c;

	c = lgreen_tube_entrance.getFirst();
	while (c != NULL)
	{
		for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
		{
			f->SetSensor(false);
		}
		c = c->next;
	}

	c = rail.getFirst();
	while (c != NULL)
	{
		for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
		{
			f->SetSensor(true);
		}
		c = c->next;
	}

	c = base_layer.getFirst();
	while (c != NULL)
	{
		for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
		{
			f->SetSensor(false);
		}
		c = c->next;
	}

	c = lgreen_tube_exit.getFirst();
	while (c != NULL)
	{
		for (b2Fixture* f = c->data->body->GetFixtureList(); f; f = f->GetNext())
		{
			f->SetSensor(true);
		}
		c = c->next;
	}
}