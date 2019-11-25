#include "Sequence.h"
#include <iostream>

Sequence::Sequence()
{
	head = nullptr; //an empty sequence points to the null pointer
}

bool Sequence::empty() const
{
	return head == nullptr; //if head points to the null pointer, the sequence is empty
}

int Sequence::size() const
{
	int count = 0;
	Node* p;
	for (p = head; p != nullptr; p = p->m_next) //iterates through each node until hitting the null pointer
	{
		count++;
	}
	return count;
}

bool Sequence::insert(int pos, const ItemType& value)
{
	if (0 <= pos && pos <= size()) //checks if value can be placed at pos
	{
		Node* newNode = new Node;
		newNode->m_data = value;
		if (size() == 0) //handles inserting to an empty sequence
		{
			head = newNode;
			newNode->m_next = nullptr;
			newNode->m_prev = nullptr;
		}
		else if (pos == 0) //handles inserting at the beginning of a sequence
		{
			newNode->m_next = head;
			newNode->m_prev = nullptr;
			head->m_prev = newNode;
			head = newNode;
		}
		else if (pos == size()) //handles inserting at the end of a sequence
		{
			Node* p;
			for (p = head; p->m_next != nullptr; p = p->m_next);
			newNode->m_next = nullptr;
			newNode->m_prev = p;
			p->m_next = newNode;
			
		}
		else //handles the general case
		{
			Node* p;
			int count = 0;
			for (p = head; p != nullptr && count != pos; p = p->m_next)
				count++;
			newNode->m_next = p;
			newNode->m_prev = p->m_prev;
			p->m_prev->m_next = newNode;
			p->m_prev = newNode;
		}
		return true;
	}
	else
		return false;
}

int Sequence::insert(const ItemType& value)
{
	int pos = size();
	int count = 0;
	Node* p;
	for (p = head; p != nullptr; p = p->m_next) //finds the position where value should go
	{
		if (value <= p->m_data)
		{
			pos = count;
			break;
		}
		count++;
	}
	if (insert(pos, value))
		return pos;
	else
		return -1; //should never happen
}

bool Sequence::erase(int pos)
{
	if (0 <= pos && pos < size()) //checks if pos is somewhere that can be erased
	{
		if (size() == 1) //handles erasing when size is one
		{
			delete head;
			head = nullptr;
		}
		else if (pos == 0) //handles erasing node at start of sequence
		{
			head = head->m_next;
			delete head->m_prev;
			head->m_prev = nullptr;
		}
		else if (pos == size()-1) //handles erasing node at end of sequence
		{
			Node* p;
			for (p = head; p->m_next != nullptr; p = p->m_next);
			p->m_prev->m_next = nullptr;
			delete p;
		}
		else //handles erasing in the general case
		{
			Node* p;
			int count = 0;
			for (p = head; p != nullptr && count != pos; p = p->m_next)
				count++;
			p->m_prev->m_next = p->m_next;
			p->m_next->m_prev = p->m_prev;
			delete p;
		}
		return true;
	}
	else
		return false;
}

int Sequence::remove(const ItemType& value)
{
	Node* p;
	int pos = 0;
	int count = 0; //keeps track of number of times erase is called
	for (p = head; p != nullptr; p = p->m_next)
	{
		if (p->m_data == value)
		{
			erase(pos);
			count++;
			p = head; //resets the loop to iterate from the beginning of the sequence again
			if (p == nullptr)
				return count;
			pos = 0;
		}
		pos++;
	}
	return count;
}

bool Sequence::get(int pos, ItemType& value) const
{
	if (0 <= pos && pos < size()) //checks that pos is a valid location
	{
		Node* p;
		int count = 0;
		for (p = head; p != nullptr && count != pos; p = p->m_next) //iterates up to pos
			count++;
		value = p->m_data;
		return true;
	}
	else
		return false;
}
bool Sequence::set(int pos, const ItemType& value)
{
	if (0 <= pos && pos < size()) //checks that pos is a valid location
	{
		Node* p;
		int count = 0;
		for (p = head; p != nullptr && count != pos; p = p->m_next) //iterates up to pos
			count++;
		p->m_data = value;
		return true;
	}
	else
		return false;
}

int Sequence::find(const ItemType& value) const
{
	Node* p;
	int count = 0;
	for (p = head; p != nullptr; p = p->m_next) //iterates through the sequence until the value is found
	{
		if (p->m_data == value)
			return count;
		count++;
	}
	return -1;
}

void Sequence::swap(Sequence& other)
{
	std::swap(head, other.head); //head pointers swap what they are pointing to
}

Sequence::~Sequence()
{
	int initialSize = size();
	for (int i = 0; i < initialSize; i++) //deletes size() nodes, lleaving head pointing to the nullptr, with no nodes left being allocated
	{
		erase(0);
	}
}

Sequence::Sequence(const Sequence& other)
{
	head = nullptr; //intitializes the sequence to be empty
	for (int i = 0; i < other.size(); i++) //iterates through other and inserts each item 
	{
		ItemType x;
		other.get(i, x);
		insert(i, x);
	}
}

Sequence& Sequence::operator=(const Sequence& other)
{
	Sequence tmp(other); //copy-swap idiom
	swap(tmp);
	return *this;
}

int subsequence(const Sequence& seq1, const Sequence& seq2)
{
	Sequence tmp(seq1); //create a copy of seq1 to allow modification
	if (seq2.empty() || seq1.size() < seq2.size())
	{
		return -1;
	}

	int deleteCount = 0; //keeps track of number of elements deleted from the copy of seq1
	
	ItemType x;
	bool complete; //keeps track of whether the subsequence was found or not; true by default
	
	for (;;)
	{
		complete = true; //resets complete to true;
		seq2.get(0, x);
		int loc = tmp.find(x); //find the first element of seq2 in seq1
		if (loc == -1)
			return -1;
		else
		{
			for (int i = 0; i < seq2.size(); i++) //check if consecutive members of seq1 match seq2
			{
				ItemType a;
				ItemType b;
				seq2.get(i, a);
				if(!tmp.get(loc + i, b)) //handles a subsequence that would go off the end of seq1
					return -1;
				if (a != b) //if there is not a match, remove the starting point in order to look for another, add to the delete count, and set complete to false
				{
					tmp.erase(loc);
					deleteCount++;
					complete = false;
					break;
				}
			}
			if (complete) //complete will only stay true if there are only matches
				return loc + deleteCount;
		}
	}
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
	Sequence a(seq1); //create copies of seq1 and seq2 in order to retain access to the values
	Sequence b(seq2);
	int initialResultSize = result.size();
	for (int i = 0; i < initialResultSize; i++) //reduce result to having head point to the null pointer (empty sequence)
	{	
		result.erase(0);
	}

	int small; //keep track of how many elements seq1 and seq2 both have
	bool aBigger; //keep track of which sequence is longer
	if (a.size() < b.size())
	{
		small = a.size();
		aBigger = false;
	}
	else
	{
		small = b.size();
		aBigger = true;
	}
	
	ItemType x;
	ItemType y;
	for (int i = 0; i < small; i++) //iterate through each sequence until one ends
	{
		a.get(i, x);
		b.get(i, y);
		result.insert(i*2, y); //insert from seq2 first to get the correct order
		result.insert(i*2, x);
	}

	if (aBigger) //append the rest of the bigger sequence 
	{
		for (int i = small; i < a.size(); i++)
		{
			a.get(i, x);
			
			result.insert(result.size(), x); //always insert at the end of the sequence
		}
	}
	else
	{
		for (int i = small; i < b.size(); i++)
		{
			b.get(i, y);
			result.insert(result.size(), y);
		}
	}
}