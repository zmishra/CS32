#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Compiler.h"
class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, int depth, StudentWorld* sw);
	virtual void doSomething() {} //Actors do nothing by default
	virtual bool isDead() { return false; } //Actors do not die by default
	virtual bool canMoveThrough() { return true; } //Actors can be moved through by default
	virtual int callCount() { return -1; } //Call count is always less than elapsed ticks by default
	virtual void poison() {} //Actors cannot be poisoned by default
	virtual void stun() {} //Actors cannot be stunned by default
	virtual bool isDangerous(int colony) { return false; } //Actors are not dangerous or enemies by default
	virtual bool isEnemy(int colony) { return false; }
	virtual bool isPheromone() { return false; } //Actors are not phermones by default
	virtual bool isMyPheromone(int colony) { return false; }
	virtual bool isMyAnthill(int colony) { return false; } //Actors are not anthills by default
	virtual bool isFood() { return false; } //Actors are not food by default
	virtual bool isInsect() { return false; } //Actors are not insects by default
	StudentWorld* world() { return m_world; }
	virtual ~Actor() {};
private:
	StudentWorld* m_world;
};

class Pebble : public Actor
{
public:
	Pebble(int startX, int startY, StudentWorld* sw);
	bool canMoveThrough() { return false; } //Pebbles can't be moved through
private:

};

class energyHolder : public Actor //Energy holders have health and can die
{
public:
	energyHolder(int imageID, int startX, int startY, int depth, int startHP, StudentWorld* sw);
	bool isDead() { return hp <= 0; }
	int changeHealthBy(int num);
	int health() { return hp; }
	int colonyNumber() { return m_colonyNumber; }
	void changeColonyNumber(int colony) { m_colonyNumber = colony; } //To allow changing of the colony number from its default value
	virtual ~energyHolder() {}
private:
	int hp;
	int m_colonyNumber; //All classes that need a colony number are derived from EnergyHolder
};

class Food : public energyHolder
{
public:
	Food(int startX, int startY, int startHP, StudentWorld* sw);
	virtual bool isFood() { return true; }
};

class Insect : public energyHolder //Insects can be stunned, poisoned, bitten, and can move
{
public:
	Insect(int imageID, int startX, int startY, int depth, int startHP, StudentWorld* sw);
	void stun();
	virtual void poison();
	int sleepTurns() { return m_sleepTurns; }
	int callCount() { return m_callCount; };
	bool isStunned() { return m_isStunned; };
	void changeSleepTurnsBy(int num) { m_sleepTurns += num; }
	void changeCallCountBy(int num) { m_callCount += num; }
	void switchStunState(bool stunned) { m_isStunned = stunned; }
	virtual void bittenFor(int dmg);
	void getForwardCoords(int& x, int& y); //put the coordinates of the square in front of the insect in x and y
	bool moveForward();
	virtual void doSomething();
	bool isInsect() { return true; }
	virtual ~Insect() {};
private:
	virtual void doSomethingHelperI() = 0;
	int m_sleepTurns;
	int m_callCount;
	bool m_isStunned;
};

class Grasshopper : public Insect
{
public:
	Grasshopper(int imageID, int startX, int startY, int startHP, StudentWorld* sw);
	
	virtual bool isDangerous(int colony) { return true; }
	virtual bool isEnemy(int colony) { return true; }
	virtual ~Grasshopper() {};
private:
	virtual void doSomethingHelperI(); //distinguishes unique grasshopper behavior
	virtual bool doSomethingHelperG() = 0; //Different grasshoppers act differently
	int moveTurns;
};

class babyGrasshopper : public Grasshopper
{
public:
	babyGrasshopper(int startX, int startY, StudentWorld* sw);
private:
	virtual bool doSomethingHelperG(); //distinguishes unique baby grasshopper behavior
};

class adultGrasshopper : public Grasshopper
{
public:
	adultGrasshopper(int startX, int startY, StudentWorld* sw, int curr_ticks);
	void poison() {}
	void bittenFor(int dmg);
private:
	virtual bool doSomethingHelperG(); //distinguishes unique adult grasshoppper behavior
};

class TriggerableActor : public Actor
{
public:
	TriggerableActor(int imageID, int startX, int startY, StudentWorld* sw);
	virtual bool isDangerous(int colony) { return true; }
	virtual ~TriggerableActor() {}
};

class Poison : public TriggerableActor
{
public:
	Poison(int startX, int startY, StudentWorld* sw);
	virtual void doSomething();
};

class Pool : public TriggerableActor
{
public:
	Pool(int startX, int startY, StudentWorld* sw);
	virtual void doSomething();
};

class Pheromone : public energyHolder
{
public:
	Pheromone(int imageID, int startX, int startY, int colony, StudentWorld* sw);
	void doSomething();
	bool isPheromone() { return true; }
	bool isMyPheromone(int colony) { return colony == colonyNumber(); }
};

class Anthill : public energyHolder
{
public:
	Anthill(int startX, int startY, int colony, Compiler* c, StudentWorld* sw);
	void doSomething();
	bool isMyAnthill(int colony) { return colony == colonyNumber(); }
private:
	Compiler* m_compiler;
};

class Ant : public Insect
{
public:
	Ant(int imageID, int startX, int startY, int colony, Compiler* c, StudentWorld* sw, int curr_ticks);
	bool isDangerous(int colony) { return colony != colonyNumber(); }
	bool isEnemy(int colony) { return colony != colonyNumber(); }
	void bittenFor(int dmg);
private:
	void doSomethingHelperI(); //distinguishes unique ant behavior
	bool checkCondition(Compiler::Condition cond); //Helper function for doSomethingHelperI
	int ic;
	int foodHeld;
	bool prevBitten;
	bool prevBlocked;
	int rand;
	Compiler* m_compiler;
};
#endif // ACTOR_H_

