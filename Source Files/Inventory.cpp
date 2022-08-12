#include "../Header files/Inventory.h"
#include "../Header files/Item.h"
#include "../Header files/Manager.h"


Inventory::Inventory()
{
	Size = 0;
	MaxStackSize = 10;
	SelectedPos = 0;
	Expanded = false;
	for (int i = 0; i < 25; i++) ItemStacks[i] = nullptr;
}

Stack* Inventory::getStackFromPos(int p)
{
	return ItemStacks[p];
}

int Inventory::GetOpenStackPos()
{
	for (int i = 0; i < 25; i++)
	{
		if (!ItemStacks[i])
			return i;
	}
	return -1;
}

void Inventory::RemoveStack(int pos)
{

	if (OpenStack.find(ItemStacks[pos]->items[0]->GetItemType()) != OpenStack.end()) {
		
		vector<int> &temp =  OpenStack.find(ItemStacks[pos]->items[0]->GetItemType())->second;
		//OpenStack.find(ItemStacks[pos]->items[0]->GetItemType())->second.erase(OpenStack.find(ItemStacks[pos]->items[0]->GetItemType())->second.begin());
			if (temp.size() == 1)
			{
				OpenStack.erase(ItemStacks[pos]->items[0]->GetItemType());
			}
			else
			{
				for (vector<int>::iterator i = temp.begin(); i != temp.end(); i++) {
					if (*i == pos)
					{
						temp.erase(i);
						break;
					}
				}
			}
	}

	delete ItemStacks[pos];
	ItemStacks[pos] = nullptr;
	Size--;
}


bool Inventory::Insert(Item* item)
{
	if ( Size == 25 && OpenStack.find(item->GetItemType()) == OpenStack.end()) // full storage
		return false;
	if (OpenStack.find(item->GetItemType()) == OpenStack.end()) { //  first time picking this item OR first un-filled stack
		Stack* stack = new Stack{ 0 };
		stack->StackSize = 1;
		stack->stackPos = GetOpenStackPos();
		stack->items.push_back(item);
		ItemStacks[stack->stackPos] = stack;
		OpenStack.insert({ item->GetItemType(), vector<int>{stack->stackPos} });
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
		ItemStacks[ OpenStack.find(item->GetItemType())->second[0]]->items.push_back(item);
		ItemStacks[OpenStack.find(item->GetItemType())->second[0]]->StackSize++;
		if (ItemStacks[OpenStack.find(item->GetItemType())->second[0]]->StackSize >= MaxStackSize) // stack is now full
		{
			OpenStack.find(item->GetItemType())->second.erase(OpenStack.find(item->GetItemType())->second.begin());
			if (OpenStack.find(item->GetItemType())->second.size()==0)
			{
				OpenStack.erase(item->GetItemType());
			}
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

		for (int i = 0; i < 25; i++)
		{
			if (ItemStacks[i]) {
				int y = 30;
				int itemspos = ItemStacks[i]->stackPos;
				if (itemspos < 10) {
					ItemStacks[i]->items[0]->DrawItem(0, Right, Picked, Vector2{ 10.0f + itemspos * 35 + itemspos * 5 + 17.5f - 8, 30 + 17.5 - 8 });
					DrawText(TextFormat("%d", ItemStacks[i]->items.size()), 10.0f + itemspos * 35 + itemspos * 5 + 30 - MeasureText(TextFormat("%d", MaxStackSize), 15), 50, 15, WHITE);
				}
			}
		}
		break;

	case 1:
		for (int i = 0; i < 50; i++)
		{

			if (i == SelectedPos)
				DrawRectangle(10 + i % 10 * 35 + i % 10 * 5, 30 + i / 10 * 40, 35, 35, Fade(GOLD, 0.5));
			else
				DrawRectangle(10 + i % 10 * 35 + i % 10 * 5, 30 + i / 10 * 40, 35, 35, Fade(BLUE, 0.3));


			if (i < 10) {
				DrawText(TextFormat("%d", i), 10.0f + i % 10 * 35 + i % 10 * 5, 30 + i/10 * 40, 9, WHITE);
			}

		}

		for (int i = 0; i < 25; i++)
		{
			if (ItemStacks[i]) {
				int y = 30;
				int itemspos = ItemStacks[i]->stackPos;

				ItemStacks[i]->items[0]->DrawItem(0, Right, Picked, Vector2{ 10.0f + itemspos % 10 * 35 + itemspos % 10 * 5 + 17.5f - 8, 30 + itemspos/10 *40 + 17.5f - 8 });
				DrawText(TextFormat("%d", ItemStacks[i]->items.size()), 10.0f + itemspos % 10 * 35 + itemspos % 10 * 5 + 30 - MeasureText(TextFormat("%d", MaxStackSize), 15), 50 + itemspos / 10 * 40, 15, WHITE);
			}
		}
		break;

	default:
		break;
	}
	

}

void Inventory::UseItem(Manager* pManager)
{
	if (SelectedPos > 25 || ItemStacks[SelectedPos] == nullptr)
		return;
	if (ItemStacks[SelectedPos]->items[0]->UseItem(pManager)) { //if used remove 1 from stack. if stack empty remove stack

		if (ItemStacks[SelectedPos]->items.size() == 1)
			RemoveStack(ItemStacks[SelectedPos]->stackPos);
		else {
			if (ItemStacks[SelectedPos]->items.size() == MaxStackSize) {
				if (OpenStack.find(ItemStacks[SelectedPos]->items[0]->GetItemType()) != OpenStack.end())
					OpenStack.find(ItemStacks[SelectedPos]->items[0]->GetItemType())->second.push_back(SelectedPos);
				else
					OpenStack.insert({ ItemStacks[SelectedPos]->items[0]->GetItemType(), vector<int>{SelectedPos} });

			}
			ItemStacks[SelectedPos]->items.erase(ItemStacks[SelectedPos]->items.begin());
			ItemStacks[SelectedPos]->StackSize--;
		}
	}
}

void Inventory::IncrementSelectedPos(int increment)
{
	SelectedPos = (SelectedPos + increment < 0) ? 9 : (SelectedPos + increment) % 10;
}

void Inventory::ToggleExpanded()
{
	Expanded = !Expanded;
}

Inventory::~Inventory()
{
	for (size_t i = 0; i < 25; i++)
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