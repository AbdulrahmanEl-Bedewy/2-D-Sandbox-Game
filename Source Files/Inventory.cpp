#include "../Header files/Inventory.h"
#include "../Header files/Item.h"

Inventory::Inventory()
{
	Size = 5;
	MaxStackSize = 10;
	SelectedPos = 0;
}

bool Inventory::Insert(Item* item)
{
	if(fullStacks.size() == Size) // full storage
		return false;
	if (Items.find(item->GetItemType()) == Items.end()) { //  first time picking this item OR first un-filled stack
		Stack* s = new Stack{ 0 };
		s->StackSize = 1;
		s->stackPos = Items.size() + fullStacks.size() + 1;
		s->items.push_back(item);
		Items.insert({ item->GetItemType(), s });
		return true;
	}
	else if(Items.find(item->GetItemType())->second->StackSize == MaxStackSize) // alraedy full stack
	{
		fullStacks.push_back(Items.find(item->GetItemType())->second);
		Items.erase(item->GetItemType());
		return Insert(item);
	}
	else
	{
		Items.find(item->GetItemType())->second->items.push_back(item);
		Items.find(item->GetItemType())->second->StackSize++;
		return true;
	}

}

void Inventory::DrawItems()
{
	for (int i = 0; i < 10; i++)
	{
		if (i== SelectedPos)
			DrawRectangle(10 + i * 35 + i * 5, 30, 35, 35, Fade(GOLD, 0.5));
		else
			DrawRectangle(10 + i * 35 + i * 5, 30, 35, 35, Fade(BLUE, 0.3));
		DrawText(TextFormat("%d", i), 10.0f + i * 35 + i * 5 , 30, 9, WHITE);

	}

	for (size_t i = 0; i < fullStacks.size(); i++)
	{
		int y = 30;
		int itemspos = fullStacks[i]->stackPos -1;

		fullStacks[i]->items[0]->DrawItem(0, Right, Picked, Vector2{ 10.0f + itemspos * 35 + itemspos * 5 + 17.5f - 8, 30 + 17.5 - 8 });
		DrawText(TextFormat("%d", MaxStackSize), 10.0f + itemspos * 35 + itemspos * 5 + 30 - MeasureText(TextFormat("%d", MaxStackSize), 15), 50, 15, WHITE);
	}

	
	for (auto i = Items.begin(); i != Items.end(); i++)
	{
		int y = 30;
		int itemspos = i->second->stackPos -1;

		i->second->items[0]->DrawItem(0, Right, Picked, Vector2{ 10.0f + itemspos * 35 + itemspos * 5 + 17.5f - 8, 30 + 17.5 - 8 });
		DrawText(TextFormat("%d",(int)i->second->items.size()), 10.0f + itemspos * 35 + itemspos * 5 + 30 - MeasureText(TextFormat("%d", (int)i->second->items.size()),15), 50, 15, WHITE);
	}
}

void Inventory::UseItem(int Stackpos)
{
}

void Inventory::IncrementSelectedPos(int increment)
{
	SelectedPos = (SelectedPos + increment < 0)? 9 : (SelectedPos + increment) % 10;
}

Inventory::~Inventory()
{
	for (size_t i = 0; i < fullStacks.size(); i++)
	{
		for (int j = 0; j < fullStacks[i]->items.size(); j++)
		{
			delete fullStacks[i]->items[j];
		}
		delete fullStacks[i];
	}

	for (auto i = Items.begin(); i != Items.end(); i++)
	{
		for (int j = 0; j < i->second->items.size(); j++)
		{
			delete i->second->items[j];
		}
		delete i->second;
	}
}
