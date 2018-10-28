#include "Module.h"
#include "Globals.h"
#include "p2List.h"
#include "p2Point.h"
#include "Box2D/Box2D/Box2D.h"
#include "ModuleTextures.h"

#define PLAYER_POS_X 292
#define PLAYER_POS_Y 425

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	iPoint position;

	PhysBody* player;	
	
	SDL_Texture* ball;
	SDL_Texture* high_score_tex;

	char high_score_text[10];
	char score_text[10];

	int lives;
	uint32 score = 0;
	uint32 high_score = 0;

	int score_font = -1;
	int high_score_font = -1;
	bool dead = false;;

private:

	void SetBall(int x, int y);
	void Lives();
};