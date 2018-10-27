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
<<<<<<< HEAD
	create_sensors();
	
=======


>>>>>>> 25e8eb376a5208dd67983cb1ca06688739e8dcf9
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

	UpdateSensors();

	//Blit score
	App->fonts->BlitText(385, 190, score, "000", 0.5f);


	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
<<<<<<< HEAD
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 5, b2_dynamicBody));
=======
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 7, b2_dynamicBody));
>>>>>>> 25e8eb376a5208dd67983cb1ca06688739e8dcf9
		circles.getLast()->data->listener = this;
	}

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
		spring->SetMaxMotorForce((m_box->body->GetPosition().y - PIXEL_TO_METERS(436))*25);//Transforms the position to the collision force with the ball
		spring->SetMotorSpeed(-500.0f);
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
	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyB->sensor) {
		p2List_item<PhysBody*>* sensor;
		sensor = App->scene_intro->sensors.getFirst();

		for (sensor; sensor != nullptr; sensor = sensor->next) {
			if (sensor->data == bodyB) {
				sensor->data->active = true;
			}
		}
	}

	App->audio->PlayFx(bonus_fx);
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
		303,	222
	};

	perimeter_init_ = App->physics->CreateChain(0, 0, perimeter_init, 64);

	/*int door[4] = {

		303,	222,
		289,	246
	};*/

	/*door_ = App->physics->CreateChain(0, 0, door, 4);*/

	int perimeter_final[14] = {
		289,	246,
		280,	260,
		288,	272,
		266,	300,
		288,	354,
		288,	438,
		194,	500
	};

	perimeter_final_ = App->physics->CreateChain(0, 0, perimeter_final, 14);

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

		119,	472,
		38,		417,
		38,		366,
		40,		366,
		40,		410,
		93,		445
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

	int left_limit[4] = {

		288,	436,
		288,	250
	};

	left_limit_ = App->physics->CreateChain(0, 0, left_limit, 4);

	int water_slide_out[64] = {

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

	water_slide_out_ = App->physics->CreateChain(0, 0, water_slide_out, 64);

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

	water_slide_in_ = App->physics->CreateChain(0, 0, water_slide_in, 74);

	int green_tube_in[22] = {

		42,		372,
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

	green_tube_in_ = App->physics->CreateChain(0, 0, green_tube_in, 22);

	int green_tube_out[32] = {

		59,		367,
		59,		27,
		57,		16,
		21,		8,
		45,		2,
		40,		0,
		23,		0,
		17,		2,
		10,		8,
		5,		17,
		4,		30,
		3,		90,
		5,		117,
		13,		142,
		35,		167,
		91,		200
	};

	green_tube_out_ = App->physics->CreateChain(0, 0, green_tube_out, 32);

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
	288,	436,
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
	l_kicker = App->physics->CreateKickers(94, 443, kicker1,14); //dyn
	r_kicker = App->physics->CreateKickers(152, 443, kicker2, 14); //dyn
	tr_kicker = App->physics->CreateKickers(236, 265, kicker3, 14); //dyn

	pivot_body1 = App->physics->CreatePivots(100,448,9);
	pivot_body2 = App->physics->CreatePivots(205, 448, 9);
	pivot_body3 = App->physics->CreatePivots(278, 270, 9);

	App->physics->createJoint(pivot_body1->body,l_kicker->body, -0.16f, 0.25f, true);
	App->physics->createJoint(pivot_body2->body, r_kicker->body, -0.25f, 0.16f);
	App->physics->createJoint(pivot_body3->body, tr_kicker->body, -0.25f, 0.0f);

	//l_kicker->GetPosition(position.x, position.y);
	App->renderer->Blit(right_flipper, 0, 0, NULL, 1.0f);

}

void ModuleSceneIntro::create_sensors()
{
	sensors.add(App->physics->CreateRectangleSensor(298, 312, 17, 3, BALL)); 
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(298, 224, 17, 3, DOOR)); //door
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(295, 198, 23, 3, RIGHT_PERIMETER));
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(211, 18, 10, 10, TOP_RIGHT));
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(181, 11, 5, 12, TOP_RIGHT_DOOR)); // top right
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(92, 21, 10, 12, TOP_LEFT_DOOR)); //top left
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(62, 40, 10, 10, TOP_LEFT));
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(258, 395, 17, 4, WATER_SLIDE_END)); //water_slide end
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(57, 208, 20, 20, WATER_SLIDE_BEGINNING)); //water_slide beginning
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(51, 395, 17, 4, GREEN_TUBE_END));
	sensors.getLast()->data->listener = this;
	sensors.add(App->physics->CreateRectangleSensor(30, 230, 20, 20, LEFT_PERIMETER));
	sensors.getLast()->data->listener = this;
}

void ModuleSceneIntro::UpdateSensors()
{
	int door[4] = {

		303,	222,
		289,	246
	};

	p2List_item<PhysBody*>* sensor;
	sensor = sensors.getFirst();

	for (sensor; sensor != nullptr; sensor = sensor->next)
	{
		if (sensor->data->active && sensor->data->sensor_type == DOOR)
		{
			door_ = App->physics->CreateChain(0, 0, door, 4);
			sensor->data->active = false;
		}
	}
}
