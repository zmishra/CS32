// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.
#ifndef MYMAP_INCLUDED
#define MYMAP_INCLUDED

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap();
	~MyMap();
	void clear();
	int size() const { return m_size; }
	void associate(const KeyType& key, const ValueType& value);

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
	struct Node
	{
		KeyType m_key;
		ValueType m_value;
		Node* m_left;
		Node* m_right;
	};
	Node* m_root;
	int m_size;
	void clearHelper(Node* root);
	bool insert(Node*& root, Node*& newNode);
	const ValueType* findHelper(const Node* root, const KeyType& key) const;
};

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap()
{
	m_root = nullptr;
	m_size = 0;
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	Node* newNode = new Node; //new node initialized and values given
	newNode->m_key = key;
	newNode->m_value = value;
	newNode->m_left = nullptr;
	newNode->m_right = nullptr;
	if (insert(m_root, newNode))
		m_size++;
	else
		delete newNode; //if the node was not put into the tree, it needs to be deleted
}

template<typename KeyType, typename ValueType>
bool MyMap<KeyType, ValueType>::insert(Node*& root, Node*& newNode) //root passed in by reference to allow modification
{
	if (root == nullptr) //end of tree, add node
	{
		root = newNode;
		return true;
	}
	else if (newNode->m_key > root->m_key)
		return insert(root->m_right, newNode);
	else if (newNode->m_key < root->m_key)
		return insert(root->m_left, newNode);
	else //keys are equal
	{
		root->m_value = newNode->m_value; //replace value
		return false;
	}
}

template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const
{
	return findHelper(m_root, key);
}

template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::findHelper(const Node* root, const KeyType& key) const
{
	if (root == nullptr) //key not found
		return nullptr;
	else if (key > root->m_key)
		return findHelper(root->m_right, key);
	else if (key < root->m_key)
		return findHelper(root->m_left, key);
	else //keys are equal
		return &(root->m_value);
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear()
{
	if (m_root != nullptr) //if the tree is not empty
		clearHelper(m_root);
	m_root = nullptr;
	m_size = 0;
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clearHelper(Node* root)
{
	if (root->m_left != nullptr) //if the left tree is not empty
		clearHelper(root->m_left);
	if (root->m_right != nullptr) //if the right tree is not empty
		clearHelper(root->m_right);
	delete root; //delete the parent after the children have been taken care of
}

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap()
{
	clear();
}

#endif //MYMAP_INCLUDED