#include "ScoreList.h"
#include <cassert>
#include <iostream>
using namespace std;

int main()
{ 
	ScoreList a;
	assert(a.size() == 0);
	assert(a.minimum() == ::numeric_limits<unsigned long>::max());
	assert(a.maximum() == ::numeric_limits<unsigned long>::max());
	assert(a.add(5));
	ScoreList b = a;
	assert(a.size() == 1);
	assert(!a.add(-1));
	assert(!a.add(101));
	a.add(4);
	a.add(6);
	a.add(3);
	a.add(7);
	a.add(2);
	a.add(8);
	a.add(1);
	a.add(100);
	a.add(0);
	a.add(0);
	a.add(9);
	a.add(100);
	a.add(7);
	assert(a.size() == 14);
	cout << b.size();
	assert(a.maximum() == 100);
	assert(a.minimum() == 0);
	assert(a.remove(7));
	assert(!a.remove(47));
	a.remove(0);
	a.remove(0);
	a.remove(100);
	a.remove(100);
	assert(a.size() == 9);
	assert(a.maximum()==9);
	assert(a.minimum()==1);

	ScoreList c;
	for (int i = 0; i < 200; i++)
	{
		c.add(1);
	}
	assert(!c.add(1));
}