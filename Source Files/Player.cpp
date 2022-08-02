//#include "../Header files/Player.h"
//#include <vector>
//using namespace std;
//
//void Player::Update(int WindowWidth, int WindowHeight, Vector2& maxPoint, Vector2& minPoint )
//{
//	float delta = GetFrameTime();
//
//	InAir = true;
//	for (int i = floorf((pos.x - 40 + (-minPoint.x)) / 32.0f); i < ceilf((pos.x + 40 + (-minPoint.x)) / 32.0f); i++)
//	{
//		if (i < dirtblocks.size() && CheckCollisionRecs(Rectangle{ pos.x, pos.y , 32,32 }, Rectangle{ dirtblocks[i].x, dirtblocks[i].y , 32,32 })) {
//			nibba.InAir = false;
//			nibba.Yspeed = 0;
//			pos.y = dirtblocks[i].y - 39;
//		}
//	}
//	if (IsKeyPressed(KEY_SPACE) && !InAir) {
//		Yspeed = -400;
//		InAir = true;
//	}
//	if (InAir)
//	{
//		Yspeed += 400 * delta;
//	}
//
//	if (IsKeyDown(KEY_D)) {
//		Xspeed = 200;
//	}
//	if (IsKeyDown(KEY_A))
//	{
//		if (Xspeed == 200)
//			Xspeed = 0;
//		else
//			Xspeed = -200;
//	}
//	if (IsKeyUp(KEY_A) && IsKeyUp(KEY_D))
//	{
//		Xspeed = 0;
//	}
//
//	if (InAir)
//	{
//		pos.y += Yspeed * delta;
//	}
//
//	pos.x += Xspeed * delta;
//
//	if (pos.y + 40 > maxPoint.y)
//		pos.y = maxPoint.y - 40;
//
//	if (pos.x + 40 > maxPoint.x)
//		pos.x = maxPoint.x - 40;
//
//	if (pos.x < minPoint.x)
//		pos.x = minPoint.x;
//}
//
//
//
