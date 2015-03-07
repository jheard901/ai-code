
#pragma once

class Puzzle;	//forward declaration

class List
{
private:
	typedef struct Node {
		Puzzle* nPuz;
		Node* next = nullptr;
		Node* prev = nullptr;
	};

	Node* ptr;		//the node that will be used to point to other nodes
	Node* head;		//the beginning of list
	Node* tail;		//the end of list

	int nLength;		//the length of the list
public:
	List();
	static List &Inst() { static List puzList; return puzList; }
	~List();

	void Insert(Puzzle* Puz);
	void Reset();		//deletes all items in list
	void Display();		//outputs values of list
};