#include "../Header files/Item.h"



//void Item::setTexture(Texture2D t)
//{
//}
//
//void Item::setName(string n)
//{
//}
//
//void Item::setDescription(string d)
//{
//}
//
//void Item::setDamage(int d)
//{
//}


void Item::setPos(Vector2 p)
{
	pos = p;
}

void Item::setPos(float x, float y)
{
	pos = Vector2{ x,y };
}

void Item::DrawText(Vector2 pos)
{
}

Item::~Item()
{
}
