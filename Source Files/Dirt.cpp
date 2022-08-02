#include "../Header files/Dirt.h"

Dirt::Dirt(UIInfo* p, ItemClass IC)
{
	pUI = p;
	itemClass = IC;
}

// to-do later
void Dirt::DrawItem(Vector2 pos, int rotation, PlayerOrientaion orientation, ItemState State)
{
	Texture2D tex = pUI->dirtTex;
	switch (State)
	{
	case Onhand:
		DrawTextureEx(tex , pos, rotation, 1.0, WHITE);
		break;
	case Placed:
		DrawTextureRec(tex, Rectangle{ 0,64,32,32 }, pos,  WHITE);
		break;
	case Mined:
		break;
	default:
		break;
	}
}

void Dirt::DrawText(Vector2 pos)
{
}
