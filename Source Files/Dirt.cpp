#include "../Header files/Dirt.h"
#include "../Header files/Manager.h"
#include "../Header files/Player.h"



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
	switch (itemstate)
	{
	case Onhand:
		DrawTextureEx(pUI->dirtTex, pos, rotation, 1.0, WHITE);
		break;
	case Placed:
		DrawTextureRec(pUI->dirtTex, Rectangle{ 0,0 , 16 , 16 }, pos, WHITE); //for tile option 2	//DrawTextureRec(pUI->dirtTex, Rectangle{ 0,/*64*/ 3, blockWidth , blockHeight }, pos, /*WHITE*/Color{ 215, 162, 125, 255 });
		break;
	case Mined:
		DrawTexturePro(pUI->dirtTex, Rectangle{ 0, 0, 16 , 16 }, Rectangle{ pos.x + 2.5f,pos.y + 2.5f, blockWidth - 5 , blockHeight - 5 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		break;
	case Picked:
		DrawTextureRec(pUI->dirtTex, Rectangle{ 0, 3, blockWidth  , blockHeight }, Invpos, Fade(BROWN,0.5));
		break;
	default:
		break;
	}
}

void Dirt::DrawText(Vector2 pos)
{
}

bool Dirt::UseItem(Manager* pMmanager)
{
	Vector2 MousePos = pMmanager->GetWorldXY(GetMousePosition());
	Vector2 PlayerPos = pMmanager->GetPlayer()->GetPos();
	PlayerPos.x += 12;
	PlayerPos.y += 32;
	//pMmanager->GetCoordinate(MousePos)
	
	if (!CheckCollisionPointCircle(MousePos, PlayerPos, 120))
		return false;

	return pMmanager->PlaceBlock(pMmanager->GetCoordinate(MousePos), this);
}
