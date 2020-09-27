#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
using namespace std;


// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
	deadPits = 0;
}

StudentWorld::~StudentWorld() {
	cleanUp();
}

double StudentWorld::getDistance(int x1, int x2, int y1, int y2) {
	return sqrt(pow(x2 - x1, 2.0) + pow(y2 - y1, 2.0));
}

void StudentWorld::addActor(Actor* anActor) {
	actors.push_back(anActor);
}

bool StudentWorld::generalOverlap(Actor* anActor) {
	list<Actor*>::iterator it = actors.begin();
	while (it != actors.end()) {
		if (anActor->isDamageable() && (*it)->overlapCheck(anActor) && (*it)->isDamageable() == false)
			return true;
		else {
			if ((*it)->overlapCheck(anActor))
				return true;
		}
		it++;
	}
	return false;
}

void StudentWorld::incrementDeadPits() {
	deadPits++;
}

//Used for bacteria damaging Socrates and for Socrates consuming pickups
bool StudentWorld::SocratesOverlap(Actor* anActor) {
	if (character->overlapCheck(anActor))
		return true;
	return false;
}

bool StudentWorld::movementOverlap(Actor* anActor, double x) {
	double newX;
	double newY;
	anActor->getPositionInThisDirection(anActor->getDirection(), x, newX, newY);
	if (sqrt(pow(newX - VIEW_WIDTH / 2, 2.0) + pow(newY - VIEW_HEIGHT / 2, 2.0)) >= 128.0)
		return true;
	list<Actor*>::iterator it = actors.begin();
	while (it != actors.end()) {
		if ((*it)->isDamageable() && (*it)->movementOverlapCheck(newX, newY)) {
			return true;
		}
		it++;
	}
	return false;
}

//can be used for bacteria eating food as well
bool StudentWorld::useToDestroy(Actor* anActor) {
	list<Actor*>::iterator it = actors.begin();
	while (it != actors.end()) {
		if (anActor->isBacteria()) {
			if ((*it)->isEatable() && (*it)->overlapCheck(anActor)) {
				(*it)->setDead();
				return true;
			}
		}
		else if ((*it)->isDamageable() && (*it)->overlapCheck(anActor)) {
			if ((*it)->hasHealth() == false && (*it)->isDead() == false) {
				(*it)->setDead(); 
				return true;
			}
		}
		else if ((*it)->isBacteria() && (*it)->overlapCheck(anActor) && (*it)->isDead() == false) {
				(*it)->takeDamage(anActor->getDamage());
				(*it)->playHurtSound();
				return true;
		}
		it++;
	}
	return false;
}

bool StudentWorld::allBacteriaDead() {
	list<Actor*>::iterator it = actors.begin();
	while (it != actors.end()) {
		if ((*it)->isBacteria()) {
			if ((*it)->isDead() == false)
				return false;
		}
		it++;
	}
	if (deadPits == getLevel())
		return true;
	return false;
}

int StudentWorld::getFoodAngle(Actor* anActor) {
	Direction foodAngle = -1000;
	double minDistance = 128.0;
	list<Actor*>::iterator it = actors.begin();
	const double PI = 4 * atan(1);
	while (it != actors.end()) {
		if ((*it)->isEatable() && getDistance(anActor->getX(), (*it)->getX(), anActor->getY(), (*it)->getY()) <= minDistance) {
			foodAngle = (atan2(((*it)->getY() - anActor->getY()), ((*it)->getX() - anActor->getX()))) * (180 / PI);
			minDistance = getDistance(anActor->getX(), (*it)->getX(), anActor->getY(), (*it)->getY());
		}
		it++;
	}
	return foodAngle;
}

int StudentWorld::getSocratesAngle(Actor* anActor, double minDistance) {
	Direction angle = -1000;
	const double PI = 4 * atan(1);
	if (getDistance(anActor->getX(), character->getX(), anActor->getY(), character->getY()) <= minDistance) {
		angle = (atan2(((character)->getY() - anActor->getY()), ((character)->getX() - anActor->getX()))) * (180 / PI);
	}
	return angle;
}

void StudentWorld::addPickUps() {
	const double PI = 4 * atan(1);
	int maxFungus = max(510 - getLevel() * 10, 200);
	int chanceFungus = randInt(0, maxFungus);
	if (chanceFungus == 0) {
		int degreeAngle = randInt(0, 360);
		addActor(new Fungus(128 + 128 * cos(degreeAngle * (PI / 180)), 128 + 128 * sin(degreeAngle * (PI / 180)), this));
	}

	int maxGoodie = max(510 - getLevel() * 10, 250);
	int chanceGoodie = randInt(0, maxGoodie);
	if (chanceGoodie == 0) {
		int whichGoodie = randInt(1, 10);
		int degreeAngle = randInt(0, 360);
		if (whichGoodie == 1) {
			addActor(new ExtraLife(128 + 128 * cos(degreeAngle * (PI / 180)), 128 + 128 * sin(degreeAngle * (PI / 180)), this));
		}
		else if (whichGoodie == 2 || whichGoodie == 3 || whichGoodie == 4) {
			addActor(new FlameThrower(128 + 128 * cos(degreeAngle * (PI / 180)), 128 + 128 * sin(degreeAngle * (PI / 180)), this));
		}
		else {
			addActor(new RestoreHealth(128 + 128 * cos(degreeAngle * (PI / 180)), 128 + 128 * sin(degreeAngle * (PI / 180)), this));
		}
	}
}

void StudentWorld::damageSocrates(int x) {
	character->takeDamage(x);
	playSound(SOUND_PLAYER_HURT);
}

void StudentWorld::restoreSocrates() {
	character->restoreHealth();
}

void StudentWorld::restoreFlames() {
	character->addFlames();
}

int StudentWorld::init() {
	character = new Socrates(this);
	deadPits = 0;
	const double PI = 4 * atan(1);
	for (int i = 0; i < getLevel(); i++) {
		int radialDist = randInt(0, 120);
		int theta = randInt(0, 360);
		Actor* toBeAdded = new Pit(128 + radialDist * cos(theta * (PI / 180)), 128 + radialDist * sin(theta * (PI / 180)), this);
		if (generalOverlap(toBeAdded) == false)
			actors.push_back(toBeAdded);
		else {
			delete toBeAdded;
			i--;
		}
	}
	for (int i = 0; i < min(5 * getLevel(), 25); i++) {
		int radialDist = randInt(0, 120);
		int theta = randInt(0, 360);
		Actor* toBeAdded = new Food(128 + radialDist * cos(theta * (PI / 180)), 128 + radialDist * sin(theta * (PI / 180)), this);
		if (generalOverlap(toBeAdded) == false)
			actors.push_back(toBeAdded);
		else {
			delete toBeAdded;
			i--;
		}
	}
	for (int i = 0; i < max(180 - 20 * getLevel(), 20); i++) {
		int radialDist = randInt(0, 120);
		int theta = randInt(0, 360);
		Actor* toBeAdded = new Dirt(128 + radialDist * cos(theta * (PI / 180)), 128 + radialDist * sin(theta * (PI / 180)), this);
		if (generalOverlap(toBeAdded) == false)
			actors.push_back(toBeAdded);
		else {
			delete toBeAdded;
			i--;
		}
	}

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	character->doSomething();
	stringstream oss;
	list<Actor*>::iterator it = actors.begin();
	while (it != actors.end()) {
		if ((*it)->isDead() == false) {
			(*it)->doSomething();
		}
		if (character->isDead()) {
			decLives();
			playSound(SOUND_PLAYER_DIE);
			return GWSTATUS_PLAYER_DIED;
		}
		if (allBacteriaDead() == true) {
			playSound(SOUND_FINISHED_LEVEL);
			return GWSTATUS_FINISHED_LEVEL;
		}
		it++;
	}
	it = actors.begin();
	while (it != actors.end()) {
		if ((*it)->isDead()) {
			if ((*it)->isBacteria()) {
				increaseScore(100);
				(*it)->playDeathSound();
			}
			delete *(it);
			list<Actor*>::iterator temp = it;
			it++;
			actors.erase(temp);
		}
		else
			it++;
	}
	addPickUps();
	oss.fill('0');
	if (getScore() < 0) {
		oss << "Score: -" << setw(5) << (getScore() * -1) << "  ";
	}
	else {
		oss << "Score: " << setw(6) << getScore() << "  ";
	}
	oss.fill(' ');
	oss << "Level: " << setw(2) << getLevel() << "  ";
	oss << "Lives: " << setw(1) << getLives() << "  ";
	oss << "Health: " << setw(3) << character->getHealth() << "  ";
	oss << "Sprays: " << setw(2) << character->getDisinfectantCount() << "  ";
	oss << "Flames: " << setw(2) << character->getFlameCount() << "  ";
	setGameStatText(oss.str());
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	if (character) {
		delete character;
		character = nullptr;
	}
	list<Actor*>::iterator it = actors.begin();
	while (it != actors.end()) {
		delete *(it);
		list<Actor*>::iterator temp = it;
		it++;
		actors.erase(temp);
	}
}
