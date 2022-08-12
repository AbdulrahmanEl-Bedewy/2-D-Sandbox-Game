#pragma once
#include <unordered_map>
#include <vector>
#include "Defs.h"
using namespace std;

// forward declaration
class Item;
class Manager;

struct Stack {
	int StackSize; // current stack size
	int stackPos; // item number in the inventory max = 50
	vector<Item*> items; // items in the stack

};

class Inventory
{
	int Size; // size of inventory
	int MaxStackSize; // max size of a stack (i think should be replace by a const)
	int SelectedPos;
	bool Expanded;
	Stack* ItemStacks[25];
	unordered_map<ItemClass, vector<int>> OpenStack;


	Stack* getStackFromPos(int p);
	int GetOpenStackPos();
	void RemoveStack(int pos);

public:
	Inventory();

	// adds items to inventory. True if added False if storage full
	bool Insert(Item* item);

	void DrawItems();

	void UseItem(Manager* pManager);

	void IncrementSelectedPos(int increment);
	void ToggleExpanded();

	~Inventory();
};