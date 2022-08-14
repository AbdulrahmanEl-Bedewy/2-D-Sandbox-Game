#include "..\Header files\Sword.h"
#include "../Header files/Manager.h"


Sword::Sword(UIInfo* p, Vector2 position)
{

	pUI = p;
	pos = position;

	Damage = 20;
	FireSpeed = 2;
	itemClass = MeleeWeaopon;
	frame = 0;
	animate = false;
	Yspeed = 0;
	itemstate = Picked;
}

void Sword::UpdateItem(Manager* pManager)
{
	if (animate && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		animate = false;
		timer = 0;
		frame = 0;
	}
	else if (animate) {
		timer += GetFrameTime();
		if (timer >= 0.116667)
		{
			timer = 0;
			frame = ++frame % 6;
		}
	}
	pos = pManager->GetScreenXY(pManager->GetPlayer()->GetPos());

	Vector2 minPoint = pManager->GetMinPoint();
	Vector2 maxPoint = pManager->GetMaxPoint();
	vector<vector<Item*>>::const_iterator dirtblocks = pManager->GetDirtBlocks();



	if (itemstate == Mined) {
		for (int i = ((int)(pos.y - minPoint.y) / blockHeight) - 1; i < ((int)(pos.y - minPoint.y) / blockHeight) + 6; i++)
		{
			bool br = false;
			for (int j = ((int)(pos.x - minPoint.x) / blockHeight) - 4; j < (int)(pos.x - minPoint.x) / blockHeight + 4; j++) // 
			{
				if (i > 0 && j > 0 && i < WorldHeight && j < WorldWidth && dirtblocks[i][j]) {

					Vector2 dirtpos = dirtblocks[i][j]->GetPos();
					if (CheckCollisionRecs(Rectangle{ pos.x ,pos.y + Yspeed * GetFrameTime(),64,64 }, Rectangle{ dirtpos.x, dirtpos.y , blockWidth, blockHeight })) {
						Yspeed = 0;
						pos.y = dirtpos.y - 63;
						br = true;
						break;
					}
					else
					{
						Yspeed = (Yspeed > 650) ? 650 : Yspeed + 700 * GetFrameTime();
					}
				}
			}
			if (br)
				break;
		}
	}
	pos.y += Yspeed * GetFrameTime();
}

void Sword::DrawItem(int rotation, PlayerOrientaion orientation, ItemState State, Vector2 Invpos)
{
	switch (itemstate)
	{
	case Onhand:
		if (orientation == Left)
		{
			DrawTexturePro(pUI->Sword, Rectangle{frame * 192.0f, 192, 192 , 192 }, Rectangle{ pos.x - 77.0f,pos.y - 67, 192 , 192 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		}
		else
		{
			DrawTexturePro(pUI->Sword, Rectangle{ frame * 192.0f, 576, 192 , 192 }, Rectangle{ pos.x + -80,pos.y + -67, 192 , 192 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		}
		DrawTexturePro(pUI->Sword, Rectangle{ 576 , 192, 192  , 192 }, Rectangle{ Invpos.x - 15 ,Invpos.y - 10 , 30  , 30 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		break;
	case Placed:
		//DrawTextureRec(pUI->dirtTex, Rectangle{ 0,0 , 16 , 16 }, pos, WHITE); //for tile option 2	//DrawTextureRec(pUI->dirtTex, Rectangle{ 0,/*64*/ 3, blockWidth , blockHeight }, pos, /*WHITE*/Color{ 215, 162, 125, 255 });
		break;
	case Mined:
		DrawTexturePro(pUI->Sword, Rectangle{ 256, 192, 64  , 64 }, Rectangle{ pos.x ,pos.y + 2.5f, 50  , 50 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		break;
	case Picked:
		DrawTexturePro(pUI->Sword, Rectangle{ 645, 287, 60  , 50 }, Rectangle{ Invpos.x ,Invpos.y , blockWidth +5 , blockHeight +5 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		break;
	default:
		break;
	}
}

void Sword::DrawName(Vector2 pos)
{
	DrawText("Long Sword", pos.x - MeasureText(pUI->dirtName.c_str(), 20) / 2, 5, 20, WHITE);
}

bool Sword::UseItem(Manager* pMmanager)
{
	animate = true;
	itemstate = Onhand;
	return false;
}
