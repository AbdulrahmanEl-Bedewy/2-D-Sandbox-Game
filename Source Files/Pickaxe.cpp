#include "../Header files/Manager.h"
#include"../Header files/Pickaxe.h"
#include "raymath.h"
using namespace std;

Pickaxe::Pickaxe(UIInfo* p, Vector2 position)
{

	pUI = p;
	pos = position;

	Damage = 20;
	SwingSpeed = 200;
	itemClass = tool;
	Mining = false;
	Yspeed = 0;
	itemstate = Picked;
}

void Pickaxe::UpdateItem(Manager* pManager)
{

	if (itemstate == Onhand)
	{
		if (Mining && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
			Mining = false;

		}
		else if (Mining) {

			// Animating the Rotation
			rotation += (pManager->GetPlayer()->GetOrientation() == Left) ? -SwingSpeed * min(GetFrameTime(), 0.05f) : SwingSpeed * min(GetFrameTime(), 0.05f);
			if (rotation >= 90 && pManager->GetPlayer()->GetOrientation() == Right) {
				rotation = -45;
			}
			else if (rotation <= 180 && pManager->GetPlayer()->GetOrientation() == Left)
			{
				rotation = 315;
			}

			float tempRotation = Vector2Angle(GetMousePosition(), Vector2{ pos.x + 17.1875f,pos.y + 30.078125f });

			if ((tempRotation > 90 && tempRotation < 270) && pManager->GetPlayer()->GetOrientation() == Left) {
				pManager->GetPlayer()->SetOrientation(Right);
			}
			else if ((tempRotation < 90 || tempRotation > 270) && pManager->GetPlayer()->GetOrientation() == Right)
			{
				pManager->GetPlayer()->SetOrientation(Left);
			}

			//Mining Blocks
			Vector2 MousePos = pManager->GetWorldXY(GetMousePosition());
			Vector2 PlayerPos = pManager->GetPlayer()->GetPos();
			PlayerPos.x += 24;
			PlayerPos.y += 38;
			if (CheckCollisionPointCircle(MousePos, PlayerPos, 80)) {
				Vector2 coordinate = pManager->GetCoordinateFromScreen(GetMousePosition());
				if (pManager->IsBlock(coordinate))
				{
					pManager->AddPickable(coordinate.y, coordinate.x, pManager->GetBlock(coordinate));
					pManager->RemoveBlock(coordinate.y, coordinate.x);
				}
			}
		}
		else
		{
			if (pManager->GetPlayer()->GetOrientation() == Left)
			{
				rotation = 270;
			}
			else
			{
				rotation = 0;
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
			pos.y += Yspeed * min(GetFrameTime(), 0.05f);
		}

	}
}

void Pickaxe::DrawItem(int rotation, PlayerOrientaion orientation, ItemState State, Vector2 Invpos)
{
	switch (itemstate)
	{
	case Onhand:
		
		if (orientation == Left)
		{
			DrawTexturePro(pUI->Pickaxe, Rectangle{ 0.0f, 0.0f, 32 , 32 }, Rectangle{ pos.x + 10 ,pos.y + 45, 45 , 45 }, Vector2{ 22.5, 22.5 }, this->rotation, WHITE);
		}
		else
		{
			DrawTexturePro(pUI->Pickaxe, Rectangle{ 0.0f, 0.0f, 32 , 32 }, Rectangle{ pos.x + 30 ,pos.y + 45, 45 , 45 }, Vector2{ 22.5, 22.5 }, this->rotation, WHITE);
		}
		DrawTexturePro(pUI->Pickaxe, Rectangle{ 0.0f, 0.0f, 32 , 32 }, Rectangle{ Invpos.x - 10 ,Invpos.y - 5 , 30  , 30 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		break;
	case Placed:
		DrawTexturePro(pUI->Pickaxe, Rectangle{ 0.0f, 0.0f, 32 , 32 }, Rectangle{ pos.x ,pos.y  , 30  , 30 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		break;
	case Mined:
		DrawTexturePro(pUI->Pickaxe, Rectangle{ 0.0f, 0.0f, 32 , 32 }, Rectangle{ pos.x  ,pos.y , 30  , 30 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		break;
	case Picked:
		DrawTexturePro(pUI->Pickaxe, Rectangle{ 0.0f, 0.0f, 32 , 32 }, Rectangle{ Invpos.x - 10 ,Invpos.y - 5 , 30  , 30 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		break;
	default:
		break;
	}
}

void Pickaxe::DrawName(Vector2 pos)
{
	DrawText("Pickaxe", pos.x - MeasureText(pUI->dirtName.c_str(), 20) / 2, 5, 20, WHITE);
}

bool Pickaxe::UseItem(Manager* pManager)
{
	Mining = true;
	itemstate = Onhand;
	
	return false;
}
