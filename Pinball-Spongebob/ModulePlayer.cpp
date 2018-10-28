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
{}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	lives = 5;

	ball = App->textures->Load("Assets/ball.png");
	high_score = App->textures->Load("Assets/high_score.png");
	score_font = App->fonts->Load("Assets/Fonts/font_score2.png", "0123456789", 1);

	SetBall(PLAYER_POS_X, PLAYER_POS_Y);
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(ball);
	App->textures->Unload(high_score);

	App->fonts->Unload(score_font);

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
		App->scene_intro->anchor_c = 0;

		if (lives != 0)
		{
			SetBall(PLAYER_POS_X, PLAYER_POS_Y);
		}
	}
	else
	{
		App->renderer->Blit(high_score, 125, 110, NULL);
		sprintf_s(high_score_text, 10, "%d", score);
		App->fonts->BlitText(210, 255, score_font, high_score_text, 0.7f);

		if (App->input->GetKey(SDL_SCANCODE_R))
		{
			SetBall(PLAYER_POS_X, PLAYER_POS_Y);
			lives = 5;
			score = 0;
		}
	}
}
