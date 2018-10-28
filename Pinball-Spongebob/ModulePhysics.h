#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum Sensors_List {
	NONE = -1,
	DISABLE_START_DOOR ,
	ACTIVE_START_DOOR,
	RAIL_IN,
	RAIL_MIDDLE,
	RAIL_MIDDLE_PREV,
	RAIL_END,
	CROWN_IN,
	CROWN_OUT,
	LEFT_PERIMETER,
	TOP_RIGHT_IN,
	TOP_LEFT_IN,
	TOP_RIGHT_OUT,
	TOP_LEFT_OUT,
	GREEN_TUBE_IN,
	GREEN_TUBE_MIDDLE,
	GREEN_TUBE_OUT,
	LIGHT_BOTTOM1,
	LIGHT_BOTTOM2,
	LIGHT_BOTTOM3,
	LIGHT_BOTTOM4,
	LIGHT_MIDDLE1,
	LIGHT_MIDDLE2,
	LIGHT_MIDDLE3,
	LIGHT_MIDDLE4,
	LIGHT_MIDDLE5,
	LIGHT_TOP1,
	LIGHT_TOP2,
	LIGHT_TOP3,
	BOUNCER_LEFT,
	BOUNCER_RIGHT,
	HAMBURGER1,
	HAMBURGER2,
	HAMBURGER3
};


enum Scores
{
	NONE_ = -1,
	HAMBURGERS,
	HOLE,
	GREEN_TUBE,
	WATER_SLIDE
};

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;

	bool active = false;

	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	Sensors_List sensor_type;
	Scores score_points;
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	PhysBody * CreateKickers(int x, int y, int * points, int size);
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius,b2BodyType type = b2_staticBody, float restitution = 0.0f, Sensors_List sensor = NONE);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, Sensors_List sensor);
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType type = b2_dynamicBody);
	PhysBody* CreateChain(int x, int y, int* points, int size, int restitution = 0.0f, Sensors_List sensor = NONE);
	PhysBody* CreatePivots(int x, int y, int diameter);
	PhysBody* CreateCircleSensor(int x, int y, int radius, Scores score);
	
	void createJoint(b2Body* bodyA, b2Body* bodyB, float low, float high, bool flip = false);
	void createPrismatic(b2Body* bodyA, b2Body* bodyB);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);
	b2World* world;

private:

	bool debug;
	
	b2MouseJoint* mouse_joint;
	b2Body* ground;

public:
	bool layer[6];
	p2List<b2RevoluteJoint*> joints;
	b2PrismaticJoint* spring;
};