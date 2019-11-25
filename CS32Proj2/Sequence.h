#ifndef SEQUENCE_H_INCLUDED
#define SEQUENCE_H_INCLUDED

#include <string>

typedef std::string ItemType;

class Sequence
{
public:
	Sequence();
	bool empty() const;
	int size() const;
	bool insert(int pos, const ItemType& value);
	int insert(const ItemType& value);
	bool erase(int pos);
	int remove(const ItemType& value);
	bool get(int pos, ItemType& value) const;
	bool set(int pos, const ItemType& value);
	int find(const ItemType& value) const;
	void swap(Sequence& other);
	~Sequence();
	Sequence(const Sequence& other);
	Sequence& operator=(const Sequence& other);
private:
	struct Node
	{
		ItemType m_data;
		Node* m_next;
		Node* m_prev;
	};
	Node* head;
};

int subsequence(const Sequence& seq1, const Sequence& seq2);
void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);

#endif