

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
		temp->nPuz->CloneFromPuzzle(Puz);

		head = temp;
		tail = temp;

		nLength++;
	}
	//if not empty, then lets check if there is only a head
	else if (head == tail)
	{
		Node* temp = new Node;
		temp->nPuz = new Puzzle;
		temp->nPuz->CloneFromPuzzle(Puz);

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
		temp->nPuz->CloneFromPuzzle(Puz);

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

	std::cout << "\n";	//removed displaying a title "Data Values:"

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

//use numbers 0-3 for choosing an item
void List::Display(int nItem)
{
	std::cout << "\n";
	GoTo(nItem);
	ptr->nPuz->Display();
}

//use numbers 0-3 for choosing an element
void List::GoTo(int element)
{
	//start from head
	ptr = head;

	if (element > -1 && element < nLength)
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

void List::DeleteLast()
{
	if (tail == nullptr) { return; }

	if (nLength < 2)
	{
		delete tail;
		head = nullptr;
		tail = nullptr;
		nLength--;
	}
	else if (nLength > 1)
	{
		ptr = tail->prev;
		delete tail;
		ptr->next = nullptr;
		tail = ptr;
		nLength--;
	}
	else { }
}

void List::DeleteLast(int nElements)
{
	if (tail == nullptr) { return; }
	if (nLength < nElements) { std::cout << "\ninvalid. not enough values in list to delete\n"; return; }

	for (int i = 0; i < nElements; i++)
	{
		if (nLength < 2)
		{
			delete tail;
			head = nullptr;
			tail = nullptr;
			nLength--;
		}
		else if (nLength > 1)
		{
			ptr = tail->prev;
			delete tail;
			ptr->next = nullptr;
			tail = ptr;
			nLength--;
		}
		else {}
	}
}

//copies all puzzles (i.e. board states) from a source list
void List::Copy(List* srcList)
{
	if (srcList == NULL) { return; }

	if (srcList->head != nullptr)
	{
		srcList->ptr = srcList->head;

		while (srcList->ptr->next != nullptr)
		{
			Insert(srcList->ptr->nPuz);
			srcList->ptr = srcList->ptr->next;
		}
		Insert(srcList->ptr->nPuz);
	}
	else
	{
		//std::cout << "\nerror. attempting to copy empty list\n";
	}
}

int List::Length()
{
	return nLength;
}

