#include "StudentWorld.h"
#include "Field.h"
#include <string>
#include <sstream> 
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(string assetDir) : GameWorld(assetDir)
{
	m_ticks = 0;
	maxAnt = 0; //0 comes first when calculating maxAnt, so it is the default
}

int StudentWorld::init()
{
	Field f;
	string fieldFile = getFieldFilename();
	string errorField;
	if (f.loadField(fieldFile, errorField) != Field::LoadResult::load_success)
	{
		setError(fieldFile + " " + errorField);
		return GWSTATUS_LEVEL_ERROR;
	}
	
	int ah0x, ah0y, ah1x, ah1y, ah2x, ah2y, ah3x, ah3y; //keep track of anthill positions until needed

	for (int i = 0; i < 64; i++) //put everything except for anthills into the 2d array
		for (int j = 0; j < 64; j++)
		{
			Field::FieldItem item = f.getContentsOf(i, j);
			if (item == Field::FieldItem::rock)
			{
				Pebble* peb = new Pebble(i, j, this);
				m_actors[i][j].push_back(peb);
			}
			else if (item == Field::FieldItem::grasshopper)
			{
				babyGrasshopper* bg = new babyGrasshopper(i, j, this);
				m_actors[i][j].push_back(bg);
			}
			else if (item == Field::FieldItem::poison)
			{
				Poison* poi = new Poison(i, j, this);
				m_actors[i][j].push_back(poi);
			}
			else if (item == Field::FieldItem::food)
			{
				Food* f = new Food(i, j, 6000, this);
				m_actors[i][j].push_back(f);
			}
			else if (item == Field::FieldItem::water)
			{
				Pool* w = new Pool(i, j, this);
				m_actors[i][j].push_back(w);
			}
			else if (item == Field::FieldItem::anthill0)
			{
				ah0x = i; ah0y = j;
			}
			else if (item == Field::FieldItem::anthill1)
			{
				ah1x = i; ah1y = j;
			}
			else if (item == Field::FieldItem::anthill2)
			{
				ah2x = i; ah2y = j;
			}
			else if (item == Field::FieldItem::anthill3)
			{
				ah3x = i; ah3y = j;
			}
		}

	Compiler *compilerForEntrant0 = nullptr, *compilerForEntrant1 = nullptr,
		*compilerForEntrant2 = nullptr, *compilerForEntrant3 = nullptr;
	Anthill *ah0, *ah1, *ah2, *ah3;
	bool ah1Made = false, ah2Made = false, ah3Made = false;
	vector<string> fileNames = getFilenamesOfAntPrograms();
	string error;

	switch (fileNames.size())
	{
	case 4:
		compilerForEntrant3 = new Compiler;
		if (!compilerForEntrant3->compile(fileNames[3], error)) //if there's an error the compiler pointed to needs to deleted to prevent memory leak
		{
			delete compilerForEntrant3;
			setError(fileNames[3] + " " + error);
			return GWSTATUS_LEVEL_ERROR;
		}
		ah3Made = true;
		compilers.push_back(compilerForEntrant3);
		ah3 = new Anthill(ah3x, ah3y, 3, compilerForEntrant3, this);
		m_actors[ah3x][ah3y].push_back(ah3);
		scores.push_back(0);
	case 3:
		compilerForEntrant2 = new Compiler;
		if (!compilerForEntrant2->compile(fileNames[2], error))
		{
			if (ah3Made)
				delete compilerForEntrant3; //if there's an error, any previously allocated compilers also need to be deleted
			delete compilerForEntrant2;
			setError(fileNames[2] + " " + error);
			return GWSTATUS_LEVEL_ERROR;
		}
		ah2Made = true;
		compilers.push_back(compilerForEntrant2);
		ah2 = new Anthill(ah2x, ah2y, 2, compilerForEntrant2, this);
		m_actors[ah2x][ah2y].push_back(ah2);
		scores.push_back(0);
	case 2:
		compilerForEntrant1 = new Compiler;
		if (!compilerForEntrant1->compile(fileNames[1], error))
		{
			if (ah3Made)
				delete compilerForEntrant3;
			if (ah2Made)
				delete compilerForEntrant2;
			delete compilerForEntrant1;
			setError(fileNames[1] + " " + error);
			return GWSTATUS_LEVEL_ERROR;
		}
		ah1Made = true;
		compilers.push_back(compilerForEntrant1);
		ah1 = new Anthill(ah1x, ah1y, 1, compilerForEntrant1, this);
		m_actors[ah1x][ah1y].push_back(ah1);
		scores.push_back(0);
	case 1:
		compilerForEntrant0 = new Compiler;
		if (!compilerForEntrant0->compile(fileNames[0], error))
		{
			if (ah3Made)
				delete compilerForEntrant3;
			if (ah2Made)
				delete compilerForEntrant2;
			if (ah1Made)
				delete compilerForEntrant1;
			delete compilerForEntrant0;
			setError(fileNames[0] + " " + error);
			return GWSTATUS_LEVEL_ERROR;
		}
		compilers.push_back(compilerForEntrant0);
		ah0 = new Anthill(ah0x, ah0y, 0, compilerForEntrant0, this);
		m_actors[ah0x][ah0y].push_back(ah0);
		scores.push_back(0);
	default:
		break;
	}

	m_ticks = 0; //set elapsed ticks to 0 and begin!
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	m_ticks++;


	for (int i = 0; i < 64; i++) //Traverse through the 2d array of vectors and ask each insect actor to doSomething(); if they move, move their pointer
		for (int j = 0; j < 64; j++)
			for (int k = m_actors[i][j].size() - 1; k >= 0; k--)
			{
				if (m_actors[i][j][k]->isInsect() && m_actors[i][j][k]->callCount() < m_ticks && !(m_actors[i][j][k]->isDead()))
					m_actors[i][j][k]->doSomething();

				if (i != m_actors[i][j][k]->getX() || j != m_actors[i][j][k]->getY())
				{
					m_actors[m_actors[i][j][k]->getX()][m_actors[i][j][k]->getY()].push_back(m_actors[i][j][k]);
					m_actors[i][j].erase(m_actors[i][j].begin() + k);
				}
			}

	for (int i = 0; i < 64; i++) //Traverse through the 2d array of vectors and ask each non-insect actor to doSomething(); ensures poison acts thrice on grasshoppers, etc.
		for (int j = 0; j < 64; j++)
			for (int k = m_actors[i][j].size() - 1; k >= 0; k--)
				if (!(m_actors[i][j][k]->isInsect()) && m_actors[i][j][k]->callCount() < m_ticks && !(m_actors[i][j][k]->isDead()))
					m_actors[i][j][k]->doSomething();

	for (int i = 0; i < 64; i++) //delete the dead stuff
		for (int j = 0; j < 64; j++)
			for (int k = m_actors[i][j].size() - 1; k >= 0; k--)
			{
				if (m_actors[i][j][k]->isDead())
				{
					delete m_actors[i][j][k];
					m_actors[i][j].erase(m_actors[i][j].begin() + k);
				}
			}

	setGameStatText(getDisplayText());

	if (m_ticks == 2000)
	{
		if (scores.size() > 0 && scores[maxAnt] >= 6) //determine if the ant in the lead has met the conditions to win
		{
			setWinner(compilers[compilers.size() - 1 - maxAnt]->getColonyName()); //compilers are stored in reverse order
			return GWSTATUS_PLAYER_WON;
		}
		else
			return GWSTATUS_NO_WINNER;
	}
	else
		return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for (int i = 0; i < 64; i++) //delete everything in the 2d array of vectors
		for (int j = 0; j < 64; j++)
		{
			int size = m_actors[i][j].size();
			for (int k = 0; k < size; k++)
			{
				delete m_actors[i][j][0];
				m_actors[i][j].erase(m_actors[i][j].begin());
			}
		}

	int s = compilers.size();
	for (int i = 0; i < s; i++) //delete the stored compilers
	{
		delete compilers[0];
		compilers.erase(compilers.begin());
	}
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}

bool StudentWorld::canMove(int x, int y) const
{
	if (x > 63 || x < 0 || y > 63 || y < 0) //cannot be out of bounds
		return false;
	for (unsigned int i = 0; i < m_actors[x][y].size(); i++)
	{
		if (!(m_actors[x][y][i]->canMoveThrough()))
			return false;
	}
	return true;
}

void StudentWorld::poison(int x, int y)
{
	for (unsigned int i = 0; i < m_actors[x][y].size(); i++)
	{
		m_actors[x][y][i]->poison();
	}
}

void StudentWorld::stun(int x, int y)
{
	for (unsigned int i = 0; i < m_actors[x][y].size(); i++)
	{
		m_actors[x][y][i]->stun();
	}
}

void StudentWorld::bite(int colony, int dmg, Actor* biter)
{
	for (unsigned int i = 0; i < m_actors[biter->getX()][biter->getY()].size(); i++) //to bite, there must be an alive enemy (that is not the biter)
		if (m_actors[biter->getX()][biter->getY()][i]->isEnemy(colony) && m_actors[biter->getX()][biter->getY()][i] != biter && !(m_actors[biter->getX()][biter->getY()][i]->isDead()))
		{
			Insect* bitee = dynamic_cast<Insect*>(m_actors[biter->getX()][biter->getY()][i]); //the actor will have to be an Insect at this point
			bitee->bittenFor(dmg);
			return;
		}
}

int StudentWorld::addFood(int x, int y, int num) //add food also handles removing food
{
	for (unsigned int i = 0; i < m_actors[x][y].size(); i++)
		if (m_actors[x][y][i]->isFood())
		{
			Food* f = dynamic_cast<Food*>(m_actors[x][y][i]); //it is known the actor pointer points to a food object, so dynamic_cast is okay
			return f->changeHealthBy(num);
		}
	if (num > 0) //should not create a dead food object; precautionary measure
	{
		Food* nf = new Food(x, y, num, this);
		m_actors[x][y].push_back(nf);
	}
	return num;
}

bool StudentWorld::isFoodPresent(int x, int y) const
{
	for (unsigned int i = 0; i < m_actors[x][y].size(); i++)
		if (m_actors[x][y][i]->isFood() && !(m_actors[x][y][i]->isDead()))
			return true;
	return false;
}

void StudentWorld::addAdultGrasshopper(int x, int y)
{
	adultGrasshopper* ag = new adultGrasshopper(x, y, this, m_ticks);
	m_actors[x][y].push_back(ag);
}

bool StudentWorld::isEnemyInsectPresent(int x, int y, int colony) const
{
	int count = 0;
	for (unsigned int i = 0; i < m_actors[x][y].size(); i++)
		if (m_actors[x][y][i]->isEnemy(colony) && !(m_actors[x][y][i]->isDead()))
			count++;
	if (colony == -1) //Grasshoppers are "colony" -1, so need check that more than itself was found
		return (count > 1);
	else
		return (count > 0);
}

void StudentWorld::addAnt(int imageID, int x, int y, int colony, Compiler* c)
{
	Ant* a = new Ant(imageID, x, y, colony, c, this, m_ticks);
	m_actors[x][y].push_back(a);
}

void StudentWorld::incrementScore(int colony)
{
	scores[colony]++;
}

int StudentWorld::getMaxAnt()
{
	for (int i = 0; i < scores.size(); i++)
	{
		if (scores[i] > scores[maxAnt])
			maxAnt = i;
	}
	return maxAnt;
}

std::string StudentWorld::getDisplayText()
{
	ostringstream oss;
	oss << "Ticks:" << setw(5) << 2000 - m_ticks; //displays ticks remaining, as in the example executable
	if (scores.size() > 0)
	{
		int winningAnt = -1;
		int leadAnt = getMaxAnt();
		if (scores[leadAnt] >= 6) //if the leading ant meets the winning condition, store its colony number
			winningAnt = leadAnt;

		oss << " - ";

		for (int i = 0; i < scores.size(); i++)
		{
			if (i > 0)
				oss << "  ";
			oss << compilers[compilers.size()-1-i]->getColonyName();
			if (winningAnt == i)
				oss << "*";
			oss << ": ";
			oss.fill('0');
			oss << setw(2) << scores[i];
			oss << " ants"; //adds ants after the number as int he example executable
		}
	}
	return oss.str();
}

bool StudentWorld::isPheromonePresent(int x, int y) const
{
	{
		for (unsigned int i = 0; i < m_actors[x][y].size(); i++)
			if (m_actors[x][y][i]->isPheromone() && !(m_actors[x][y][i]->isDead()))
				return true;
		return false;
	}
}

void StudentWorld::addPheromone(int imageID, int x, int y, int colony)
{
	for (unsigned int i = 0; i < m_actors[x][y].size(); i++)
		if (!(m_actors[x][y][i]->isDead()) && m_actors[x][y][i]->isMyPheromone(colony)) //Necessary to check for the ants' own pheromone to add to
		{
			Pheromone* p = dynamic_cast<Pheromone*>(m_actors[x][y][i]); //The object is guaranteed to be a Pheromone
			p->changeHealthBy(256);
			if (p->health() > 768)
				p->changeHealthBy(768 - p->health());
			return;
		}
	Pheromone* p = new Pheromone(imageID, x, y, colony, this);
	m_actors[x][y].push_back(p);
}

bool StudentWorld::isAnthillPresent(int x, int y, int colony) const
{
	for (unsigned int i = 0; i < m_actors[x][y].size(); i++)
		if (!(m_actors[x][y][i]->isDead()) && m_actors[x][y][i]->isMyAnthill(colony))
			return true;
	return false;
}

bool StudentWorld::detectDanger(int x, int y, int colony) const
{
	for (unsigned int i = 0; i < m_actors[x][y].size(); i++)
		if (!(m_actors[x][y][i]->isDead()) && m_actors[x][y][i]->isDangerous(colony))
			return true;
	return false;
}