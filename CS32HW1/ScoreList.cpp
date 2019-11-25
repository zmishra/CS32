#include "ScoreList.h"
#include "Sequence.h"

ScoreList::ScoreList()
{}

bool ScoreList::add(unsigned long score)
{
	if (score >= 0 && score <= 100 && m_grades.size() != DEFAULT_MAX_ITEMS)
	{
		m_grades.insert(score);
		return true;
	}
	else
		return false;
}

bool ScoreList::remove(unsigned long score)
{
	int p = m_grades.find(score);
	return m_grades.erase(p);
}

int ScoreList::size() const
{
	return m_grades.size();
}

unsigned long ScoreList::minimum() const
{
	if (m_grades.empty())
	{
		return NO_SCORE;
	}
	else
	{
		unsigned long min;
		m_grades.get(0, min);
		return min;
	}
}

unsigned long ScoreList::maximum() const
{
	if (m_grades.empty())
	{
		return NO_SCORE;
	}
	else
	{
		unsigned long max;
		m_grades.get(m_grades.size()-1, max);
		return max;
	}
}