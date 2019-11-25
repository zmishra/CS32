#include "Actor.h"
#include "StudentWorld.h"
using namespace std;

Actor::Actor(int imageID, int startX, int startY, int depth, StudentWorld* sw) : GraphObject(imageID, startX, startY, right, depth)
{
	m_world = sw;
}

Pebble::Pebble(int startX, int startY, StudentWorld* sw) : Actor(IID_ROCK, startX, startY, 1, sw)
{
}

energyHolder::energyHolder(int imageID, int startX, int startY, int depth, int startHP, StudentWorld* sw) : Actor(imageID, startX, startY, depth, sw), hp(startHP)
{
	m_colonyNumber = -1; //anything that is not an ant, anthill, or pheromone has a colony number of -1
} 

int energyHolder::changeHealthBy(int num) //return health subtracted
{
	int changedBy;
	if (-num > hp) //if trying to take away more health than the object has
	{
		changedBy = hp;
		hp = 0;
	}
	else
	{
		changedBy = -num;
		hp += num;
	}
	return changedBy;
}

Food::Food(int startX, int startY, int startHP, StudentWorld* sw) : energyHolder(IID_FOOD, startX, startY, 2, startHP, sw)
{
}

TriggerableActor::TriggerableActor(int imageID, int startX, int startY, StudentWorld* sw) : Actor(imageID, startX, startY, 2, sw)
{
}

Poison::Poison(int startX, int startY, StudentWorld* sw) : TriggerableActor(IID_POISON, startX, startY, sw)
{
}

void Poison::doSomething() //ask the world to poison some coordinate
{
	world()->poison(getX(), getY());
}

Pool::Pool(int startX, int startY, StudentWorld* sw) : TriggerableActor(IID_WATER_POOL, startX, startY, sw)
{
}

void Pool::doSomething() //ask the world to stun some coordinate
{
	world()->stun(getX(), getY());
}

Insect::Insect(int imageID, int startX, int startY, int depth, int startHP, StudentWorld* sw) : energyHolder(imageID, startX, startY, depth, startHP, sw)
{
	m_sleepTurns = 0;
	m_callCount = 0;
	m_isStunned = false;
}

void Insect::stun()
{
	if (!m_isStunned)
	{
		m_sleepTurns += 2;
		m_isStunned = true;
	}
}

void Insect::poison()
{
	if (!isDead())
	{
		changeHealthBy(-150);
		if (isDead())
			world()->addFood(getX(), getY(), 100);
	}
}

void Insect::bittenFor(int dmg)
{
	if (!isDead()) //redundant check, but it prevents extra food from being made
	{
		changeHealthBy(-dmg);
		if (isDead())
			world()->addFood(getX(), getY(), 100);
	}
}

bool Insect::moveForward() //return true if successfully moved, false if not
{
	int x, y;
	getForwardCoords(x, y);
	if (world()->canMove(x, y))
	{
		moveTo(x, y);
		return true;
	}
	else
		return false;
}

void Insect::getForwardCoords(int& x, int& y) //puts the coordinates of the square the insect is facing in x and y
{
	switch (getDirection())
	{
	case up:
		x = getX();
		y = getY() + 1;
		break;
	case right:
		x = getX() + 1;
		y = getY();
		break;
	case down:
		x = getX();
		y = getY() - 1;
		break;
	case left:
		x = getX() - 1;
		y = getY();
		break;
	}
}

void Insect::doSomething()
{
	changeHealthBy(-1);
	changeCallCountBy(1);
	if (isDead())
	{
		world()->addFood(getX(), getY(), 100);
		return;
	}
	if (sleepTurns() > 0)
	{
		changeSleepTurnsBy(-1);
		return;
	}
	doSomethingHelperI(); //Different for each insect type
}

Grasshopper::Grasshopper(int imageID, int startX, int startY, int startHP, StudentWorld* sw) : Insect(imageID, startX, startY, 1, startHP, sw)
{
	setDirection(Direction(randInt(1, 4)));
	moveTurns = randInt(2, 10);
}

babyGrasshopper::babyGrasshopper(int startX, int startY, StudentWorld* sw) : Grasshopper(IID_BABY_GRASSHOPPER, startX, startY, 500, sw)
{
}

bool babyGrasshopper::doSomethingHelperG() //what baby grasshoppers do differently from adults
{
	if (health() >= 1600)
	{
		changeHealthBy(-health());
		world()->addFood(getX(), getY(), 100);
		world()->addAdultGrasshopper(getX(), getY());
		return true;
	}
	else
		return false;
}

adultGrasshopper::adultGrasshopper(int startX, int startY, StudentWorld* sw, int curr_ticks) : Grasshopper(IID_ADULT_GRASSHOPPER, startX, startY, 1600, sw)
{
	changeCallCountBy(curr_ticks); //Prevents calling the grasshopper twice in the same tick
}

bool adultGrasshopper::doSomethingHelperG() //what adult grasshoppers do differently from babies
{
	if (randInt(1, 3) == 1) 
	{
		if (world()->isEnemyInsectPresent(getX(), getY(), -1))
		{
			world()->bite(-1, 50, this);
			changeSleepTurnsBy(2);
			return true;
		}
	}
	if (randInt(1, 10) == 1)
	{
		for (int i = 0; i < 100; i++)
		{
			int x = randInt(-10, 10); //uniformly distributed square
			int y = randInt(-10, 10);
			if (((x*x + y*y) <= 100) && ((x*x + y*y) != 0) && world()->canMove(getX() + x, getY() + y)) //point needs to be in a circle of radius 10 and the numbers can't both be 0
			{
				moveTo(getX() + x, getY() + y);
				changeSleepTurnsBy(2);
				switchStunState(false);
				return true;
			}
		}
	}
	return false;
}

void adultGrasshopper::bittenFor(int dmg)
{
	Insect::bittenFor(dmg); //same insect behavior
	if (!isDead() && randInt(1, 2) == 1) //plus retaliation
		world()->bite(-1, 50, this);
}

void Grasshopper::doSomethingHelperI() //what all grasshoppers do
{
	if (doSomethingHelperG())
		return;
	if (world()->isFoodPresent(getX(), getY()))
	{
		changeHealthBy(world()->addFood(getX(), getY(), -200)); //take health from food and add to grasshopper
		if (randInt(1, 2) == 1)
		{
			changeSleepTurnsBy(2);
			return;
		}
	}
	if (moveTurns == 0)
	{
		setDirection(Direction(randInt(1, 4)));
		moveTurns = randInt(2, 10);
	}

	if (moveForward())
	{
		moveTurns--;
		switchStunState(false);
	}
	else
		moveTurns = 0;

	changeSleepTurnsBy(2);
}

Pheromone::Pheromone(int imageID, int startX, int startY, int colony, StudentWorld* sw) : energyHolder(imageID, startX, startY, 2, 256, sw)
{
	changeColonyNumber(colony);
}

void Pheromone::doSomething()
{
	changeHealthBy(-1);
}

Anthill::Anthill(int startX, int startY, int colony, Compiler* c, StudentWorld* sw) : energyHolder(IID_ANT_HILL, startX, startY, 2, 8999, sw), m_compiler(c)
{
	changeColonyNumber(colony);
}

void Anthill::doSomething()
{
	changeHealthBy(-1);
	if (health() <= 0)
	{
		return;
	}
	if (world()->isFoodPresent(getX(), getY()))
	{
		changeHealthBy(world()->addFood(getX(), getY(), -10000)); //take health from food and add to anthill
		return;
	}
	if (health() >= 2000)
	{
		world()->addAnt(IID_ANT_TYPE0 + colonyNumber(), getX(), getY(), colonyNumber(), m_compiler);
		changeHealthBy(-1500);
		world()->incrementScore(colonyNumber());
	}

}

Ant::Ant(int imageID, int startX, int startY, int colony, Compiler* c, StudentWorld* sw, int curr_ticks) : Insect(imageID, startX, startY, 1, 1500, sw), m_compiler(c)
{
	changeColonyNumber(colony);
	setDirection(Direction(randInt(1, 4)));
	ic = 0;
	foodHeld = 0;
	rand = 0;
	prevBitten = false;
	prevBlocked = false;
	changeCallCountBy(curr_ticks);
}

void Ant::doSomethingHelperI() //what ants do differently from grasshoppers
{
	for (int i = 0; i < 10; i++)
	{
		Compiler::Command cmd;

		if (!m_compiler->getCommand(ic, cmd)) //if there's an error reading a command, kill the ant
		{
			changeHealthBy(-health());
			return;
		}

		switch (cmd.opcode)
		{
		case Compiler::moveForward:
			if (moveForward())
			{
				switchStunState(false);
				prevBlocked = false;
				prevBitten = false;
			}
			else
				prevBlocked = true;

			ic++;
			return;
		case Compiler::eatFood:
			if (foodHeld > 100)
			{
				foodHeld -= 100;
				changeHealthBy(100);
			}
			else
			{
				changeHealthBy(foodHeld);
				foodHeld = 0;
			}
			ic++;
			return;
		case Compiler::dropFood:
			world()->addFood(getX(), getY(), foodHeld);
			foodHeld = 0;
			ic++;
			return;
		case Compiler::bite:
			world()->bite(colonyNumber(), 15, this);
			ic++;
			return;
		case Compiler::pickupFood:
			if (world()->isFoodPresent(getX(), getY()))
			{
				if (1800 - foodHeld < 400)
					foodHeld += world()->addFood(getX(), getY(), foodHeld - 1800);
				else
					foodHeld += world()->addFood(getX(), getY(), -400);
			}
			ic++;
			return;
		case Compiler::emitPheromone:
			world()->addPheromone(IID_PHEROMONE_TYPE0 + colonyNumber(), getX(), getY(), colonyNumber());
			ic++;
			return;
		case Compiler::faceRandomDirection:
			setDirection(Direction(randInt(1, 4)));
			ic++;
			return;
		case Compiler::rotateClockwise:
			switch (getDirection())
			{
			case up:
				setDirection(right);
				break;
			case right:
				setDirection(down);
				break;
			case down:
				setDirection(left);
				break;
			case left:
				setDirection(up);
				break;
			}
			ic++;
			return;
		case Compiler::rotateCounterClockwise:
			switch (getDirection())
			{
			case up:
				setDirection(left);
				break;
			case right:
				setDirection(up);
				break;
			case down:
				setDirection(right);
				break;
			case left:
				setDirection(down);
				break;
			}
			ic++;
			return;
		case Compiler::generateRandomNumber:
			if (stoi(cmd.operand1) == 0)
				rand = 0;
			else
				rand = randInt(0, stoi(cmd.operand1) - 1);
			ic++;
			break;
		case Compiler::goto_command:
			ic = stoi(cmd.operand1);
			break;
		case Compiler::if_command:
			if (checkCondition(Compiler::Condition(stoi(cmd.operand1)))) //helper function used to keep things neat
				ic = stoi(cmd.operand2);
			else
				ic++;
			break;
		}
	}
}

bool Ant::checkCondition(Compiler::Condition cond)
{
	int x, y;
	switch (cond)
	{
	case Compiler::last_random_number_was_zero:
		return rand == 0;
	case Compiler::i_am_carrying_food:
		return foodHeld > 0;
	case Compiler::i_am_hungry:
		return health() <= 25;
	case Compiler::i_am_standing_with_an_enemy:
		return world()->isEnemyInsectPresent(getX(), getY(), colonyNumber());
	case Compiler::i_am_standing_on_food:
		return world()->isFoodPresent(getX(), getY());
	case Compiler::i_am_standing_on_my_anthill:
		return world()->isAnthillPresent(getX(), getY(), colonyNumber());
	case Compiler::i_smell_pheromone_in_front_of_me:
		getForwardCoords(x, y);
		return world()->isPheromonePresent(x, y);
	case Compiler::i_smell_danger_in_front_of_me:
		getForwardCoords(x, y);
		return world()->detectDanger(x, y, colonyNumber());
	case Compiler::i_was_bit:
		return prevBitten;
	case Compiler::i_was_blocked_from_moving:
		return prevBlocked;
	default:
		return false;
	}
}

void Ant::bittenFor(int dmg)
{
	Insect::bittenFor(dmg);
	prevBitten = true;
}