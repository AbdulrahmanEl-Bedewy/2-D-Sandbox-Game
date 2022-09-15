#include "../Header files/Inventory.h"
#include "../Header files/Item.h"
#include "../Header files/Manager.h"
#include "../Header files/Bow.h"
using namespace std;


Inventory::Inventory(UIInfo* p /* for testing*/)
{
	Size = 0;
	//MaxStackSize = 10;
	SelectedPos = 0;
	Expanded = false;
	for (int i = 0; i < StorageSize; i++) ItemStacks[i] = nullptr;
}

//Stack* Inventory::getStackFromPos(int p)
//{
//	return ItemStacks[p];
//}

int Inventory::GetOpenStackPos() // finds the index of the first empty place in the storage
{
	for (int i = 0; i < StorageSize; i++)
	{
		if (!ItemStacks[i])
			return i;
	}
	return -1;
}

void Inventory::RemoveStack(int pos) // remove a stack after all the items in the stack are used up. might need changes for deleting items in trash
{

	if (OpenStack.find(ItemStacks[pos]->items[0]->GetItemType()) != OpenStack.end()) {
		
		vector<int> &temp =  OpenStack.find(ItemStacks[pos]->items[0]->GetItemType())->second;

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


bool Inventory::Insert(Item* item) // add item to inventort. true ---> there is storage and its added, false ---> otherwise
{
	if ( Size == StorageSize && OpenStack.find(item->GetItemType()) == OpenStack.end()) // full storage
		return false;
	if (OpenStack.find(item->GetItemType()) == OpenStack.end()) { //  first time picking this item OR first un-filled stack
		Stack* stack = new Stack{ 0 };
		stack->items[0] = item;
		stack->StackSize = 1;
		stack->stackPos = GetOpenStackPos();
		ItemStacks[stack->stackPos] = stack;
		OpenStack.insert({ item->GetItemType(), vector<int>{stack->stackPos} });
		Size++;
		return true;
	}
	else
	{
		Stack*& stack = ItemStacks[OpenStack.find(item->GetItemType())->second[0]];
		stack->items[stack->StackSize] = item;
		stack->StackSize++;
		if ( stack->StackSize >= MaxStackSize) // stack is now full
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

void Inventory::DrawItems(PlayerOrientaion Orientation)
{
	switch (Expanded)
	{
	case 0:
		for (int i = 0; i < 10; i++)
		{
			if (i == SelectedPos)
				DrawRectangle(10 + i * 35 + i * 5, 30, 35, 35, Fade(GOLD, 0.5));
			else
				DrawRectangle(10 + i * 35 + i * 5, 30, 35, 35, Fade(BLUE, 0.9));
			DrawText(TextFormat("%d", i), 10.0f + i * 35 + i * 5, 30, 9, WHITE);

		}

		for (int i = 0; i < StorageSize; i++)
		{
			if (ItemStacks[i]) {
				int y = 30;
				int itemspos = ItemStacks[i]->stackPos;
				if (itemspos < 10) {
					ItemStacks[i]->items[0]->DrawItem(0, Orientation, Picked, Vector2{ 10.0f + itemspos * 35 + itemspos * 5 + 17.5f - 8, 30 + 17.5 - 8 });
					if (i == SelectedPos)
						ItemStacks[i]->items[0]->DrawName(Vector2{ 205,15 });
					DrawText(TextFormat("%d", ItemStacks[i]->StackSize), 10.0f + itemspos * 35 + itemspos * 5 + 30 - MeasureText(TextFormat("%d", MaxStackSize), 15), 50, 15, WHITE);
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
				DrawRectangle(10 + i % 10 * 35 + i % 10 * 5, 30 + i / 10 * 40, 35, 35, Fade(BLUE, 0.9));


			if (i < 10) {
				DrawText(TextFormat("%d", i), 10.0f + i % 10 * 35 + i % 10 * 5, 30 + i/10 * 40, 9, WHITE);
			}

		}

		for (int i = 0; i < StorageSize; i++)
		{
			if (ItemStacks[i]) {
				int y = 30;
				int itemspos = ItemStacks[i]->stackPos;

				ItemStacks[i]->items[0]->DrawItem(0, Orientation, Picked, Vector2{ 10.0f + itemspos % 10 * 35 + itemspos % 10 * 5 + 17.5f - 8, 30 + itemspos/10 *40 + 17.5f - 8 });
				DrawText(TextFormat("%d", ItemStacks[i]->StackSize), 10.0f + itemspos % 10 * 35 + itemspos % 10 * 5 + 30 - MeasureText(TextFormat("%d", MaxStackSize), 15), 50 + itemspos / 10 * 40, 15, WHITE);
			}
		}
		break;

	default:
		break;
	}
	

}

void Inventory::UseItem(Manager* pManager)
{
	if (SelectedPos > StorageSize || ItemStacks[SelectedPos] == nullptr)
		return;
	if ((ItemStacks[SelectedPos]->items[0])->UseItem(pManager)) { //if used remove 1 from stack. if stack empty remove stack

		if (ItemStacks[SelectedPos]->StackSize == 1) {
			ItemStacks[SelectedPos]->StackSize--;
			RemoveStack(ItemStacks[SelectedPos]->stackPos);
		}
		else {
			if (ItemStacks[SelectedPos]->StackSize == MaxStackSize) {
				if (OpenStack.find(ItemStacks[SelectedPos]->items[0]->GetItemType()) != OpenStack.end())
					OpenStack.find(ItemStacks[SelectedPos]->items[0]->GetItemType())->second.push_back(SelectedPos);
				else
					OpenStack.insert({ ItemStacks[SelectedPos]->items[0]->GetItemType(), vector<int>{SelectedPos} });

			}
			
			ItemStacks[SelectedPos]->items[0] = ItemStacks[SelectedPos]->items[  ItemStacks[SelectedPos]->StackSize-1  ];
			ItemStacks[SelectedPos]->StackSize--;
			ItemStacks[SelectedPos]->items[0]->setState(Onhand);
		}
	}
}

void Inventory::IncrementSelectedPos(int increment) // used to update the state of the items in the hotbar when scrolling through
{
	if (ItemStacks[SelectedPos])
	{
		ItemStacks[SelectedPos]->items[0]->setState(Picked);
	}
	
	SelectedPos = (SelectedPos + increment < 0) ? 9 : (SelectedPos + increment) % 10;
	if (ItemStacks[SelectedPos])
	{
		ItemStacks[SelectedPos]->items[0]->setState(Onhand);
	}
}

void Inventory::ToggleExpanded()
{
	Expanded = !Expanded;
}

void Inventory::UpdateSelected(Manager* pManager)
{
	if(ItemStacks[SelectedPos])
	ItemStacks[SelectedPos]->items[0]->UpdateItem(pManager);

	if (IsKeyPressed(KEY_T)) { // throw item/stack

		Vector2 playerPos = pManager->GetPlayer()->GetPos();
		playerPos.y += 32;

		if (pManager->GetPlayer()->GetOrientation() == Left)
			playerPos.x -= blockWidth;
		else
			playerPos.x += 3*blockWidth;

		Vector2 playerCoordinate = pManager->GetCoordinate(playerPos);

		for (int i = 0; i < ItemStacks[SelectedPos]->StackSize; i++)
		{
			ItemStacks[SelectedPos]->items[i]->setState(Mined);
			ItemStacks[SelectedPos]->items[i]->setPos(playerPos);
			pManager->AddPickable(playerCoordinate.y, playerCoordinate.x,ItemStacks[SelectedPos]->items[i]);
		}

		ItemStacks[SelectedPos]->StackSize = 0;

		RemoveStack(SelectedPos);
	}
		
}

Inventory::~Inventory()
{
	for (size_t i = 0; i < StorageSize; i++)
	{
		if (ItemStacks[i])
		{
			delete ItemStacks[i];
		}
	}
	OpenStack.clear();
	
}