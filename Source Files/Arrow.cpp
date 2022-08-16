#include "..\Header files\Arrow.h"
#include "../Header files/Manager.h"
#include "raymath.h"
using namespace std;

Arrow::Arrow(UIInfo* p, Vector2 position)
{

	pUI = p;
	pos = position;

	Damage = 20;
	FireSpeed = 2;
	itemClass = ItemClass::ammo;
	itemstate = Picked;
	Fired = false;
	Yspeed = 0;
	Xspeed = 0;
}

void Arrow::UpdateItem(Manager* pManager)
{

	

	 if (Fired) {
		 rotation = 180 - atan2f(-Yspeed, Xspeed) * (180.0f / PI);
		 if (rotation < 0) rotation += 360.0f;
		 Yspeed += 400 * min(GetFrameTime(), 0.05f);

		
	 }



	


	
	Vector2 minPoint = pManager->GetMinPoint();
	Vector2 maxPoint = pManager->GetMaxPoint();
	std::vector<std::vector<Item*>>::const_iterator dirtblocks = pManager->GetDirtBlocks();



 	if (Fired) {
		Vector2 temp = /*pManager->GetWorldXY*/(Vector2{ pos.x - 24 * cosf((this->rotation) * PI / 180) - 5 + Xspeed * min(GetFrameTime(), 0.05f), pos.y - 24 * sinf((this->rotation) * PI / 180) -5 + Yspeed * min(GetFrameTime(), 0.05f) });
		for (int i = ((int)(temp.y - minPoint.y) / blockHeight) - 1; i < ((int)(temp.y - minPoint.y) / blockHeight) + 6; i++)
		{
			bool br = false;
			for (int j = ((int)(temp.x - minPoint.x) / blockHeight) - 4; j < (int)(temp.x - minPoint.x) / blockHeight + 4; j++) // 
			{
				if (i > 0 && j > 0 && i < WorldHeight && j < WorldWidth && dirtblocks[i][j]) {

					Vector2 dirtpos = dirtblocks[i][j]->GetPos();
					if (CheckCollisionRecs(Rectangle{ temp.x, temp.y,10,10 }, Rectangle{ dirtpos.x, dirtpos.y , blockWidth, blockHeight })) {
						Yspeed = 0;
	//					pos.y =  dirtpos.y - 63.0f;
						Xspeed = 0;
						Fired = false;
						hit = true;
						br = true;
						pos = temp;
						break;
					}
				}
			}
			if (br)
				break;
		}
	}
	pos.y += Yspeed * min(GetFrameTime(), 0.05f);
	pos.x += Xspeed * min(GetFrameTime(), 0.05f);

	
//	Vector2Rotate(pos, rotation);
}

void Arrow::DrawItem(int rotation, PlayerOrientaion orientation, ItemState State, Vector2 Invpos)
{
	switch (itemstate)
	{
	case Onhand:
		DrawCircle(pos.x, pos.y, 2,RED);
		/* pos.y += -48 * sinf((180 - rotation) * PI / 180);
		 pos.x += 48 * cosf((180 - rotation) * PI / 180);*/
		DrawCircle(pos.x - 24 * cosf((this->rotation) * PI / 180) , pos.y - 24 * sinf(( this->rotation) * PI / 180 ) , 2, RED);
		DrawTexturePro(pUI->Arrow, Rectangle{ 200.0f , 103, 32 , 8 }, Rectangle{ pos.x , pos.y , 48 , 12 }, Vector2{ 24 , 6}, this->rotation, WHITE);
		DrawTexturePro(pUI->Arrow, Rectangle{ 256, 192, 64  , 64 }, Rectangle{ Invpos.x - 15 ,Invpos.y - 10 , 30  , 30 }, Vector2{ 0, 0 }, 0.0f, WHITE);

		break;
	case Placed:
		//DrawTextureRec(pUI->dirtTex, Rectangle{ 0,0 , 16 , 16 }, pos, WHITE); //for tile option 2	//DrawTextureRec(pUI->dirtTex, Rectangle{ 0,/*64*/ 3, blockWidth , blockHeight }, pos, /*WHITE*/Color{ 215, 162, 125, 255 });
		break;
	case Mined:
		DrawTexturePro(pUI->Arrow, Rectangle{ 256, 192, 64  , 64 }, Rectangle{ pos.x ,pos.y + 2.5f, 50  , 50 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		break;
	case Picked:
		DrawTexturePro(pUI->Arrow, Rectangle{ 256, 192, 64  , 64 }, Rectangle{ Invpos.x ,Invpos.y , blockWidth - 5 , blockHeight - 5 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		break;
	default:
		break;
	}
}

void Arrow::DrawName(Vector2 pos)
{
	DrawText("Bow", pos.x - MeasureText(pUI->dirtName.c_str(), 20) / 2, 5, 20, WHITE);
}

bool Arrow::Hit()
{
	return hit;
}

void Arrow::ApplyEffect()
{
}


bool Arrow::UseItem(Manager* pManager)
{
	Fired = true;
	itemstate = Onhand;
	rotation = Vector2Angle(GetMousePosition(), Vector2{ pos.x + 25,pos.y + 25 });
	Yspeed = - 500 * sinf((180-rotation )* PI / 180);
	Xspeed = 500 * cosf((180 - rotation) * PI / 180);
	pos = /*pManager->GetScreenXY(*/pManager->GetPlayer()->GetPos();
	pos.y += 45;
	pos.x += 15 ;
	return false;
}
