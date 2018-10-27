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

	int lives;
	bool dead = false;;

private:

	void SetBall(int x, int y);
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void Lives();
};