#include "../Header files/Dirt.h"
#include "../Header files/Manager.h"
#include "../Header files/Player.h"



Dirt::Dirt(UIInfo* p, ItemClass IC, Vector2 position)
{
	pUI = p;
	itemClass = IC;
	pos = position;
	Yspeed = 0;
	itemstate = Placed;
}

void Dirt::UpdateItem(Manager* pManager)
{
	Vector2 minPoint = pManager->GetMinPoint();
	Vector2 maxPoint = pManager->GetMaxPoint();
	vector<vector<Item*>>::const_iterator dirtblocks = pManager->GetDirtBlocks();


	if (itemstate == Mined) {
		bool br = false;
		for (int i = ((int)(pos.y - minPoint.y) / blockHeight) - 1; i < ((int)(pos.y - minPoint.y) / blockHeight) + 6; i++)
		{
			bool br = false;
			for (int j = ((int)(pos.x - minPoint.x) / blockHeight) - 4; j < (int)(pos.x - minPoint.x) / blockHeight + 4; j++) // 
			{
				if (i > 0 && j > 0 && i < WorldHeight && j < WorldWidth && dirtblocks[i][j]) {

					Vector2 dirtpos = dirtblocks[i][j]->GetPos();
					if (dirtpos.y > pos.y &&  CheckCollisionRecs(Rectangle{ pos.x ,pos.y + Yspeed * min(GetFrameTime(), 0.05f) ,blockWidth,blockHeight }, Rectangle{ dirtpos.x, dirtpos.y , blockWidth, blockHeight })) {
					
						Vector2 tempPos1 = pManager->GetCoordinate(pos);
						Vector2 tempPos2 = pManager->GetCoordinate(pos.x, dirtpos.y - blockHeight + 1);
						if (tempPos1.y < tempPos2.y) { // if block changes coordinate while falling
							pManager->RemovePickable(tempPos1.y, tempPos1.x, this);	 // move its position in the pickables list    /*pManager->GetCoordinate(pos).y, pManager->GetCoordinate(pos).x*/
							pManager->AddPickable(tempPos2.y, tempPos2.x, this);  //pManager->GetCoordinate(pos.x,pos.y + Yspeed * min(GetFrameTime(),0.05f)).y, pManager->GetCoordinate(pos).x
						}

						Yspeed = 0;
						pos.y = dirtpos.y - blockHeight + 1;
						br = true;
						break;
					} 
				}
			}
			if (br)
				break;
			
		}
		if(!br)
			Yspeed = (Yspeed + 100 * GetFrameTime() > 150) ? 150 : Yspeed + 100 * min(GetFrameTime(),0.05f);

		Vector2 tempPos1 = pManager->GetCoordinate(pos);
		Vector2 tempPos2 = pManager->GetCoordinate(pos.x, pos.y + Yspeed * min(GetFrameTime(), 0.05f));
		if ( tempPos1.y < tempPos2.y ) { // if block changes coordinate while falling
			pManager->RemovePickable(tempPos1.y,tempPos1.x , this);	 // move its position in the pickables list    /*pManager->GetCoordinate(pos).y, pManager->GetCoordinate(pos).x*/
			pManager->AddPickable(tempPos2.y, tempPos2.x, this);  //pManager->GetCoordinate(pos.x,pos.y + Yspeed * min(GetFrameTime(),0.05f)).y, pManager->GetCoordinate(pos).x
		}
		pos.y += Yspeed * min(GetFrameTime(), 0.05f);
	}
	
	if(itemstate == Onhand)
		pos = pManager->GetScreenXY(pManager->GetPlayer()->GetPos());
}

// to-do later
void Dirt::DrawItem(int rotation, PlayerOrientaion orientation, ItemState State, Vector2 Invpos)
{
	switch (itemstate)
	{
	case Onhand:
		if (orientation == Left)
		{
			pos.y += 32;
			DrawTextureEx(pUI->dirtTex, pos, rotation, 1.0, WHITE);
		}
		else
		{
			pos.y += 32;
			pos.x += 29;
			DrawTextureEx(pUI->dirtTex, pos, rotation, 1.0, WHITE);
		}
		
		DrawTextureRec(pUI->dirtTex, Rectangle{ 0, 3, blockWidth  , blockHeight }, Invpos, Fade(BROWN, 0.5));
		break;
	case Placed:
		DrawTextureRec(pUI->dirtTex, Rectangle{ 0,0 , 16 , 16 }, pos, WHITE); //for tile option 2	//DrawTextureRec(pUI->dirtTex, Rectangle{ 0,/*64*/ 3, blockWidth , blockHeight }, pos, /*WHITE*/Color{ 215, 162, 125, 255 });
		break;
	case Mined:
		DrawTexturePro(pUI->dirtTex, Rectangle{ 0, 0, 16 , 16 }, Rectangle{ pos.x + 2.5f,pos.y + 2.5f, blockWidth - 5 , blockHeight - 5 }, Vector2{ 0, 0 }, 0.0f, WHITE);
		//DrawTextureEx(pUI->dirtTex, pos, rotation, 1.0, WHITE);
		break;
	case Picked:
		DrawTextureRec(pUI->dirtTex, Rectangle{ 0, 3, blockWidth  , blockHeight }, Invpos, Fade(BROWN,0.5));
		break;
	default:
		break;
	}
}

void Dirt::DrawName(Vector2 pos)
{
	DrawText(pUI->dirtName.c_str(), pos.x - MeasureText(pUI->dirtName.c_str(), 20)/2, 5, 20, WHITE);
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
