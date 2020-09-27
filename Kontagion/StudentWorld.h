#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <sstream> 
#include <iomanip>  
#include <list>

class Actor;
class Socrates;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
	~StudentWorld();
    virtual int init();
    virtual int move();
	virtual void cleanUp();
	void addActor(Actor* anActor);
	bool SocratesOverlap(Actor* anActor);
	bool movementOverlap(Actor* anActor, double x);
	bool useToDestroy(Actor* anActor);
	int getFoodAngle(Actor* anActor);
	int getSocratesAngle(Actor* anActor, double minDistance);
	void damageSocrates(int x);
	void incrementDeadPits();
	void restoreSocrates();
	void restoreFlames();
private:
	Socrates* character;
	std::list<Actor*> actors;
	double getDistance(int x1, int x2, int y1, int y2);
	int deadPits;
	bool allBacteriaDead();
	bool generalOverlap(Actor* anActor);
	void addPickUps();
};

#endif // STUDENTWORLD_H_
