#include "newSequence.h"
#include <iostream>

Sequence::Sequence(int maxSize)
{
	if (maxSize < 0)
	{
		std::cout << "Error: maxSize cannot be negative." << std::endl;
		exit(1);
	}
	m_sequence = new ItemType[maxSize];
	m_size = 0;
	m_maxSize = maxSize;
}

Sequence::~Sequence() 
{
	delete[] m_sequence;
}

Sequence::Sequence(const Sequence& other)
{
	m_sequence = new ItemType[other.m_maxSize];
	m_size = other.m_size;
	m_maxSize = other.m_maxSize;
	for (int i = 0; i < m_size; i++)
	{
		m_sequence[i] = other.m_sequence[i];
	}
}

Sequence& Sequence::operator=(const Sequence& other)
{
	Sequence tmp(other);
	swap(tmp);
	return *this;
}

bool Sequence::empty() const
{
	return (m_size == 0);
}

int Sequence::size() const
{
	return m_size;
}

bool Sequence::insert(int pos, const ItemType& value)
{
	if (0 <= pos && pos <= size() && size() < m_maxSize)
	{
		for (int i = size(); i > pos; i--)
		{
			m_sequence[i] = m_sequence[i - 1];
		}
		m_sequence[pos] = value;
		m_size++;
		return true;
	}
	else
		return false;
}

int Sequence::insert(const ItemType& value)
{
	int p = size();
	for (int i = 0; i < size(); i++)
	{
		if (value <= m_sequence[i])
		{
			p = i;
			break;
		}
	}
	if (insert(p, value))
		return p;
	else
		return -1;
}

bool Sequence::erase(int pos)
{
	if (0 <= pos && pos < size())
	{
		for (int i = pos; i < size() - 1; i++)
		{
			m_sequence[i] = m_sequence[i + 1];
		}
		m_size--;
		return true;
	}
	else
	{
		return false;
	}
}

int Sequence::remove(const ItemType& value)
{
	int count = 0;
	for (int i = 0; i < size(); i++)
	{
		if (m_sequence[i] == value)
		{
			erase(i);
			i--;
			count++;
		}
	}
	return count;
}

bool Sequence::get(int pos, ItemType& value) const
{
	if (0 <= pos && pos < size())
	{
		value = m_sequence[pos];
		return true;
	}
	else
		return false;
}

bool Sequence::set(int pos, const ItemType& value)
{
	if (0 <= pos && pos < size())
	{
		m_sequence[pos] = value;
		return true;
	}
	else
		return false;
}

int Sequence::find(const ItemType& value) const
{
	for (int i = 0; i < size(); i++)
	{
		if (m_sequence[i] == value)
		{
			return i;
		}
	}
	return -1;
}

void Sequence::swap(Sequence& other)
{
	std::swap(m_maxSize, other.m_maxSize);
	std::swap(m_size, other.m_size);
	std::swap(m_sequence, other.m_sequence);
}
