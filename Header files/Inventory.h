#pragma once
#include <unordered_map>
#include <vector>
#include "Defs.h"


// forward declaration
class Item;
class Manager;

struct Stack {
	int StackSize; // current stack size
	int stackPos; // item number in the inventory max = 50
	Item* items[MaxStackSize]; // items in the stack
	~Stack() {
		for (int i = 0; i < StackSize; i++)
		{
			delete items[i];
		}
	}
};

class Inventory
{
	int Size; // size of inventory
	//int MaxStackSize; // max size of a stack (i think should be replace by a const)
	int SelectedPos;
	bool Expanded;
	Stack* ItemStacks[StorageSize];
	std::unordered_map<ItemClass, std::vector<int>> OpenStack;


	Stack* getStackFromPos(int p);
	int GetOpenStackPos();
	void RemoveStack(int pos);

public:
	Inventory(UIInfo* p);

	// adds items to inventory. True if added False if storage full
	bool Insert(Item* item);

	void DrawItems(PlayerOrientaion Orientation);

	void UseItem(Manager* pManager);

	void IncrementSelectedPos(int increment);
	void ToggleExpanded();

	void UpdateSelected(Manager* pManager);

	~Inventory();
};