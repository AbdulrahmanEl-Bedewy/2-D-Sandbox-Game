#include "..\Header files\Manager.h"
#include "../SimpleNoise/SimplexNoise.h"
//#include <iostream>
//using namespace std;

Manager::Manager(UIInfo* p) :player(p)
{
	pUI = p;

	minPoint = Vector2{ -WorldWidth / 2 * blockWidth ,-WorldHeight / 2 * blockHeight };
	maxPoint = Vector2{ WorldWidth / 2 * blockWidth ,WorldHeight / 2 * blockHeight };

	scrollingBack1 = 0.0f;
	scrollingBack2 = 0.0f;
	scrollingBack3 = 0.0f;
	scrollingBack4 = 0.0f;
	scrollingBackSun_Moon = 0.0f;

	Day = true;
	

	GenerateWorld();



	camera = { 0 };
	camera.target = Vector2{ player.GetPos().x + 20 , player.GetPos().y + 20 };;
	camera.offset = Vector2{ 800 / 2.0f, 400 / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1;
}

void Manager::Update(int WindowWidth, int WindowHeight)
{

	player.Update(this);
	UpdateCam(WindowWidth, WindowHeight);

	scrollingBackSun_Moon += 10* GetFrameTime();
	if (scrollingBackSun_Moon >= 1600)
	{
		Day = !Day;
		scrollingBackSun_Moon = 0;
	}

	if (player.GetSpeedX() < 0) {
		scrollingBack1 += 0.1f;
		scrollingBack2 += 0.2f;
		scrollingBack3 += 0.7f;
		scrollingBack4 += 1.0f;
	}
	else if (player.GetSpeedX() > 0) {
		scrollingBack1 -= 0.1f;
		scrollingBack2 -= 0.2f;
		scrollingBack3 -= 0.7f;
		scrollingBack4 -= 1.0f;
	}

	switch (Day)
	{
	case 0:
		if (scrollingBack1 <= -pUI->Nightbackground1.width * WindowWidth / pUI->Morningbackground1.width) scrollingBack1 = 0;
		if (scrollingBack2 <= -pUI->Nightbackground1.width * WindowWidth / pUI->Morningbackground2.width) scrollingBack2 = 0;
		if (scrollingBack3 <= -pUI->Nightbackground1.width * WindowWidth / pUI->Morningbackground3.width) scrollingBack3 = 0;
		if (scrollingBack4 <= -pUI->Nightbackground1.width * WindowWidth / pUI->Morningbackground4.width) scrollingBack4 = 0;
		//if (scrollingBack5 <= -pUI->Morningbackground5.width * WindowWidth / pUI->Morningbackground5.width) scrollingBack5 = 0;

		//too far to right
		if (scrollingBack1 >= pUI->Nightbackground1.width * WindowWidth / pUI->Morningbackground1.width) scrollingBack1 = 0;
		if (scrollingBack2 >= pUI->Nightbackground1.width * WindowWidth / pUI->Morningbackground2.width) scrollingBack2 = 0;
		if (scrollingBack3 >= pUI->Nightbackground1.width * WindowWidth / pUI->Morningbackground3.width) scrollingBack3 = 0;
		if (scrollingBack4 >= pUI->Nightbackground1.width * WindowWidth / pUI->Morningbackground4.width) scrollingBack4 = 0;
		
		if (scrollingBackSun_Moon >= pUI->Moon.width * WindowWidth / pUI->Moon.width) scrollingBackSun_Moon = 0;
		break;
	case 1:

		 // went too far to left
		if (scrollingBack1 <= -pUI->Morningbackground1.width * WindowWidth / pUI->Morningbackground1.width) scrollingBack1 = 0;
		if (scrollingBack2 <= -pUI->Morningbackground2.width * WindowWidth / pUI->Morningbackground2.width) scrollingBack2 = 0;
		if (scrollingBack3 <= -pUI->Morningbackground3.width * WindowWidth / pUI->Morningbackground3.width) scrollingBack3 = 0;
		if (scrollingBack4 <= -pUI->Morningbackground4.width * WindowWidth / pUI->Morningbackground4.width) scrollingBack4 = 0;
		//if (scrollingBack5 <= -pUI->Morningbackground5.width * WindowWidth / pUI->Morningbackground5.width) scrollingBack5 = 0;

		//too far to right
		if (scrollingBack1 >= pUI->Morningbackground1.width * WindowWidth / pUI->Morningbackground1.width) scrollingBack1 = 0;
		if (scrollingBack2 >= pUI->Morningbackground2.width * WindowWidth / pUI->Morningbackground2.width) scrollingBack2 = 0;
		if (scrollingBack3 >= pUI->Morningbackground3.width * WindowWidth / pUI->Morningbackground3.width) scrollingBack3 = 0;
		if (scrollingBack4 >= pUI->Morningbackground4.width * WindowWidth / pUI->Morningbackground4.width) scrollingBack4 = 0;

		if (scrollingBackSun_Moon >= pUI->Sun.width * WindowWidth / pUI->Sun.width) scrollingBackSun_Moon = 0;
	//	if (scrollingBack5 >= pUI->Morningbackground5.width * WindowWidth / pUI->Morningbackground5.width) scrollingBack5 = 0;

		break;
	default:
		break;
	}
}



void Manager::UpdateCam(int WindowWidth, int WindowHeight)
{
	Vector2 pos = player.GetPos();
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
	float arr[WorldWidth];
	float seed = rand();

	//float fNoiseSeed1D[344];
	//for (int i = 0; i < 344; i++) fNoiseSeed1D[i] = (float)rand() / (float)RAND_MAX;
	//PerlinNoise1D(344, fNoiseSeed1D, 4, 0.4, arr); //6, 0.6
	
	//for (int i = 0; i < 344; i++) {
	//	arr[i] = (int)(arr[i] * 800) / blockHeight * blockHeight;
	//}
	//int test = 0;
	float smoothness = 320.0f;
	float modifier = 0.02; //0.03
	for (int x = 0; x < WorldWidth; x++) {
		//if (test % 80 == 0)
		//	smoothness = 10;
		//else
		//	smoothness = 320;
		arr[x] = (int)(SimplexNoise::noise(x / smoothness, seed)* 600 / blockHeight) * blockHeight;
	}

	int first = 0;

	for (int i = 0; i < WorldHeight /*112*/; i++)
	{
		vector<Item*> temp;
		wall.push_back(vector<int>{});
		for (int j = 0; j < WorldWidth /*344*/; j++)
		{
			
			
			if (i * blockHeight + minPoint.y < -arr[j]) /*if(j*blockHeight -5000>0)*/
			{
				temp.push_back(NULL);
				wall[i].push_back(0);
			}
			else {
				wall[i].push_back(1);
				if (first < 51 && j * blockWidth + minPoint.x > 0) // mined blocks to test picking up 
				{
					Item* d = new Dirt(pUI, Block, Vector2{ (float)j * blockWidth + minPoint.x, (float)i * blockHeight + minPoint.y });
					d->setState(Mined);
					temp.push_back(d);
					first++;
				}
				else
				{
					if (round(SimplexNoise::noise(j * modifier + seed, i * modifier + seed)) == 0 && i * blockHeight + minPoint.y > 400 && i!=155) {
						temp.push_back(NULL);
					}
					else {
						Item* d = new Dirt(pUI, Block, Vector2{ (float)j * blockWidth + minPoint.x, (float)i * blockHeight + minPoint.y });
						d->setState(Placed);
						temp.push_back(d);
					}
				}
			}
		}
		dirtblocks.push_back(temp);
	}


	//for (int i = (minPoint.y + maxPoint.y) / 2; i < maxPoint.y; i += pUI->blockHeight)
	//{
	//	vector<Dirt*> temp;
	//	for (int j = minPoint.x; j < maxPoint.x; j += pUI->blockWidth)
	//	{
	//		if (j == -8 || j == 24)
	//			continue;
	//		temp.push_back(new Dirt(pUI, Block, Vector2{ (float)j, (float)i }));
	//	}
	//	dirtblocks.push_back(temp);
	//}
	//cout << sizeof(dirtblocks) << endl << sizeof(dirtblocks[0]) << endl << sizeof(*dirtblocks[154][350]) << endl;
}

bool Manager::isSurfaceTile(int row , int column) {
	// used to determine which tiles should be grass
	if (row == 0 ||column==0)
		return true;
	
	if (!dirtblocks[row-1][column])
		return true;

	return false;
}

Vector2 Manager::GetCoordinate(int x, int y)
{
	return Vector2{ (float)((int)(x - minPoint.x) / blockWidth), (float)((int)(y - minPoint.y) / blockHeight) };
}

Vector2 Manager::GetCoordinate(Vector2 p)
{
	return  Vector2{ (float)((int)(p.x - minPoint.x) / blockWidth), (float)((int)(p.y - minPoint.y) / blockHeight) };
}

Vector2 Manager::GetCoordinateFromScreen(int x, int y)
{
	//need fix
	return Vector2{ (float)((int)(x - minPoint.x)/blockWidth ), (float)((int)(y - minPoint.y) / blockHeight ) };
}

Vector2 Manager::GetCoordinateFromScreen(Vector2 p)
{
	p = GetScreenToWorld2D(p, camera);
	return  Vector2{ (float)((int)(p.x - minPoint.x) / blockWidth ), (float)((int)(p.y - minPoint.y) / blockHeight ) };
}

Vector2 Manager::GetWorldXY(Vector2 pos)
{
	return GetScreenToWorld2D(pos,camera);
}

void Manager::Draw(int WindowWidth, int WindowHeight)
{
	BeginDrawing();
	ClearBackground(LIGHTGRAY);
	//BeginScissorMode(0, 0, WindowWidth, WindowHeight);
	

	float backgroundWidth1;
	float backgroundWidth2;
	float backgroundWidth3;
	float backgroundWidth4;
	/*float MorningbackgroundWidth5 = pUI->Morningbackground5.width * WindowWidth / pUI->Morningbackground5.width;*/

	switch (Day)
	{
	case 0:
		backgroundWidth1 = pUI->Nightbackground1.width * WindowWidth / pUI->Nightbackground1.width + WindowWidth * 0.25;
		backgroundWidth2 = pUI->Nightbackground2.width * WindowWidth / pUI->Nightbackground2.width;
		backgroundWidth3 = pUI->Nightbackground3.width * WindowWidth / pUI->Nightbackground3.width;
		backgroundWidth4 = pUI->Nightbackground4.width * WindowWidth / pUI->Nightbackground4.width;

		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack1, 0, backgroundWidth1, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack1 + backgroundWidth1, 0, backgroundWidth1, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack1 - backgroundWidth1, 0, backgroundWidth1, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);



		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack2, 0, backgroundWidth2, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack2 + backgroundWidth2, 0, backgroundWidth2, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack2 - backgroundWidth2, 0, backgroundWidth2, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);


		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack3, 0, backgroundWidth3, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack3 + backgroundWidth3, 0, backgroundWidth3, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack3 - backgroundWidth3, 0, backgroundWidth3, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);


		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack4, 0, backgroundWidth4, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack4 + backgroundWidth4, 0, backgroundWidth4, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack4 - backgroundWidth4, 0, backgroundWidth4, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);

		DrawTexturePro(pUI->Moon, Rectangle{ 0,0,(float)pUI->Moon.width, (float)pUI->Moon.height }, Rectangle{ scrollingBackSun_Moon - 60, 100, 200, 200 }, Vector2{ 0,0 }, 0.0f, WHITE);

		break;
	case 1:

		backgroundWidth1 = pUI->Morningbackground1.width * WindowWidth / pUI->Morningbackground1.width + WindowWidth * 0.25;
		backgroundWidth2 = pUI->Morningbackground2.width * WindowWidth / pUI->Morningbackground2.width;
		backgroundWidth3 = pUI->Morningbackground3.width * WindowWidth / pUI->Morningbackground3.width;
		backgroundWidth4 = pUI->Morningbackground4.width * WindowWidth / pUI->Morningbackground4.width;

		DrawTexturePro(pUI->Morningbackground1, Rectangle{ 0,0,(float)pUI->Morningbackground1.width, (float)pUI->Morningbackground1.height }, Rectangle{ scrollingBack1, 0, backgroundWidth1, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Morningbackground1, Rectangle{ 0,0,(float)pUI->Morningbackground1.width, (float)pUI->Morningbackground1.height }, Rectangle{ scrollingBack1 + backgroundWidth1, 0, backgroundWidth1, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Morningbackground1, Rectangle{ 0,0,(float)pUI->Morningbackground1.width, (float)pUI->Morningbackground1.height }, Rectangle{ scrollingBack1 - backgroundWidth1, 0, backgroundWidth1, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);


		DrawTexturePro(pUI->Sun, Rectangle{ 0,0,(float)pUI->Sun.width, (float)pUI->Sun.height }, Rectangle{ scrollingBackSun_Moon - 60, 100, 200, 200}, Vector2{ 0,0 }, 0.0f, WHITE);


		DrawTexturePro(pUI->Morningbackground2, Rectangle{ 0,0,(float)pUI->Morningbackground2.width, (float)pUI->Morningbackground2.height }, Rectangle{ scrollingBack2, 0, backgroundWidth2, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Morningbackground2, Rectangle{ 0,0,(float)pUI->Morningbackground2.width, (float)pUI->Morningbackground2.height }, Rectangle{ scrollingBack2 + backgroundWidth2, 0, backgroundWidth2, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Morningbackground2, Rectangle{ 0,0,(float)pUI->Morningbackground2.width, (float)pUI->Morningbackground2.height }, Rectangle{ scrollingBack2 - backgroundWidth2, 0, backgroundWidth2, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);


		DrawTexturePro(pUI->Morningbackground3, Rectangle{ 0,0,(float)pUI->Morningbackground3.width, (float)pUI->Morningbackground3.height }, Rectangle{ scrollingBack3, 0, backgroundWidth3, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Morningbackground3, Rectangle{ 0,0,(float)pUI->Morningbackground3.width, (float)pUI->Morningbackground3.height }, Rectangle{ scrollingBack3 + backgroundWidth3, 0, backgroundWidth3, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Morningbackground3, Rectangle{ 0,0,(float)pUI->Morningbackground3.width, (float)pUI->Morningbackground3.height }, Rectangle{ scrollingBack3 - backgroundWidth3, 0, backgroundWidth3, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);


		DrawTexturePro(pUI->Morningbackground4, Rectangle{ 0,0,(float)pUI->Morningbackground4.width, (float)pUI->Morningbackground4.height }, Rectangle{ scrollingBack4, 0, backgroundWidth4, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Morningbackground4, Rectangle{ 0,0,(float)pUI->Morningbackground4.width, (float)pUI->Morningbackground4.height }, Rectangle{ scrollingBack4 + backgroundWidth4, 0, backgroundWidth4, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Morningbackground4, Rectangle{ 0,0,(float)pUI->Morningbackground4.width, (float)pUI->Morningbackground4.height }, Rectangle{ scrollingBack4 - backgroundWidth4, 0, backgroundWidth4, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);



	default:
		break;
	}

	//DrawTexturePro(pUI->Morningbackground5, Rectangle{ 0,0,(float)pUI->Morningbackground5.width, (float)pUI->Morningbackground5.height }, Rectangle{ scrollingBack5, 0, MorningbackgroundWidth5, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
	//DrawTexturePro(pUI->Morningbackground5, Rectangle{ 0,0,(float)pUI->Morningbackground5.width, (float)pUI->Morningbackground5.height }, Rectangle{ scrollingBack5 + MorningbackgroundWidth5, 0, MorningbackgroundWidth5, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
	//DrawTexturePro(pUI->Morningbackground5, Rectangle{ 0,0,(float)pUI->Morningbackground5.width, (float)pUI->Morningbackground5.height }, Rectangle{ scrollingBack5 - MorningbackgroundWidth5, 0, MorningbackgroundWidth5, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);



	BeginMode2D(camera);

	
	Vector2 CenterPos = GetScreenToWorld2D(Vector2{ WindowWidth / 2.0f,WindowHeight / 2.0f }, camera); //player.GetPos();
	

	for (int i = ((int)(CenterPos.y - minPoint.y )/blockHeight )  - WindowHeight /blockHeight * 0.5 ; i < (((CenterPos.y - minPoint.y) / blockHeight) + WindowHeight / blockHeight * 0.5) +3 ; i++)
	{
		for (int j = ((int)(CenterPos.x - minPoint.x) / blockWidth )  - WindowWidth / blockWidth * 0.5 - 3; j < ((CenterPos.x - minPoint.x)/ blockWidth + WindowWidth / blockWidth *0.5 ) +3; j++) // 
		{
			if (i < dirtblocks.size() && j < dirtblocks[i].size()) {
				if (wall[i][j])
				{
					DrawTexturePro(pUI->wall, Rectangle{ 21,75, 50,50 }, Rectangle{ (float)j * blockWidth + minPoint.x, (float)i * blockHeight + minPoint.y, blockWidth , blockHeight }, Vector2{ 0,0 }, 0.0f, WHITE);

					//DrawTexturePro(pUI->wall, Rectangle{ 0,0, 2048,2048 }, Rectangle{ (float)j * blockWidth + minPoint.x, (float)i * blockHeight + minPoint.y, blockWidth , blockHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
				}
				if(dirtblocks[i][j])
					dirtblocks[i][j]->DrawItem(0, Right, Placed);
			}
		}
	}


	//for (int i = /*floorf((nibba.rec.y - WindowHeight + (-minPoint.y)) / (float)pUI->blockHeight)*/ 0; i < ceilf((pos.y + WindowHeight  /* + (-minPoint.y)*/) / (float)pUI->blockHeight); i++)
	//{
	//	for (int j = floorf((pos.x - (2 - camera.zoom) * WindowWidth + (-minPoint.x)) / 32.0f); j < ceilf((pos.x + (2 - camera.zoom)*WindowWidth + (-minPoint.x) + 32) / 32.0f); j++) // 
	//	{
	//		if (i < dirtblocks.size() && j < dirtblocks[i].size() && dirtblocks[i][j])
	//			dirtblocks[i][j]->DrawItem(0, Right, Placed);
	//	}
	//}


	player.draw();
	EndMode2D();

	DrawFPS(20, 60);
	player.drawInv();
	//EndScissorMode();
	EndDrawing();
}

void Manager::AddPickable(int i, int j, Item* item) // might need re-implementation later
{
	Pickables[i][j] = item;
}

void Manager::RemoveBlock(int i, int j)
{
	dirtblocks[i][j] = NULL;
}

bool Manager::PlaceBlock(int i, int j, Item* item)
{
	if (!dirtblocks[i+1][j] && !dirtblocks[i][j + 1] && !dirtblocks[i - 1][j] && !dirtblocks[i][j - 1]) // cant place tile in space
		return false;
	if (item->GetItemType() != Block)
		return false;
	if (!dirtblocks[i][j])
	//{
	//	dirtblocks[i][j]->setState(Mined);
	//	//AddPickable(i, j, dirtblocks[i][j]);
	//	dirtblocks[i][j] = item;
	//	item->setState(Placed);
	//}
	//else
	{
		item->setState(Placed);
		item->setPos(j * blockWidth + minPoint.x, i * blockHeight + minPoint.y);
		dirtblocks[i][j] = item;
		return true;
	}
	return false;
}

bool Manager::PlaceBlock(Vector2 pos, Item* item)
{
	return PlaceBlock(pos.y,pos.x,item);
}

Vector2 Manager::GetMinPoint()
{
	return minPoint;
}

Vector2 Manager::GetMaxPoint()
{
	return maxPoint;
}

vector<vector<Item*>>::const_iterator  Manager::GetDirtBlocks()
{
	return dirtblocks.begin();
}

vector<vector<Item*>>::const_iterator Manager::GetPickables()
{
	return Pickables.begin();
}

const Player* Manager::GetPlayer()
{
	return &player;
}

Manager::~Manager()
{

	for (int i = 0; i < 112; i++)
	{
		for (int j = 0; j < 344; j++)
		{
			if (dirtblocks[i][j])
				delete dirtblocks[i][j];
		}
	}

}
