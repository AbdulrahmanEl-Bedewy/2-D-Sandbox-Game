#include "../Header files/Item.h"






void Item::setPos(Vector2 p)
{
	pos = p;
}

void Item::setPos(float x, float y)
{
	pos = Vector2{ x,y };
}

void Item::setState(ItemState state)
{
	itemstate = state;
}

Vector2 Item::GetPos()
{
	return pos;
}

ItemClass Item::GetItemType()
{
	return itemClass;
}

ItemState Item::GetItemState()
{
	return itemstate;
}


Item::~Item()
{
}
