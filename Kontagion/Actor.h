#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {

public:
	Actor(int imgID, int X, int Y, Direction dir, int depthObj, StudentWorld* world);
	virtual ~Actor();
	virtual void doSomething() = 0;
	virtual bool isDamageable();
	virtual bool isEatable();
	virtual bool isBacteria();
	bool overlapCheck(Actor* anActor);
	bool movementOverlapCheck(int X, int Y);
	virtual bool hasHealth();
	virtual void takeDamage(int x);
	virtual int getDamage();
	void setDead();
	virtual bool isDead();
	virtual void playHurtSound();
	virtual void playDeathSound();
	StudentWorld* getWorld() const;
private:
	bool m_dead = false;
	StudentWorld* m_world;
};

//Socrates Stuff
class Socrates : public Actor {

public:
	Socrates(StudentWorld* world);
	virtual ~Socrates();
	virtual void doSomething();
	virtual bool isDead();
	int getDisinfectantCount();
	int getFlameCount();
	int getHealth();
	void takeDamage(int x);
	void restoreHealth();
	void addFlames();
	void decrementDisinfectantCount();
	void increaseDisinfectantCount();
	void decrementFlameCount();
private:
	int disinfectantCount = 20;
	int flameCount = 5;
	int healthPoints = 100;
};


//Bacteria Stuff
class Bacteria : public Actor {

public:
	Bacteria(int imgID, int X, int Y, StudentWorld* world);
	virtual bool hasHealth();
	virtual bool isDead();
	virtual void doSomething() = 0;
	virtual void takeDamage(int x);
	int getFoodEaten();
	int getMovementPlanDist();
	virtual void playHurtSound() = 0;
	virtual void playDeathSound() = 0;
	virtual bool isBacteria();
	virtual ~Bacteria();
protected:
	void setHealth(int x);
	void decreaseMovementPlanDist();
	void resetFoodEaten();
	void resetMovementPlanDist();
	void eatFood();
	void becomeFood();
	void movementReset();
	void computeNewCoords(int& x, int& y);
	void finalMovement();
private:
	int movementPlanDist = 0;
	int healthPoints;
	int foodEaten = 0;
};


//Salmonella
class Salmonella : public Bacteria {

public:
	Salmonella(int X, int Y, StudentWorld* world);
	virtual void doSomething();
	virtual ~Salmonella();
	virtual void playHurtSound();
	virtual void playDeathSound();
};


//Aggressive Salmonella
class AggressiveSalmonella : public Bacteria {

public:
	AggressiveSalmonella(int X, int Y, StudentWorld* world);
	virtual void doSomething();
	virtual ~AggressiveSalmonella();
	virtual void playHurtSound();
	virtual void playDeathSound();
};


//EColi
class EColi : public Bacteria {

public:
	EColi(int X, int Y, StudentWorld* world);
	virtual void doSomething();
	virtual ~EColi();
	virtual void playHurtSound();
	virtual void playDeathSound();
};


//Pit Stuff
class Pit : public Actor {

public:
	Pit(int X, int Y, StudentWorld* world);
	virtual void doSomething();
	virtual ~Pit();
private:
	int SalmonellaCount, AggressiveCount, EColiCount;
	bool isEmpty();
	void emitBacteria();
};


//Dirt Stuff
class Dirt : public Actor {

public:
	Dirt(int X, int Y, StudentWorld* world);
	virtual void doSomething();
	virtual ~Dirt();
	virtual bool isDamageable();
};


//Food Stuff
class Food : public Actor {

public:
	Food(int X, int Y, StudentWorld* world);
	virtual void doSomething();
	virtual ~Food();
	virtual bool isEatable();
};


//Projectile Class
class Projectile : public Actor {

public:
	Projectile(int imgID, int X, int Y, Direction dir, StudentWorld* world);
	virtual void doSomething();
	virtual ~Projectile();
	virtual int getDamage();
protected:
	void setTicks(int x);
	void setDamage(int x);
	void incrementTicks();
	int getTickCount();
	bool tryToDestroy();
private:
	int startX;
	int startY;
	int ticksMoved = 0;
	int maxTicks;
	int damage;
};


//Disinfectant Spray
class Disinfectant : public Projectile {

public:
	Disinfectant(int X, int Y, Direction dir, StudentWorld* world);
	virtual ~Disinfectant();
};


//Flames
class Flame : public Projectile {

public:
	Flame(int X, int Y, Direction dir, StudentWorld* world);
	virtual ~Flame();
};


//PickUps Stuff
class PickUp : public Actor {

public:
	PickUp(int imgID, int X, int Y, StudentWorld* world);
	virtual void doSomething();
	virtual bool isDamageable();
	virtual ~PickUp();
protected:
	virtual void performUniqueAction() = 0;
	void decrementLifetime();
	int getLifetime();
private:
	int m_Lifetime;
};

//RestoreHealth Goodie
class RestoreHealth : public PickUp {

public:
	RestoreHealth(int X, int Y, StudentWorld* world);
	virtual ~RestoreHealth();
	virtual void performUniqueAction();
};

class FlameThrower : public PickUp {

public:
	FlameThrower(int X, int Y, StudentWorld* world);
	virtual ~FlameThrower();
	virtual void performUniqueAction();
};

class ExtraLife : public PickUp {

public:
	ExtraLife(int X, int Y, StudentWorld* world);
	virtual ~ExtraLife();
	virtual void performUniqueAction();
};

class Fungus : public PickUp {

public:
	Fungus(int X, int Y, StudentWorld* world);
	virtual ~Fungus();
	virtual void performUniqueAction();
};
#endif // ACTOR_H_
