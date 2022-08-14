#include "..\Header files\Bow.h"
#include "../Header files/Manager.h"

Bow::Bow(UIInfo* p,Vector2 position)
{
    
    pUI = p;
    pos = position;

    Damage = 20;
    FireSpeed = 2;
    itemClass = RangedWeapon;
	frame = 0;
    animate = false;
	Yspeed = 0;
}

void Bow::UpdateItem(Manager* pManager)
{
    if (animate && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        animate = false;
        timer = 0;
		frame = 0;
    }
	else if (animate) {
		timer += GetFrameTime();
		if (timer >= 0.125)
		{
			timer = 0;
			frame = ++frame % 8;
		}
	}
	pos = pManager->GetScreenXY( pManager->GetPlayer()->GetPos());
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

void Bow::DrawItem(int rotation, PlayerOrientaion orientation, ItemState State, Vector2 Invpos)
{
	switch (itemstate)
	{
	case Onhand:
		if (orientation==Left)
		{
			DrawTexturePro(pUI->Bow, Rectangle{ 128.0f + frame * 64, 64, 64 , 64 }, Rectangle{ pos.x - 12.0f,pos.y + 15, 50 , 50 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		}
		else
		{
			DrawTexturePro(pUI->Bow, Rectangle{ 128.0f + frame * 64, 192.0f, 64 , 64 }, Rectangle{ pos.x + 2.0f,pos.y + 15, 50 , 50 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		}
		DrawTexturePro(pUI->Bow, Rectangle{ 256, 192, 64  , 64 }, Rectangle{ Invpos.x -15 ,Invpos.y-10 , 30  , 30  }, Vector2{ 0, 0 }, 0.0f, WHITE);
		break;
	case Placed:
		//DrawTextureRec(pUI->dirtTex, Rectangle{ 0,0 , 16 , 16 }, pos, WHITE); //for tile option 2	//DrawTextureRec(pUI->dirtTex, Rectangle{ 0,/*64*/ 3, blockWidth , blockHeight }, pos, /*WHITE*/Color{ 215, 162, 125, 255 });
		break;
	case Mined:
		DrawTexturePro(pUI->Bow, Rectangle{ 256, 192, 64  , 64 }, Rectangle{ pos.x ,pos.y + 2.5f, 50  , 50  }, Vector2{ 0, 0 }, 0.0f, WHITE);
		break;
	case Picked:
		DrawTexturePro(pUI->Bow, Rectangle{ 256, 192, 64  , 64 }, Rectangle{ Invpos.x ,Invpos.y , blockWidth - 5 , blockHeight - 5 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		break;
	default:
		break;
	}
}

void Bow::DrawName(Vector2 pos)
{
	DrawText("Bow", pos.x - MeasureText(pUI->dirtName.c_str(), 20) / 2, 5, 20, WHITE);
}

bool Bow::UseItem(Manager* pMmanager)
{
	animate = true;
	itemstate = Onhand;
    return false;
}
