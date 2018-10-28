#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//ball = NULL;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	lives = 5;

	ball = App->textures->Load("Assets/ball.png");
	SetBall(PLAYER_POS_X, PLAYER_POS_Y);
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(ball);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	Lives();

	return UPDATE_CONTINUE;
}

void ModulePlayer::SetBall(int x, int y)
{
	player = App->physics->CreateCircle(x, y, 7, b2_dynamicBody);
	player->listener = (Module*)App->scene_intro;
}

void ModulePlayer::Lives()
{
	if (position.y < 500)
		dead = false;
	
	else if (position.y >= 500 && !dead)
	{
		dead = true;
		lives -= 1;

		if (lives != 0)
		{
			SetBall(PLAYER_POS_X, PLAYER_POS_Y);
		}
	}
	else
	{
		if (App->input->GetKey(SDL_SCANCODE_R))
		{
			SetBall(PLAYER_POS_X, PLAYER_POS_Y);
			lives = 5;
		}
	}
}
