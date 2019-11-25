#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	int init(); //set up world
	int move(); //run simulation
	void cleanUp(); //deletes all dynamically allocated objects
	bool canMove(int x, int y) const;

	void poison(int x, int y);
	void stun(int x, int y);
	void bite(int colony, int dmg, Actor* biter);
	
	bool isFoodPresent(int x, int y) const;
	bool isEnemyInsectPresent(int x, int y, int colony) const;
	bool isPheromonePresent(int x, int y) const;
	bool isAnthillPresent(int x, int y, int colony) const;
	bool detectDanger(int x, int y, int colony) const;

	int addFood(int x, int y, int num);
	void addAdultGrasshopper(int x, int y);
	void addAnt(int imageID, int x, int y, int colony, Compiler* compiler);
	void addPheromone(int imageID, int x, int y, int colony);

	void incrementScore(int colony);
	int getMaxAnt();
	std::string getDisplayText();

	~StudentWorld();
private:
	int m_ticks;
	int maxAnt;
	std::vector<Compiler*> compilers;
	std::vector<int> scores;
	std::vector<Actor*> m_actors[64][64];

};

#endif // STUDENTWORLD_H_
