#include "Sequence.h"
#include <iostream>
#include <cassert>
using namespace std;

#define TEST_WITH_LONG
#ifdef TEST_WITH_LONG

void test()
{
	Sequence s;
	assert(s.insert(0, 10));
	assert(s.insert(0, 20));
	assert(s.size() == 2);
	ItemType x = 999;
	assert(s.get(0, x) && x == 20);
	assert(s.get(1, x) && x == 10);
}

int main()
{
	Sequence s;
	assert(s.empty());
	assert(s.find(42) == -1);
	s.insert(42);
	assert(s.size() == 1 && s.find(42) == 0);
	test();
	cout << "Passed all tests" << endl;
}


#else
void test1()
{
	Sequence s;
	assert(s.insert(0, "lavash"));
	assert(s.insert(0, "tortilla"));
	assert(s.size() == 2);
	ItemType x = "injera";
	assert(s.get(0, x) && x == "tortilla");
	assert(s.get(1, x) && x == "lavash");
}

int main()
{
	Sequence s;
	s.insert("e");
	s.insert("b");
	s.insert("c");
	s.insert("b");
	s.insert("a");
	assert(s.remove("b") == 2);
	assert(s.size() == 3);
	string x;
	assert(s.get(0, x) && x == "a");
	assert(s.get(1, x) && x == "c");
	assert(s.get(2, x) && x == "e");
	assert(s.set(2, "z") && s.get(2, x) && x == "z");
	
	Sequence s1;
	s1.insert(0, "paratha");
	s1.insert(0, "focaccia");
	Sequence s2;
	s2.insert(0, "roti");
	s1.swap(s2);
	assert(s1.size() == 1 && s1.find("roti") == 0 && s2.size() == 2 &&
		s2.find("focaccia") == 0 && s2.find("paratha") == 1);

	Sequence s3;
	s3.insert(0, "dosa");
	s3.insert(1, "pita");
	s3.insert(2, "");
	s3.insert(3, "matzo");
	assert(s3.find("") == 2);
	s3.remove("dosa");
	assert(s3.size() == 3 && s3.find("pita") == 0 && s3.find("") == 1 &&
		s3.find("matzo") == 2);

	Sequence s4;
	assert(s4.empty());
	assert(s4.find("laobing") == -1);
	s4.insert("laobing");
	assert(s4.size() == 1 && s4.find("laobing") == 0);

	test1();
	cout << "Passed all tests" << endl;

}
#endif
