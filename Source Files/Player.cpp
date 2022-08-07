#include "../Header files/Player.h"
#include "../Header files/Manager.h"
#include <vector>
using namespace std;


Player::Player(UIInfo* pUI)
{
	pos = Vector2{ 0,-400};
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

void Player::Update(Manager* pManager)
{
	float delta = GetFrameTime();
	Vector2 minPoint = pManager->getminPoint();
	Vector2 maxPoint = pManager->getmaxPoint();
	vector<vector<Dirt*>>::const_iterator dirtblocks = pManager->getDirtBlocks();
	InAir = true;
	if (pos.y >= -80)
	{
		for (int i = floorf((pos.x - 40 + (-minPoint.x)) / pUI->blockWidth); i < ceilf((pos.x + 40 + (-minPoint.x)) / pUI->blockWidth); i++)
		{
			if (i < dirtblocks[0].size()) {
				Vector2 dirtpos = dirtblocks[0][i]->GetPos();
				if (CheckCollisionRecs(Rectangle{ pos.x + 12 ,pos.y,17,64 }, Rectangle{ dirtpos.x, dirtpos.y , pUI->blockWidth, pUI->blockWidth })) {
					InAir = false;
					Yspeed = 0;
					pos.y = dirtpos.y - 63;
				}
			}
		}
	}


	if (IsKeyPressed(KEY_SPACE) && !InAir ) {
		Yspeed = -400;
		InAir = true;
	}



	if (IsKeyDown(KEY_D)) {
		Xspeed = 200;
		orientation = Right;
		isWalking = true;
	}
	if (IsKeyDown(KEY_A))
	{
		if (Xspeed == 200) {
			Xspeed = 0;
			isWalking = false;
		}
		else {
			Xspeed = -200;
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

	if (pos.x + 64 > maxPoint.x)
		pos.x = maxPoint.x - 64;

	if (pos.x < minPoint.x)
		pos.x = minPoint.x;
	if (IsKeyPressed(KEY_R)) {
		pos.x = -60;
		pos.y = -400;
	}
	if (IsKeyPressed(KEY_E)) {
		pos.x = 70;
		pos.y = -400;
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



