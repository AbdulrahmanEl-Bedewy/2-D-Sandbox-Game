#include "..\Header files\Sword.h"
#include "../Header files/Manager.h"
using namespace std;


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
	if (itemstate == Onhand) {
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
	}
	else if (itemstate == Mined) {
		Vector2 minPoint = pManager->GetMinPoint();
		Vector2 maxPoint = pManager->GetMaxPoint();
		std::vector<std::vector<Item*>>::const_iterator dirtblocks = pManager->GetDirtBlocks();
		bool br = false;
		for (int i = ((int)(pos.y - minPoint.y) / blockHeight) - 1; i < ((int)(pos.y - minPoint.y) / blockHeight) + 6; i++)
		{
			bool br = false;
			for (int j = ((int)(pos.x - minPoint.x) / blockHeight) - 4; j < (int)(pos.x - minPoint.x) / blockHeight + 4; j++) // 
			{
				if (i > 0 && j > 0 && i < WorldHeight && j < WorldWidth && dirtblocks[i][j]) {

					Vector2 dirtpos = dirtblocks[i][j]->GetPos();
					if (dirtpos.y > pos.y && CheckCollisionRecs(Rectangle{ pos.x ,pos.y + Yspeed * min(GetFrameTime(), 0.05f) ,30,30 }, Rectangle{ dirtpos.x, dirtpos.y , blockWidth, blockHeight })) {

						Vector2 tempPos1 = pManager->GetCoordinate(pos);
						Vector2 tempPos2 = pManager->GetCoordinate(pos.x, dirtpos.y - blockHeight + 1);// coordinate of block position just above ground
						if (tempPos1.y < tempPos2.y) { // if block changes coordinate while falling
							pManager->RemovePickable(tempPos1.y, tempPos1.x, this);	 // move its position in the pickables list    
							pManager->AddPickable(tempPos2.y, tempPos2.x, this);  
						}

						Yspeed = 0;
						pos.y = dirtpos.y - 30 + 1;
						br = true;
						break;
					}
				}
			}
			if (br)
				break;

		}
		if (!br)
			Yspeed = (Yspeed + 100 * GetFrameTime() > 150) ? 150 : Yspeed + 100 * min(GetFrameTime(), 0.05f);

		Vector2 tempPos1 = pManager->GetCoordinate(pos);
		Vector2 tempPos2 = pManager->GetCoordinate(pos.x, pos.y + Yspeed * min(GetFrameTime(), 0.05f));
		if (tempPos1.y < tempPos2.y) { // if block changes coordinate while falling
			pManager->RemovePickable(tempPos1.y, tempPos1.x, this);	 // move its position in the pickables list    
			pManager->AddPickable(tempPos2.y, tempPos2.x, this);  
		}
		pos.y += Yspeed * min(GetFrameTime(), 0.05f);
	}
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
		DrawTexturePro(pUI->Sword, Rectangle{ 645, 287, 60  , 50 }, Rectangle{ Invpos.x ,Invpos.y , blockWidth + 5 , blockHeight + 5 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		break;
	case Placed:
		break;
	case Mined:
		DrawTexturePro(pUI->Sword, Rectangle{ 645, 287, 60  , 50 }, Rectangle{ pos.x ,pos.y ,30 , 30 }, Vector2{ 0, 0 }, 0.0f, WHITE);
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
