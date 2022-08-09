#pragma once
#include <unordered_map>
#include <vector>
#include "Defs.h"
using namespace std;

// forward declaration
class Item;

struct Stack {
	int StackSize; // current stack size
	int stackPos; // item number in the inventory max = 50
	vector<Item*> items; // items in the stack

};

class Inventory
{
	int Size; // size of inventory
	int MaxStackSize; // max size of a stack (i think should be replace by a const)
	vector<Stack*> fullStacks; 
	unordered_map<ItemClass,Stack*> Items;
	

public:
	Inventory();

	// adds items to inventory. True if added False if storage full
	bool Insert(Item* item);
	void DrawItems();

	~Inventory();
};

