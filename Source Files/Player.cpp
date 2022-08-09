#include "../Header files/Player.h"
#include "../Header files/Manager.h"
#include <vector>
using namespace std;


Player::Player(UIInfo* pUI)
{
	pos = Vector2{ 0,-700};
	Xspeed = 0;
	Yspeed = 100;
	walkFrame = 0;
	frameDuration =0;
	orientation = Left;
	isWalking = false;
	InAir = true;
	this->pUI = pUI;
	texture = LoadTexture("textures/player/NPCSprites/Conrad.png");
}

Vector2 Player::getPos()
{
	return pos;
}

float Player::getSpeedX()
{
	return Xspeed;
}

void Player::Update(Manager* pManager)
{
	float delta = GetFrameTime();
	Vector2 minPoint = pManager->getminPoint();
	Vector2 maxPoint = pManager->getmaxPoint();
	vector<vector<Dirt*>>::const_iterator dirtblocks = pManager->getDirtBlocks();
	InAir = true;

	// check for collisions for picking items and not falling
	for (int i = ((int)(pos.y - minPoint.y) / 16) - 1; i < ((int)(pos.y - minPoint.y) / 16) + 6; i++)
	{
		bool br = false;
		for (int j = ((int)(pos.x - minPoint.x) / 16) - 4; j < (int)(pos.x - minPoint.x) / 16 + 4; j++) // 
		{
			if (i>0 && j > 0 &&i < WorldHeight && j < WorldWidth && dirtblocks[i][j]) {
			
				Vector2 dirtpos = dirtblocks[i][j]->GetPos();
				
				if (CheckCollisionRecs(Rectangle{ pos.x + 12 ,pos.y,17,64 }, Rectangle{ dirtpos.x, dirtpos.y , pUI->blockWidth, pUI->blockWidth })) {
					
					if (dirtblocks[i][j]->GetItemState() == Mined) { // picking action
					
						if (inventory.Insert(dirtblocks[i][j])) {
							dirtblocks[i][j]->setState(Picked);
							pManager->RemoveBlock(i, j);
						}
				
					}
					else {
						InAir = false;
						Yspeed = 0;
						pos.y = dirtpos.y - 63;
						br = true;
						break;
					}
				}
			}
		}
		if (br)
			break;
	}


	// movement

	if (IsKeyPressed(KEY_SPACE) && !InAir ) {
		Yspeed = -400;
		InAir = true;
	}



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
	inventory.DrawItems();
}



