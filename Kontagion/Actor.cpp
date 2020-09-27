#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//Actor Stuff
Actor::Actor(int imgID, int X, int Y, Direction dir, int depthObj, StudentWorld* world)
	:GraphObject(imgID, X, Y, dir, depthObj)
{
	m_world = world;
}

Actor::~Actor() 
{}

bool Actor::isDead() 
{
	return m_dead;
}

bool Actor::isDamageable() {
	return false;
}

bool Actor::isEatable() {
	return false;
}

bool Actor::isBacteria() {
	return false;
}

bool Actor::hasHealth() {
	return false;
}

int Actor::getDamage() {
	return 0;
}

void Actor::playHurtSound() 
{}

void Actor::playDeathSound()
{}

void Actor::setDead() {
	m_dead = true;
}

void Actor::takeDamage(int x) {
	return;
}

bool Actor::overlapCheck(Actor* anActor) {
	if (sqrt(pow(getX() - anActor->getX(), 2.0) + pow(getY() - anActor->getY(), 2.0)) <= SPRITE_WIDTH)
		return true;
	return false;
}

bool Actor::movementOverlapCheck(int X, int Y) {
	if (sqrt(pow(getX() - X, 2.0) + pow(getY() - Y, 2.0)) <= SPRITE_WIDTH / 2.0)
		return true;
	return false;
}

StudentWorld* Actor::getWorld() const {
	return m_world;
}

//Socrates Stuff
Socrates::Socrates(StudentWorld* world) 
	:Actor(IID_PLAYER, 0, 128, 0, 0, world)
{}

Socrates::~Socrates() 
{}

int Socrates::getFlameCount() {
	return flameCount;
}

int Socrates::getDisinfectantCount() {
	return disinfectantCount;
}

int Socrates::getHealth() {
	return healthPoints;
}

bool Socrates::isDead() {
	if (healthPoints <= 0)
		return true;
	return false;
}

void Socrates::takeDamage(int x) {
	healthPoints -= x;
}

void Socrates::restoreHealth() {
	healthPoints = 100;
}

void Socrates::addFlames() {
	flameCount += 5;
}

void Socrates::decrementDisinfectantCount() {
	disinfectantCount--;
}

void Socrates::increaseDisinfectantCount() {
	disinfectantCount++;
}

void Socrates::decrementFlameCount() {
	flameCount--;
}

void Socrates::doSomething() {
	if (getHealth() <= 0)
		setDead();
	if (isDead())
		return;
	int ch;
	if (getWorld()->getKey(ch)) {
		const double PI = 4 * atan(1);
		switch (ch)
		{
			case KEY_PRESS_LEFT:
				setDirection(getDirection() + 5);
				moveTo(128 * cos((getDirection() + 180) * (PI / 180)) + 128, 128 * sin((getDirection() + 180) * (PI / 180)) + 128);
				break;
			case KEY_PRESS_RIGHT:
				setDirection(getDirection() - 5);
				moveTo(128 * cos((getDirection() + 180) * (PI / 180)) + 128, 128 * sin((getDirection() + 180) * (PI / 180)) + 128);
				break;
			case KEY_PRESS_SPACE:
				if (getDisinfectantCount() > 0) {
					getWorld()->addActor(new Disinfectant(getX() + cos(getDirection() * (PI / 180)), getY() + sin(getDirection() * (PI / 180)), getDirection(), getWorld()));
					getWorld()->playSound(SOUND_PLAYER_SPRAY);
				}
				if (getDisinfectantCount() > 0)
					decrementDisinfectantCount();
				break;
			case KEY_PRESS_ENTER:
				if (getFlameCount() > 0) {
					for (int i = 0; i < 16; i++) {
						getWorld()->addActor(new Flame(getX() + cos((getDirection() + 22 * i) * (PI / 180)), getY() + sin((getDirection() + 22 * i) * (PI / 180)), getDirection() + 22 * i, getWorld()));
					}
					getWorld()->playSound(SOUND_PLAYER_FIRE);
				}
				if (getFlameCount() > 0)
					decrementFlameCount();
				break;
			default:
				break;
		}
	}
	else {
		if (getDisinfectantCount() < 20) {
			increaseDisinfectantCount();
		}
	}
}


//Bacteria Stuff
Bacteria::Bacteria(int imgID, int X, int Y, StudentWorld* world)
	:Actor(imgID, X, Y, 90, 0, world)
{}

bool Bacteria::isBacteria() {
	return true;
}

bool Bacteria::isDead() {
	if (healthPoints <= 0)
		return true;
	return false;
}

bool Bacteria::hasHealth() {
	return true;
}

int Bacteria::getFoodEaten() {
	return foodEaten;
}

int Bacteria::getMovementPlanDist() {
	return movementPlanDist;
}

void Bacteria::resetMovementPlanDist() {
	movementPlanDist = 10;
}

void Bacteria::setHealth(int x) {
	healthPoints = x;
}

void Bacteria::takeDamage(int x) {
	healthPoints -= x;
}

void Bacteria::decreaseMovementPlanDist() {
	movementPlanDist--;
}

void Bacteria::eatFood() {
	foodEaten++;
}

void Bacteria::resetFoodEaten() {
	foodEaten = 0;
}

void Bacteria::becomeFood() {
	int newFood = randInt(0, 1);
	if (newFood == 0) {
		getWorld()->addActor(new Food(getX(), getY(), getWorld()));
	}
}

void Bacteria::computeNewCoords(int& x, int& y) {
	if (getX() < VIEW_WIDTH / 2)
		x = getX() + SPRITE_RADIUS;
	else if (getX() > VIEW_WIDTH / 2)
		x = getX() - SPRITE_RADIUS;
	if (getY() < VIEW_HEIGHT / 2)
		y = getY() + SPRITE_RADIUS;
	if (getY() > VIEW_HEIGHT / 2)
		y = getY() - SPRITE_RADIUS;
}

void Bacteria::movementReset() {
	setDirection(randInt(0, 359));
	resetMovementPlanDist();
}

void Bacteria::finalMovement() {
	const double PI = 4 * atan(1);
	if (getMovementPlanDist() > 0) {
		decreaseMovementPlanDist();
		if (getWorld()->movementOverlap(this, 3.0) == false) {
			moveTo(getX() + (3 * cos(getDirection() * (PI / 180))), getY() + (3 * sin(getDirection() * (PI / 180))));
		}
		else {
			movementReset();
		}
		return;
	}
	else {
		int newDir = getWorld()->getFoodAngle(this);
		if (newDir < -180) {
			movementReset();
		}
		else {
			setDirection(newDir);
			if (getWorld()->movementOverlap(this, 3) == true) {
				movementReset();
				return;
			}
			else {
				moveTo(getX() + (3 * cos(getDirection() * (PI / 180))), getY() + (3 * sin(getDirection() * (PI / 180))));
			}
		}
	}
}

Bacteria::~Bacteria()
{}


//Salmonella Stuff
Salmonella::Salmonella(int X, int Y, StudentWorld* world)
	:Bacteria(IID_SALMONELLA, X, Y, world)
{
	setHealth(4);
}

void Salmonella::playHurtSound() {
	getWorld()->playSound(SOUND_SALMONELLA_HURT);
}

void Salmonella::playDeathSound() {
	getWorld()->playSound(SOUND_SALMONELLA_DIE);
}

void Salmonella::doSomething() {
	if (isDead()) {
		becomeFood();
		return;
	}
	if (getWorld()->SocratesOverlap(this)) {
		getWorld()->damageSocrates(1);
	}
	else {
		if (getFoodEaten() == 3) {
			int newx, newy;
			computeNewCoords(newx, newy);
			getWorld()->addActor(new Salmonella(newx, newy, getWorld()));
			resetFoodEaten();
		}

		else {
			if (getWorld()->useToDestroy(this))
				eatFood();
		}
	}
	finalMovement();
}

Salmonella::~Salmonella()
{}


//Aggressive Salmonella
AggressiveSalmonella::AggressiveSalmonella(int X, int Y, StudentWorld* world) 
	:Bacteria(IID_SALMONELLA, X, Y, world)
{
	setHealth(10);
}

void AggressiveSalmonella::playHurtSound() {
	getWorld()->playSound(SOUND_SALMONELLA_HURT);
}

void AggressiveSalmonella::playDeathSound() {
	getWorld()->playSound(SOUND_SALMONELLA_DIE);
}

void AggressiveSalmonella::doSomething() {
	if (isDead()) {
		becomeFood();
		return;
	}
	bool behaviorDetermine = false;
	const double PI = 4 *atan(1);
	int newDir = getWorld()->getSocratesAngle(this, 72.0);
	if (newDir >= -180) {
		behaviorDetermine = true;
		setDirection(newDir);
		if (getWorld()->movementOverlap(this, 3.0) == false)
			moveTo(getX() + (3 * cos(getDirection() * (PI / 180.0))), getY() + (3 * sin(getDirection() * (PI / 180.0))));
	}
	if (getWorld()->SocratesOverlap(this)) {
		getWorld()->damageSocrates(2);
		if (behaviorDetermine)
			return;
	}
	else {
		if (getFoodEaten() == 3) {
			int newx, newy;
			computeNewCoords(newx, newy);
			getWorld()->addActor(new AggressiveSalmonella(newx, newy, getWorld()));
			resetFoodEaten();
			if (behaviorDetermine)
				return;
		} 
		else {
			if (getWorld()->useToDestroy(this))
				eatFood();
		}
	}
	if (!behaviorDetermine) {
		finalMovement();
	}
}

AggressiveSalmonella::~AggressiveSalmonella()
{}


//EColi Stuff
EColi::EColi(int X, int Y, StudentWorld* world)
	:Bacteria(IID_ECOLI, X, Y, world)
{
	setHealth(5);
}

void EColi::playHurtSound() {
	getWorld()->playSound(SOUND_ECOLI_HURT);
}

void EColi::playDeathSound() {
	getWorld()->playSound(SOUND_ECOLI_DIE);
}

void EColi::doSomething() {
	if (isDead())
		becomeFood();
	if (getWorld()->SocratesOverlap(this)) {
		getWorld()->damageSocrates(4);
	}
	else {
		if (getFoodEaten() == 3) {
			int newx, newy;
			computeNewCoords(newx, newy);
			getWorld()->addActor(new EColi(newx, newy, getWorld()));
			resetFoodEaten();
		}

		else {
			if (getWorld()->useToDestroy(this))
				eatFood();
		}
	}
	int newAngle;
	newAngle = getWorld()->getSocratesAngle(this, 256.0);
	if (newAngle >= -180) {
		setDirection(newAngle);
		for (int i = 0; i < 10; i++) {
			if (getWorld()->movementOverlap(this, 2.0) == false) {
				moveAngle(getDirection(), 2.0);
				return;
			}
			else {
				setDirection(getDirection() + 10);
			}
		}
	}
}

EColi::~EColi()
{}


//Pit Stuff
Pit::Pit(int X, int Y, StudentWorld* world)
	:Actor(IID_PIT, X, Y, 0, 1, world)
{
	SalmonellaCount = 5;
	AggressiveCount = 3;
	EColiCount = 2;
}

bool Pit::isEmpty() {
	if (SalmonellaCount == 0 && AggressiveCount == 0 && EColiCount == 0)
		return true;
	return false;
}

void Pit::emitBacteria() {
	int chance = randInt(0, 49);
	if (chance == 20) {	
		int validTypes[3] = { 0, 0, 0 };
		if (SalmonellaCount > 0)
			validTypes[0] = 1;
		if (AggressiveCount > 0)
			validTypes[1] = 1;
		if (EColiCount > 0)
			validTypes[2] = 1;
		while (true) {
			int emission = randInt(0, 2);
			if (validTypes[emission] == 1) {
				if (emission == 0) {
					getWorld()->addActor(new Salmonella(getX(), getY(), getWorld()));
					getWorld()->playSound(SOUND_BACTERIUM_BORN);
					SalmonellaCount--;
					return;
				}
				else if (emission == 1) {
					getWorld()->addActor(new AggressiveSalmonella(getX(), getY(), getWorld()));
					getWorld()->playSound(SOUND_BACTERIUM_BORN);
					AggressiveCount--;
					return;
				}
				else {
					getWorld()->addActor(new EColi(getX(), getY(), getWorld()));
					getWorld()->playSound(SOUND_BACTERIUM_BORN);
					EColiCount--;
					return;
				}
			}
		} 
	}
}

void Pit::doSomething() {
	if (isEmpty()) {
		setDead();
		getWorld()->incrementDeadPits();
	}
	else {
		emitBacteria();
	}
}

Pit::~Pit() 
{}


//Dirt Stuff
Dirt::Dirt(int X, int Y, StudentWorld* world)
	:Actor(IID_DIRT, X, Y, 0, 1, world)
{}

void Dirt::doSomething() {
	return;
}

Dirt::~Dirt() 
{}

bool Dirt::isDamageable() {
	return true;
}


//Food Stuff
Food::Food(int X, int Y, StudentWorld* world)
	:Actor(IID_FOOD, X, Y, 90, 1, world)
{}

void Food::doSomething() {
	return;
}

Food::~Food()
{}

bool Food::isEatable() {
	return true;
}

//Projectile Stuff
Projectile::Projectile(int imgID, int X, int Y, Direction dir, StudentWorld* world) 
	:Actor(imgID, X, Y, dir, 1, world)
{
	startX = X;
	startY = Y;
}

void Projectile::incrementTicks() {
	ticksMoved++;
}

int Projectile::getTickCount() {
	return ticksMoved;
}

int Projectile::getDamage() {
	return damage;
}

bool Projectile::tryToDestroy() {
	if (getWorld()->useToDestroy(this))
		return true;
	return false;
}

void Projectile::setTicks(int x) {
	maxTicks = x;
}

void Projectile::setDamage(int x) {
	damage = x;
}

void Projectile::doSomething() {
	if (isDead())
		return;

	if (tryToDestroy()) {
		setDead();
		return;
	}
	else {
		moveAngle(getDirection(), SPRITE_WIDTH);
		incrementTicks();

		if (getTickCount() > maxTicks)
			setDead();
	}
}


Projectile::~Projectile()
{}


//Disinfectant Stuff
Disinfectant::Disinfectant(int X, int Y, Direction dir, StudentWorld* world)
	:Projectile(IID_SPRAY, X, Y, dir, world)
{
	setTicks(14);
	setDamage(2);
}


Disinfectant::~Disinfectant()
{}


//Flame Stuff
Flame::Flame(int X, int Y, Direction dir, StudentWorld* world)
	:Projectile(IID_FLAME, X, Y, dir, world)
{
	setTicks(4);
	setDamage(5);
}

Flame::~Flame()
{}


//PickUps Stuff
PickUp::PickUp(int imgID, int X, int Y, StudentWorld* world)
	:Actor(imgID, X, Y, 0, 1, world)
{
	m_Lifetime = std::max(randInt(0, 300 - 10 * getWorld()->getLevel() - 1), 50);
}

int PickUp::getLifetime() {
	return m_Lifetime;
}

void PickUp::decrementLifetime() {
	m_Lifetime--;
}

bool PickUp::isDamageable() {
	return true;
}

void PickUp::doSomething() {
	if (isDead())
		return;
	else {
		if (getWorld()->SocratesOverlap(this)) {
			setDead();
			performUniqueAction();
			return;
		}
		else {
			decrementLifetime();
			if (getLifetime() <= 0)
				setDead();
		}
	}
}

PickUp::~PickUp() 
{}

//Restore Health Stuff
RestoreHealth::RestoreHealth(int X, int Y, StudentWorld* world) 
	:PickUp(IID_RESTORE_HEALTH_GOODIE, X, Y, world)
{}

void RestoreHealth::performUniqueAction() {
	getWorld()->restoreSocrates();
	getWorld()->increaseScore(250);
	getWorld()->playSound(SOUND_GOT_GOODIE);
}

RestoreHealth::~RestoreHealth()
{}

//FlameThrower Stuff
FlameThrower::FlameThrower(int X, int Y, StudentWorld* world)
	:PickUp(IID_FLAME_THROWER_GOODIE, X, Y, world)
{}

void FlameThrower::performUniqueAction() {
	getWorld()->restoreFlames();
	getWorld()->increaseScore(300);
	getWorld()->playSound(SOUND_GOT_GOODIE);
}

FlameThrower::~FlameThrower()
{}

//Extra Life Goodie Stuff
ExtraLife::ExtraLife(int X, int Y, StudentWorld* world)
	:PickUp(IID_EXTRA_LIFE_GOODIE, X, Y, world)
{}

void ExtraLife::performUniqueAction() {
	getWorld()->incLives();
	getWorld()->increaseScore(500);
	getWorld()->playSound(SOUND_GOT_GOODIE);
}

ExtraLife::~ExtraLife()
{}

//Fungus Stuff
Fungus::Fungus(int X, int Y, StudentWorld* world)
	:PickUp(IID_FUNGUS, X, Y, world)
{}

void Fungus::performUniqueAction() {
	getWorld()->damageSocrates(20);
	getWorld()->increaseScore(-50);
}

Fungus::~Fungus()
{}

