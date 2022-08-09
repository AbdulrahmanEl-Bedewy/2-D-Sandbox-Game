#include "../Header files/Dirt.h"

Dirt::Dirt(UIInfo* p, ItemClass IC, Vector2 position)
{
	pUI = p;
	itemClass = IC;
	pos = position;
	itemstate = Placed;
}

// to-do later
void Dirt::DrawItem(int rotation, PlayerOrientaion orientation, ItemState State, Vector2 Invpos)
{
	Texture2D tex = pUI->dirtTex;
	switch (itemstate)
	{
	case Onhand:
		DrawTextureEx(tex, pos, rotation, 1.0, WHITE);
		break;
	case Placed:
		DrawTextureRec(tex, Rectangle{ 0,/*64*/ 3, pUI->blockWidth , pUI->blockHeight }, pos, WHITE);
		break;
	case Mined:
		DrawTexturePro(tex, Rectangle{ 0,/*64*/ 3, pUI->blockWidth , pUI->blockHeight }, Rectangle{ pos.x,pos.y, pUI->blockWidth - 3 , pUI->blockHeight -3 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		break;
	case Picked:
		DrawTextureRec(tex, Rectangle{ 0, 3, pUI->blockWidth  , pUI->blockHeight }, Invpos, WHITE);
		break;
	default:
		break;
	}
}

void Dirt::DrawText(Vector2 pos)
{
}