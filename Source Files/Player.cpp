#include "../Header files/Player.h"
#include "../Header files/Manager.h"
#include "../Header files/Bow.h"
#include "../Header files/Sword.h"
#include <vector>
using namespace std;


Player::Player(UIInfo* pUI): inventory(pUI)
{
	pos = Vector2{ 0,0};
	Xspeed = 0;
	Yspeed = 100;
	walkFrame = 0;
	frameDuration =0;
	orientation = Left;
	isWalking = false;
	InAir = false;
	this->pUI = pUI;
	texture = LoadTexture("textures/player/NPCSprites/Conrad.png");
	inventory.Insert(new Bow(pUI, Vector2{ 0,50 }));
	inventory.Insert(new Sword(pUI, Vector2{ 0,50 }));
}

Vector2 Player::GetPos() const
{
	return pos;
}

float Player::GetSpeedX() const
{
	return Xspeed;
}

void Player::SetPos(Vector2 position)
{
	pos = position;
}

void Player::SetOrientation(PlayerOrientaion orientation)
{
	this->orientation = orientation;
}

PlayerOrientaion Player::GetOrientation()
{
	return orientation;
}

void Player::Update(Manager* pManager)
{
	float delta = min(GetFrameTime(),0.05f);
	Vector2 minPoint = pManager->GetMinPoint();
	Vector2 maxPoint = pManager->GetMaxPoint();
	vector<vector<Item*>>::const_iterator dirtblocks = pManager->GetDirtBlocks();
	unordered_map <int, vector<Item*>>* pickables = pManager->GetPickables();
	InAir = true;


	if (IsKeyDown(KEY_D)) {
		Xspeed = 400;
		orientation = Right;
		isWalking = true;
	}
	if (IsKeyDown(KEY_A))
	{
		if (Xspeed == 400) {
			Xspeed = 0;
			isWalking = false;
		}
		else {
			Xspeed = -400;
			orientation = Left;
			isWalking = true;
		}
	}
	if (IsKeyUp(KEY_A) && IsKeyUp(KEY_D))
	{
		Xspeed = 0;
		isWalking = false;
	}

	//Xspeed=-10;

	// check for collisions for picking items and not falling 
	for (int i = ((int)(pos.y - minPoint.y) / blockHeight) - 1; i < ((int)(pos.y - minPoint.y) / blockHeight) + 6; i++)
	{
		bool br = false;
		for (int j = ((int)(pos.x - minPoint.x) / blockHeight) - 4; j < (int)(pos.x - minPoint.x) / blockHeight + 4; j++) // 
		{
			if (i>0 && j > 0 &&i < WorldHeight && j < WorldWidth ) {
			
				
				
				if (CheckCollisionRecs(Rectangle{ pos.x + 12 + Xspeed * delta ,pos.y + Yspeed * delta,17,64 }, Rectangle{ j * blockWidth + minPoint.x , i * blockHeight + minPoint.y , blockWidth, blockHeight }))/*Rectangle{ dirtpos.x, dirtpos.y , blockWidth, blockHeight }*/ {

					//if (dirtblocks[i][j]->GetItemState() == Mined) { // picking action
					//
					//	if (inventory.Insert(dirtblocks[i][j])) {
					//		dirtblocks[i][j]->setState(Picked);
					//		pManager->RemoveBlock(i, j);
					//	}

					//}
					//else {
					/*for (int k = pickables[i][j].size()-1; k >=0 ; k--)
					{
						if (inventory.Insert(pickables[i][j][k]))
						{
							pickables[i][j][k]->setState(Picked);
							pManager->RemovePickable(i, j, pickables[i][j][k]);

							
						}
					}*/

					if ((*pickables).find(i * WorldWidth + j) != (*pickables).end()) {
						vector<Item*>& tempVector = (*(*pickables).find(i * WorldWidth + j)).second;
						for (int k = tempVector.size() - 1; k >= 0; k--)
						{
							if (inventory.Insert(tempVector[k]))
							{
								tempVector[k]->setState(Picked);
								pManager->RemovePickable(i, j, tempVector[k]);


							}
						}
					}

					if (dirtblocks[i][j]) {
						Vector2 dirtpos = dirtblocks[i][j]->GetPos();

						if (Xspeed != 0 && dirtpos.y - pos.y > 2 * blockHeight) {
							pos.y -= pos.y + 63 - dirtpos.y;
						}
						/*else*/
						if (Xspeed > 0 && dirtpos.y - pos.y < 2 * blockHeight && dirtpos.x > pos.x + 12)
						{
							Xspeed = 0;
							pos.x = dirtpos.x - 29;
							isWalking = false;
						}
						if (Xspeed < 0 && dirtpos.y - pos.y < 2 * blockHeight && dirtpos.x < pos.x + 12) {
							Xspeed = 0;
							pos.x = dirtpos.x + blockWidth - 12;
							isWalking = false;
						}
						if (Yspeed < 0 && dirtpos.y < pos.y && dirtpos.x < pos.x + 29 && dirtpos.x + blockHeight > pos.x + 12) {
							Yspeed = 0;
							pos.y = dirtpos.y + blockHeight;
						}
						if (InAir && dirtpos.y - pos.y > 64 - 10 && dirtpos.x < pos.x + 29 && dirtpos.x + blockHeight > pos.x + 12) {
							InAir = false;
							Yspeed = 0;
							pos.y = dirtpos.y - 63;
							br = true;
						}

						/*InAir = false;
						Yspeed = 0;
						pos.y = dirtpos.y - 63;
						br = true;
						break;
						}*/
					}
				}
			}
		}
		if (br)
			break;
		else
		{
			InAir = true;
		}
	}


	// movement

	if (IsKeyPressed(KEY_SPACE) && !InAir ) {
		Yspeed = -400;
		InAir = true;
	}




	if (InAir)
	{
		Yspeed = (Yspeed > 650) ? 650 : Yspeed + 700 * delta;
		isWalking = false;
	}

	pos.y += Yspeed * delta;
	pos.x += Xspeed * delta;


	if (pos.y + 64 > maxPoint.y)
		pos.y = maxPoint.y - 64;

	if (pos.x + 29 > maxPoint.x)
		pos.x = maxPoint.x - 29;

	if (pos.x < minPoint.x)
		pos.x = minPoint.x;
	if (IsKeyPressed(KEY_R)) {
		pos.x = -60;
		pos.y = -700;
	}
	if (IsKeyPressed(KEY_E)) {
		pos.x = 70;
		pos.y = -700;
	}

	//inventory management
	inventory.IncrementSelectedPos(-GetMouseWheelMove());
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		inventory.UseItem(pManager);
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		Vector2 coordinate = pManager->GetCoordinateFromScreen(GetMousePosition());
		if (dirtblocks[coordinate.y][coordinate.x])
		{
			dirtblocks[coordinate.y][coordinate.x]->setState(Mined);
			pManager->AddPickable(coordinate.y, coordinate.x, dirtblocks[coordinate.y][coordinate.x]);
			pManager->RemoveBlock(coordinate.y, coordinate.x);
		}
	}

	if (IsKeyPressed(KEY_ENTER)) {
		inventory.ToggleExpanded();
	}
}

void Player::UpdateInventory(Manager* pManager)
{
	inventory.UpdateSelected(pManager);
}

void Player::draw()
{

		if (isWalking)
		{
			frameDuration += GetFrameTime();
			if (frameDuration >= 0.15) {
				walkFrame = (walkFrame + 1) % 4;
				frameDuration = 0;
			}
		}
		else
		{
			frameDuration = 0;
			walkFrame = 0;
		}

		if (orientation == Left)
			DrawTexturePro(texture, Rectangle{ walkFrame * 32.0f + 4,  96 ,32,31 }, Rectangle{ pos.x, pos.y ,64,64 }, Vector2{ 0,0 }, 0.0f, WHITE);
		else
			DrawTexturePro(texture, Rectangle{ walkFrame * 32.0f + 4, 32,32,31 }, Rectangle{ pos.x, pos.y ,64,64 }, Vector2{0,0 }, 0.0f, WHITE);

	
}

void Player::drawInv()
{
	inventory.DrawItems(orientation);
}



