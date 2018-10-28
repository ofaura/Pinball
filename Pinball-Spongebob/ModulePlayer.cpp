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

	high_score_tex = App->textures->Load("Assets/high_score.png");
	score_font = App->fonts->Load("Assets/Fonts/font_score2.png", "0123456789", 1);
	high_score_font = App->fonts->Load("Assets/Fonts/high_score_font.png", "0123456789", 1);

	death_fx = App->audio->LoadFx("Assets/Audio/death.wav");
	SetBall(PLAYER_POS_X, PLAYER_POS_Y);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(high_score_tex);

	App->fonts->Unload(score_font);
	App->fonts->Unload(high_score_font);

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
	player = App->physics->CreateCircle(x, y, 7, b2_dynamicBody,0.2f);
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
			App->audio->PlayFx(death_fx);
			SetBall(PLAYER_POS_X, PLAYER_POS_Y);
		}
	}
	else
	{
		App->renderer->Blit(high_score_tex, 125, 110, NULL);

		if (high_score < score)
			high_score = score;

		sprintf_s(score_text, 10, "%d", score);
		sprintf_s(high_score_text, 10, "%d", high_score);
		App->fonts->BlitText(210, 255, score_font, score_text, 0.7f);
		App->fonts->BlitText(190, 200, high_score_font, high_score_text, 0.7f);

		if (App->input->GetKey(SDL_SCANCODE_R))
		{
			SetBall(PLAYER_POS_X, PLAYER_POS_Y);
			lives = 5;
			score = 0;
		}
	}
}
