#include "../Header files/Inventory.h"
#include "../Header files/Item.h"
#include "../Header files/Manager.h"


Inventory::Inventory()
{
	Size = 0;
	MaxStackSize = 10;
	SelectedPos = 0;
	Expanded = false;
	for (int i = 0; i < 5; i++) ItemStacks[i] = nullptr;
}

Stack* Inventory::getStackFromPos(int p)
{
	return ItemStacks[p];
}

int Inventory::GetOpenStackPos()
{
	for (int i = 0; i < 5; i++)
	{
		if (!ItemStacks[i])
			return i;
	}
	return -1;
}

void Inventory::RemoveStack(int pos)
{
	if (OpenStack.find(ItemStacks[pos]->items[0]->GetItemType()) != OpenStack.end())
		OpenStack.erase(ItemStacks[pos]->items[0]->GetItemType());

	delete ItemStacks[pos];
	ItemStacks[pos] = nullptr;
	Size--;
}


bool Inventory::Insert(Item* item)
{
	if ( Size == 5 && OpenStack.find(item->GetItemType()) == OpenStack.end()) // full storage
		return false;
	if (OpenStack.find(item->GetItemType()) == OpenStack.end()) { //  first time picking this item OR first un-filled stack
		Stack* stack = new Stack{ 0 };
		stack->StackSize = 1;
		stack->stackPos = GetOpenStackPos();
		stack->items.push_back(item);
		ItemStacks[stack->stackPos] = stack;
		OpenStack.insert({ item->GetItemType(), stack->stackPos });
		Size++;
		return true;
	}
	//else if (OpenStack.find(item->GetItemType())->second->StackSize == MaxStackSize) // alraedy full stack
	//{
	//	fullStacks.push_back(Items.find(item->GetItemType())->second);
	//	Items.erase(item->GetItemType());
	//	return Insert(item);
	//}
	else
	{
		ItemStacks[ OpenStack.find(item->GetItemType())->second]->items.push_back(item);
		ItemStacks[OpenStack.find(item->GetItemType())->second]->StackSize++;
		if (ItemStacks[OpenStack.find(item->GetItemType())->second]->StackSize >= MaxStackSize)
		{
			OpenStack.erase(item->GetItemType());
		}
		return true;
	}

}

void Inventory::DrawItems()
{
	switch (Expanded)
	{
	case 0:
		for (int i = 0; i < 10; i++)
		{
			if (i == SelectedPos)
				DrawRectangle(10 + i * 35 + i * 5, 30, 35, 35, Fade(GOLD, 0.5));
			else
				DrawRectangle(10 + i * 35 + i * 5, 30, 35, 35, Fade(BLUE, 0.3));
			DrawText(TextFormat("%d", i), 10.0f + i * 35 + i * 5, 30, 9, WHITE);

		}

		for (int i = 0; i < 5; i++)
		{
			if (ItemStacks[i]) {
				int y = 30;
				int itemspos = ItemStacks[i]->stackPos;

				ItemStacks[i]->items[0]->DrawItem(0, Right, Picked, Vector2{ 10.0f + itemspos * 35 + itemspos * 5 + 17.5f - 8, 30 + 17.5 - 8 });
				DrawText(TextFormat("%d", ItemStacks[i]->items.size()), 10.0f + itemspos * 35 + itemspos * 5 + 30 - MeasureText(TextFormat("%d", MaxStackSize), 15), 50, 15, WHITE);
			}
		}
		break;

	case 1:
		break;

	default:
		break;
	}
	

}

void Inventory::UseItem(Manager* pManager)
{
	if (SelectedPos > 5 || ItemStacks[SelectedPos] == nullptr)
		return;
	if (ItemStacks[SelectedPos]->items[0]->UseItem(pManager)) { //if used remove 1 from stack. if stack empty remove stack

		if (ItemStacks[SelectedPos]->items.size() == 1)
			RemoveStack(ItemStacks[SelectedPos]->stackPos);
		else {
			/*if (ItemStacks[SelectedPos]->items.size() == MaxStackSize)
				OpenStack.insert({ ItemStacks[SelectedPos]->items[0]->GetItemType(), ItemStacks[SelectedPos]->stackPos });*/
			ItemStacks[SelectedPos]->items.erase(ItemStacks[SelectedPos]->items.begin());
		}
	}
}

void Inventory::IncrementSelectedPos(int increment)
{
	SelectedPos = (SelectedPos + increment < 0) ? 9 : (SelectedPos + increment) % 10;
}

Inventory::~Inventory()
{
	for (size_t i = 0; i < 5; i++)
	{
		if (ItemStacks[i])
		{
			for (int j = 0; j < ItemStacks[i]->items.size(); j++)
			{
				delete ItemStacks[i]->items[j];
			}
			delete ItemStacks[i];
		}
	}
	OpenStack.clear();
	
}