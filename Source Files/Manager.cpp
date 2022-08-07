#include "..\Header files\Manager.h"

Manager::Manager(UIInfo* p) :player(p)
{
	minPoint = Vector2{ -5000,-900 };
	maxPoint = Vector2{ 500,900 };
	pUI = p;

	scrollingBack1 = 0.0f;
	scrollingBack2 = 0.0f;
	scrollingBack3 = 0.0f;
	scrollingBack4 = 0.0f;
	scrollingBack5 = 0.0f;


	GenerateWorld();



	camera = { 0 };
	camera.target = Vector2{ player.getPos().x + 20 , player.getPos().y + 20 };;
	camera.offset = Vector2{ 800 / 2.0f, 400 / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 0.8;
}

void Manager::Update(int WindowWidth, int WindowHeight)
{

	player.Update(this);
	UpdateCam(WindowWidth, WindowHeight);

	scrollingBack1 -= 0.7f;
	scrollingBack2 -= 0.5f;
	scrollingBack3 -= 1.0f;
	scrollingBack4 -= 0.5f;
	scrollingBack5 -= 1.0f;


	if (scrollingBack1 <= -pUI->background1.width * WindowWidth / pUI->background1.width) scrollingBack1 = 0;
	if (scrollingBack2 <= -pUI->background2.width * WindowWidth / pUI->background2.width) scrollingBack2 = 0;
	if (scrollingBack3 <= -pUI->background3.width * WindowWidth / pUI->background3.width) scrollingBack3 = 0;
	if (scrollingBack4 <= -pUI->background4.width * WindowWidth / pUI->background4.width) scrollingBack4 = 0;
	if (scrollingBack5 <= -pUI->background5.width * WindowWidth / pUI->background5.width) scrollingBack5 = 0;
}



void Manager::UpdateCam(int WindowWidth, int WindowHeight)
{
	Vector2 pos = player.getPos();
	camera.target = Vector2{ pos.x + 20 , pos.y + 20 };
	camera.offset = Vector2{ WindowWidth / 2.0f, WindowHeight / 2.0f };
	Vector2 max = GetWorldToScreen2D(Vector2{ maxPoint.x, maxPoint.y }, camera);
	Vector2 min = GetWorldToScreen2D(Vector2{ minPoint.x , minPoint.y }, camera);
	if (max.x < WindowWidth) camera.offset.x = WindowWidth - (max.x - WindowWidth / 2);
	if (max.y < WindowHeight) camera.offset.y = WindowHeight - (max.y - WindowHeight / 2);
	if (min.x > 0) camera.offset.x = WindowWidth / 2 - min.x;
	if (min.y > 0) camera.offset.y = WindowHeight / 2 - min.y;
}

void Manager::GenerateWorld()
{
	for (int i = (minPoint.y + maxPoint.y) / 2; i < maxPoint.y; i += pUI->blockHeight)
	{
		vector<Dirt*> temp;
		for (int j = minPoint.x; j < maxPoint.x; j += pUI->blockWidth)
		{
			if (j == -8 || j == 24)
				continue;
			temp.push_back(new Dirt(pUI, Block, Vector2{ (float)j, (float)i }));
		}
		dirtblocks.push_back(temp);
	}
}

bool isSurfaceTile(int row , int column) {
	// should be redone when a propper 2D array/vector system is implemented to represent the world
	return row > 0;
}

void Manager::Draw(int WindowWidth, int WindowHeight)
{
	BeginDrawing();
	ClearBackground(LIGHTGRAY);

	float backgroundWidth1 = pUI->background1.width * WindowWidth / pUI->background1.width;
	float backgroundWidth2 = pUI->background2.width * WindowWidth / pUI->background2.width;
	float backgroundWidth3 = pUI->background3.width * WindowWidth / pUI->background3.width;
	float backgroundWidth4 = pUI->background4.width * WindowWidth / pUI->background4.width;
	float backgroundWidth5 = pUI->background5.width * WindowWidth / pUI->background5.width;

	DrawTexturePro(pUI->background1, Rectangle{ 0,0,(float)pUI->background1.width, (float)pUI->background1.height }, Rectangle{ scrollingBack1, 0, backgroundWidth1, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
	DrawTexturePro(pUI->background1, Rectangle{ 0,0,(float)pUI->background1.width, (float)pUI->background1.height }, Rectangle{ scrollingBack1 + backgroundWidth1, 0, backgroundWidth1, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);



	DrawTexturePro(pUI->background2, Rectangle{ 0,0,(float)pUI->background2.width, (float)pUI->background2.height }, Rectangle{ scrollingBack2, 0, backgroundWidth2, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
	DrawTexturePro(pUI->background2, Rectangle{ 0,0,(float)pUI->background2.width, (float)pUI->background2.height }, Rectangle{ scrollingBack2 + backgroundWidth2, 0, backgroundWidth2, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);


	DrawTexturePro(pUI->background3, Rectangle{ 0,0,(float)pUI->background3.width, (float)pUI->background3.height }, Rectangle{ scrollingBack3, 0, backgroundWidth3, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
	DrawTexturePro(pUI->background3, Rectangle{ 0,0,(float)pUI->background3.width, (float)pUI->background3.height }, Rectangle{ scrollingBack3 + backgroundWidth3, 0, backgroundWidth3, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);

	DrawTexturePro(pUI->background4, Rectangle{ 0,0,(float)pUI->background4.width, (float)pUI->background4.height }, Rectangle{ scrollingBack4, 0, backgroundWidth4, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
	DrawTexturePro(pUI->background4, Rectangle{ 0,0,(float)pUI->background4.width, (float)pUI->background4.height }, Rectangle{ scrollingBack4 + backgroundWidth4, 0, backgroundWidth4, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);

	DrawTexturePro(pUI->background5, Rectangle{ 0,0,(float)pUI->background5.width, (float)pUI->background5.height }, Rectangle{ scrollingBack5, 0, backgroundWidth5, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
	DrawTexturePro(pUI->background5, Rectangle{ 0,0,(float)pUI->background5.width, (float)pUI->background5.height }, Rectangle{ scrollingBack5 + backgroundWidth5, 0, backgroundWidth5, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);


	BeginMode2D(camera);

	
	Vector2 pos = player.getPos();

	for (int i = /*floorf((nibba.rec.y - WindowHeight + (-minPoint.y)) / (float)pUI->blockHeight)*/ 0; i < ceilf((pos.y + WindowHeight  /* + (-minPoint.y)*/) / (float)pUI->blockHeight); i++)
	{
		for (int j = floorf((pos.x - (2 - camera.zoom) * WindowWidth + (-minPoint.x)) / 32.0f); j < ceilf((pos.x + (2 - camera.zoom)*WindowWidth + (-minPoint.x) + 32) / 32.0f); j++) // 
		{
			if (i < dirtblocks.size() && j < dirtblocks[i].size())
				dirtblocks[i][j]->DrawItem(0, Right, Placed);
		}
	}


	player.draw();
	EndMode2D();


	for (int i = 0; i < 10; i++)
	{
		DrawRectangle(10 + i * 35 + i * 5, 30, 35, 35, Fade(BLUE, 0.3));
	}
	EndDrawing();
}

Vector2 Manager::getminPoint()
{
	return minPoint;
}

Vector2 Manager::getmaxPoint()
{
	return maxPoint;
}

vector<vector<Dirt*>>::const_iterator  Manager::getDirtBlocks()
{
	return dirtblocks.begin();
}
