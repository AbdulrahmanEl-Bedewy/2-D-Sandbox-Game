#include "..\Header files\Manager.h"

Manager::Manager(Dirt* d) 
{
	minPoint = Vector2{ -5000,-900 };
	maxPoint = Vector2{ 5000,900 };
	for (int i = minPoint.x; i < maxPoint.x; i += 32)
	{
		if (i == -8 || i == 24)
			continue;
		dirtblocks.push_back(Vector2{ (float)i, 368 });
	}
	dirt = d;
	nibba.rec = Rectangle{ 0,0,40,40 };
	nibba.Xspeed = 0;
	nibba.Yspeed = 100;
	nibba.InAir = true;
	camera = { 0 };
	camera.target = Vector2{ nibba.rec.x + 20 , nibba.rec.y + 20 };;
	camera.offset = Vector2{ 800 / 2.0f, 400 / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
}

void Manager::Update(int WindowWidth, int WindowHeight)
{

	UpdatePlayer(WindowWidth, WindowHeight);
	UpdateCam(WindowWidth, WindowHeight);

}

void Manager::UpdatePlayer(int WindowWidth, int WindowHeight)
{
	nibba.InAir = true;
	for (int i = floorf((nibba.rec.x - 40 + (-minPoint.x)) / 32.0f); i < ceilf((nibba.rec.x + 40 + (-minPoint.x)) / 32.0f); i++)
	{
		if (i < dirtblocks.size() && CheckCollisionRecs(nibba.rec, Rectangle{ dirtblocks[i].x, dirtblocks[i].y , 32,32 })) {
			nibba.InAir = false;
			nibba.Yspeed = 0;
			nibba.rec.y = dirtblocks[i].y - 39;
		}
	}
	if (IsKeyPressed(KEY_SPACE) && !nibba.InAir) {
		nibba.Yspeed = -400;
		nibba.InAir = true;
	}
	if (nibba.InAir)
	{
		nibba.Yspeed += 600 * GetFrameTime();
	}

	if (IsKeyDown(KEY_D)) {
		nibba.Xspeed = 200;
	}
	if (IsKeyDown(KEY_A))
	{
		if (nibba.Xspeed == 200)
			nibba.Xspeed = 0;
		else
			nibba.Xspeed = -200;
	}
	if (IsKeyUp(KEY_A) && IsKeyUp(KEY_D))
	{
		nibba.Xspeed = 0;
	}

	if (nibba.InAir)
	{
		nibba.rec.y += nibba.Yspeed * GetFrameTime();
	}

	nibba.rec.x += nibba.Xspeed * GetFrameTime();

	if (nibba.rec.y + 40 > maxPoint.y)
		nibba.rec.y = maxPoint.y - 40;

	if (nibba.rec.x + 40 > maxPoint.x)
		nibba.rec.x = maxPoint.x - 40;

	if (nibba.rec.x < minPoint.x)
		nibba.rec.x = minPoint.x;

}

void Manager::UpdateCam(int WindowWidth, int WindowHeight)
{
	camera.target = Vector2{ nibba.rec.x + 20 , nibba.rec.y + 20 };
	camera.offset = Vector2{ WindowWidth / 2.0f, WindowHeight / 2.0f };
	Vector2 max = GetWorldToScreen2D(Vector2{ maxPoint.x, maxPoint.y }, camera);
	Vector2 min = GetWorldToScreen2D(Vector2{ minPoint.x , minPoint.y }, camera);
	if (max.x < WindowWidth) camera.offset.x = WindowWidth - (max.x - WindowWidth / 2);
	if (max.y < WindowHeight) camera.offset.y = WindowHeight - (max.y - WindowHeight / 2);
	if (min.x > 0) camera.offset.x = WindowWidth / 2 - min.x;
	if (min.y > 0) camera.offset.y = WindowHeight / 2 - min.y;
}

void Manager::Draw(int WindowWidth, int WindowHeight)
{
	BeginDrawing();
	BeginMode2D(camera);
	ClearBackground(LIGHTGRAY);
	Vector2 itemPos = GetScreenToWorld2D(Vector2{ 10.0f , 10.0f }, camera);
	for (int i = floorf((nibba.rec.x - WindowWidth + (-minPoint.x)) / 32.0f); i < ceilf((nibba.rec.x + WindowWidth + (-minPoint.x) +32) / 32.0f); i++)
	{
		if(i<dirtblocks.size())
			dirt->DrawItem(dirtblocks[i], 0, Left, Placed);
	}
	DrawRectangleRec(nibba.rec, RED);
	EndMode2D();

	for (int i = 0; i < 10; i++)
	{
		DrawRectangle(10 + i * 35 + i * 5, 30, 35, 35, Fade(BLUE, 0.3));
	}
	EndDrawing();
}
