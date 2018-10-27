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
	DISABLE_START_DOOR,
	ACTIVE_START_DOOR,
	RAIL_IN,
	RAIL_END,
	CROWN_OUT,
	LEFT_PERIMETER,
	TOP_RIGHT_IN,
	TOP_LEFT_IN,
	TOP_RIGHT_OUT,
	TOP_LEFT_OUT,
	GREEN_TUBE_IN,
	GREEN_TUBE_MIDDLE,
	GREEN_TUBE_OUT,
	LIGHT_BOTTOM
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


	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	Sensors_List sensor_type;
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

	PhysBody* CreateCircle(int x, int y, int radius,b2BodyType type = b2_staticBody);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, Sensors_List sensor);
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType type = b2_dynamicBody);
	PhysBody* CreateChain(int x, int y, int* points, int size);


	PhysBody* CreatePivots(int x, int y, int diameter);
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