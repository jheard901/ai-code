
#include "list.h"
#include "puzzle.h"
#include <iostream>

List::List()
{
	ptr = nullptr;
	head = nullptr;
	tail = nullptr;
	nLength = 0;
}

void List::Insert(Puzzle* Puz)
{
	//check if the list is empty first
	if (head == nullptr)
	{
		Node* temp = new Node;
		temp->nPuz = new Puzzle;
		temp->nPuz->CloneFromBoard(Puz->GetBoard());

		head = temp;
		tail = temp;

		nLength++;
	}
	//if not empty, then lets check if there is only a head
	else if (head == tail)
	{
		Node* temp = new Node;
		temp->nPuz = new Puzzle;
		temp->nPuz->CloneFromBoard(Puz->GetBoard());

		head->next = temp;
		temp->prev = head;
		tail = temp;

		nLength++;
	}
	//we assume there is a minimum of two nodes if the prior condition was not true
	else
	{
		Node* temp = new Node;
		temp->nPuz = new Puzzle;
		temp->nPuz->CloneFromBoard(Puz->GetBoard());

		temp->prev = tail;
		tail->next = temp;
		tail = temp;

		nLength++;
	}
}

void List::Reset()
{
	if (head == nullptr) { return; }

	while (head != nullptr)
	{
		if (head == tail)
		{
			delete tail->nPuz;
			delete tail;
			head = nullptr;
			tail = nullptr;
			nLength--;
		}
		else
		{
			ptr = tail->prev;
			delete tail->nPuz;
			delete tail;
			nLength--;

			tail = ptr;
		}
	}
}

void List::Display()
{
	//start from head, then output each node's Data value until making it to the end
	ptr = head;

	std::cout << "Data Values:\n";

	if (ptr == nullptr) { return; }		//prevents access violation error

	while (ptr->next != nullptr)
	{
		ptr->nPuz->Display();
		ptr = ptr->next;
		std::cout << "\n";
		//output this immediately since tail->next is NULL
		if (ptr == tail)
		{
			ptr->nPuz->Display();
		}
	}
}

//use numbers 0-3 for choosing an element
void List::GoTo(int element)
{
	//start from head
	ptr = head;

	if (!element > nLength)
	{
		for (int n = 0; n < element; n++)
		{
			ptr = ptr->next;
		}
	}
	else
	{
		std::cout << "\nerror - element position is larger than list";
	}
}

