#include "..\Header files\Bow.h"
#include "../Header files/Manager.h"
#include"../Header files/Arrow.h"
#include "raymath.h"
using namespace std;

Bow::Bow(UIInfo* p, Vector2 position)
{

	pUI = p;
	pos = position;

	Damage = 20;
	FireSpeed = 2;
	FiringTimer = 0;
	itemClass = RangedWeapon;
	frame = 0;
	animate = false;
	Yspeed = 0;
}

void Bow::UpdateItem(Manager* pManager)
{
	if (itemstate == Onhand)
	{
		if (animate && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
			animate = false;
			timer = 0;
			frame = 0;
			FiringTimer = 0;
		}
		else if (animate) {
			timer += min(GetFrameTime(), 0.05f);
			FiringTimer += min(GetFrameTime(), 0.05f);
			if (timer >= 0.125)
			{
				timer = 0;
				frame = ++frame % 8;
			}
			if (FiringTimer >= 1.0f / FireSpeed)
			{
				FiringTimer = 0;
			}
			if ((rotation > 90 && rotation < 270) && pManager->GetPlayer()->GetOrientation() == Left) {
				pManager->GetPlayer()->SetOrientation(Right);
			}
			else if ((rotation < 90 || rotation > 270) && pManager->GetPlayer()->GetOrientation() == Right)
			{
				pManager->GetPlayer()->SetOrientation(Left);
			}
		}
		else
		{
			if (pManager->GetPlayer()->GetOrientation() == Left)
			{
				rotation = -45 + 360;
			}
			else
			{
				rotation = 225;
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

void Bow::DrawItem(int rotation, PlayerOrientaion orientation, ItemState State, Vector2 Invpos)
{
	switch (itemstate)
	{
	case Onhand:
		if (orientation == Left)
		{
			DrawTexturePro(pUI->Bow, Rectangle{ 192.0f + frame * 64, 64, 64 , 64 }, Rectangle{ pos.x + 10 ,pos.y + 45, 50 , 50 }, Vector2{ 17.1875, 30.078125 }, this->rotation, WHITE);
		}
		else
		{
			DrawTexturePro(pUI->Bow, Rectangle{ 192.0f + frame * 64, 64, 64 , 64 }, Rectangle{ pos.x + 27 ,pos.y + 45, 50 , 50 }, Vector2{ 17.1875, 30.078125 }, this->rotation, WHITE);
		}
		DrawTexturePro(pUI->Bow, Rectangle{ 256, 192, 64  , 64 }, Rectangle{ Invpos.x - 15 ,Invpos.y - 10 , 30  , 30 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		break;
	case Placed:
		
		break;
	case Mined:
		DrawTexturePro(pUI->Bow, Rectangle{ 256, 192, 64  , 64 }, Rectangle{ pos.x  ,pos.y , 30  , 30 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		break;
	case Picked:
		DrawTexturePro(pUI->Bow, Rectangle{ 256, 192, 64  , 64 }, Rectangle{ Invpos.x - 15 ,Invpos.y - 10 , 30  , 30 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		break;
	default:
		break;
	}
}

void Bow::DrawName(Vector2 pos)
{
	DrawText("Bow", pos.x - MeasureText(pUI->dirtName.c_str(), 20) / 2, 5, 20, WHITE);
}

bool Bow::UseItem(Manager* pManager)
{
	animate = true;
	itemstate = Onhand;
	rotation = Vector2Angle(GetMousePosition(), Vector2{ pos.x + 17.1875f,pos.y + 30.078125f });
	if (FiringTimer == 0) {
		Arrow* arrow = new Arrow(pUI, pos);
		pManager->AddFiredAmmo(arrow);
		arrow->UseItem(pManager);
	}
	return false;
}
