#include "..\Header files\Manager.h"
#include "../SimpleNoise/SimplexNoise.h"
//#include <iostream>
//using namespace std;

Manager::Manager(UIInfo* p) :player(p)
{
	pUI = p;

	minPoint = Vector2{ -WorldWidth / 2 * pUI->blockWidth ,-WorldHeight / 2 * pUI->blockHeight };
	maxPoint = Vector2{ WorldWidth / 2 * pUI->blockWidth ,WorldHeight / 2 * pUI->blockHeight };

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
	camera.zoom = 1;
}

void Manager::Update(int WindowWidth, int WindowHeight)
{

	player.Update(this);
	UpdateCam(WindowWidth, WindowHeight);
	if (player.getSpeedX() < 0) {
		scrollingBack1 += 0.3f;
		scrollingBack2 += 0.5f;
		scrollingBack3 += 0.7f;
		scrollingBack4 += 1.0f;
		scrollingBack5 += 1.4f;
	}
	else if (player.getSpeedX() > 0) {
		scrollingBack1 -= 0.3f;
		scrollingBack2 -= 0.5f;
		scrollingBack3 -= 0.7f;
		scrollingBack4 -= 1.0f;
		scrollingBack5 -= 1.4f;
	}
	 // went too far to left
	if (scrollingBack1 <= -pUI->background1.width * WindowWidth / pUI->background1.width) scrollingBack1 = 0;
	if (scrollingBack2 <= -pUI->background2.width * WindowWidth / pUI->background2.width) scrollingBack2 = 0;
	if (scrollingBack3 <= -pUI->background3.width * WindowWidth / pUI->background3.width) scrollingBack3 = 0;
	if (scrollingBack4 <= -pUI->background4.width * WindowWidth / pUI->background4.width) scrollingBack4 = 0;
	if (scrollingBack5 <= -pUI->background5.width * WindowWidth / pUI->background5.width) scrollingBack5 = 0;

	//too far to right
	if (scrollingBack1 >= pUI->background1.width * WindowWidth / pUI->background1.width) scrollingBack1 = 0;
	if (scrollingBack2 >= pUI->background2.width * WindowWidth / pUI->background2.width) scrollingBack2 = 0;
	if (scrollingBack3 >= pUI->background3.width * WindowWidth / pUI->background3.width) scrollingBack3 = 0;
	if (scrollingBack4 >= pUI->background4.width * WindowWidth / pUI->background4.width) scrollingBack4 = 0;
	if (scrollingBack5 >= pUI->background5.width * WindowWidth / pUI->background5.width) scrollingBack5 = 0;
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


// function by OneLoneCoder
void PerlinNoise1D(int nCount, float* fSeed, int nOctaves, float fBias, float* fOutput)
{
	// Used 1D Perlin Noise
	for (int x = 0; x < nCount; x++)
	{
		float fNoise = 0.0f;
		float fScaleAcc = 0.0f;
		float fScale = 1.0f;

		for (int o = 0; o < nOctaves; o++)
		{
			int nPitch = nCount >> o;
			int nSample1 = (x / nPitch) * nPitch;
			int nSample2 = (nSample1 + nPitch) % nCount;

			float fBlend = (float)(x - nSample1) / (float)nPitch;

			float fSample = (1.0f - fBlend) * fSeed[nSample1] + fBlend * fSeed[nSample2];

			fScaleAcc += fScale;
			fNoise += fSample * fScale;
			fScale = fScale / fBias;
		}

		// Scale to seed range
		fOutput[x] = fNoise / fScaleAcc;
	}
}


void Manager::GenerateWorld()
{
	float arr[WorldWidth];
	float seed = rand();

	//float fNoiseSeed1D[344];
	//for (int i = 0; i < 344; i++) fNoiseSeed1D[i] = (float)rand() / (float)RAND_MAX;
	//PerlinNoise1D(344, fNoiseSeed1D, 4, 0.4, arr); //6, 0.6
	
	//for (int i = 0; i < 344; i++) {
	//	arr[i] = (int)(arr[i] * 800) / 16 * 16;
	//}
	//int test = 0;
	float smoothness = 320.0f;
	float modifier = 0.01;
	for (int x = 0; x < WorldWidth; x++) {
		//if (test % 80 == 0)
		//	smoothness = 10;
		//else
		//	smoothness = 320;
		arr[x] = (int)(SimplexNoise::noise(x / smoothness, seed)* 600 / 16) * 16;
	}

	int first = 0;

	for (int i = 0; i < (maxPoint.y - minPoint.y) / pUI->blockHeight /*112*/; i++)
	{
		vector<Dirt*> temp;
		for (int j = 0; j < (maxPoint.x - minPoint.x) / pUI->blockWidth /*344*/; j++)
		{
			if (i * 16 + minPoint.y < -arr[j]) /*if(j*16 -5000>0)*/
				temp.push_back(NULL);
			else
				if (first < 51 && j * 16 + minPoint.x > 0) // mined blocks to test picking up 
				{
					Dirt* d = new Dirt(pUI, Block, Vector2{ (float)j * 16 + minPoint.x, (float)i * 16 + minPoint.y });
					d->setState(Mined);
					temp.push_back(d);
					first++;
				}
				else
				{
					if (round(SimplexNoise::noise(j * modifier + seed, i * modifier + seed)) == 0 && i*16 + minPoint.y > 0) {
						temp.push_back(NULL);
					}
					else {
						Dirt* d = new Dirt(pUI, Block, Vector2{ (float)j * 16 + minPoint.x, (float)i * 16 + minPoint.y });
						d->setState(Placed);
						temp.push_back(d);
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

bool isSurfaceTile(int row , int column) {
	// should be redone when a propper 2D array/vector system is implemented to represent the world
	return row > 0;
}

void Manager::Draw(int WindowWidth, int WindowHeight)
{
	BeginDrawing();
	ClearBackground(LIGHTGRAY);

	float backgroundWidth1 = pUI->background1.width * WindowWidth / pUI->background1.width + WindowWidth * 0.25;
	float backgroundWidth2 = pUI->background2.width * WindowWidth / pUI->background2.width;
	float backgroundWidth3 = pUI->background3.width * WindowWidth / pUI->background3.width;
	float backgroundWidth4 = pUI->background4.width * WindowWidth / pUI->background4.width;
	float backgroundWidth5 = pUI->background5.width * WindowWidth / pUI->background5.width;

	DrawTexturePro(pUI->background1, Rectangle{ 0,0,(float)pUI->background1.width, (float)pUI->background1.height }, Rectangle{ scrollingBack1, 0, backgroundWidth1, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
	DrawTexturePro(pUI->background1, Rectangle{ 0,0,(float)pUI->background1.width, (float)pUI->background1.height }, Rectangle{ scrollingBack1 + backgroundWidth1, 0, backgroundWidth1, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
	DrawTexturePro(pUI->background1, Rectangle{ 0,0,(float)pUI->background1.width, (float)pUI->background1.height }, Rectangle{ scrollingBack1 - backgroundWidth1, 0, backgroundWidth1, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);



	DrawTexturePro(pUI->background2, Rectangle{ 0,0,(float)pUI->background2.width, (float)pUI->background2.height }, Rectangle{ scrollingBack2, 0, backgroundWidth2, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
	DrawTexturePro(pUI->background2, Rectangle{ 0,0,(float)pUI->background2.width, (float)pUI->background2.height }, Rectangle{ scrollingBack2 + backgroundWidth2, 0, backgroundWidth2, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
	DrawTexturePro(pUI->background2, Rectangle{ 0,0,(float)pUI->background2.width, (float)pUI->background2.height }, Rectangle{ scrollingBack2 - backgroundWidth2, 0, backgroundWidth2, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);


	DrawTexturePro(pUI->background3, Rectangle{ 0,0,(float)pUI->background3.width, (float)pUI->background3.height }, Rectangle{ scrollingBack3, 0, backgroundWidth3, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
	DrawTexturePro(pUI->background3, Rectangle{ 0,0,(float)pUI->background3.width, (float)pUI->background3.height }, Rectangle{ scrollingBack3 + backgroundWidth3, 0, backgroundWidth3, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
	DrawTexturePro(pUI->background3, Rectangle{ 0,0,(float)pUI->background3.width, (float)pUI->background3.height }, Rectangle{ scrollingBack3 - backgroundWidth3, 0, backgroundWidth3, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);


	DrawTexturePro(pUI->background4, Rectangle{ 0,0,(float)pUI->background4.width, (float)pUI->background4.height }, Rectangle{ scrollingBack4, 0, backgroundWidth4, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
	DrawTexturePro(pUI->background4, Rectangle{ 0,0,(float)pUI->background4.width, (float)pUI->background4.height }, Rectangle{ scrollingBack4 + backgroundWidth4, 0, backgroundWidth4, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
	DrawTexturePro(pUI->background4, Rectangle{ 0,0,(float)pUI->background4.width, (float)pUI->background4.height }, Rectangle{ scrollingBack4 - backgroundWidth4, 0, backgroundWidth4, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);


	DrawTexturePro(pUI->background5, Rectangle{ 0,0,(float)pUI->background5.width, (float)pUI->background5.height }, Rectangle{ scrollingBack5, 0, backgroundWidth5, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
	DrawTexturePro(pUI->background5, Rectangle{ 0,0,(float)pUI->background5.width, (float)pUI->background5.height }, Rectangle{ scrollingBack5 + backgroundWidth5, 0, backgroundWidth5, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
	DrawTexturePro(pUI->background5, Rectangle{ 0,0,(float)pUI->background5.width, (float)pUI->background5.height }, Rectangle{ scrollingBack5 - backgroundWidth5, 0, backgroundWidth5, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);



	BeginMode2D(camera);

	
	Vector2 pos = player.getPos();


	for (int i = ((int)(pos.y - minPoint.y )/16)  - WindowHeight /8 ; i < ((int)(pos.y - minPoint.y) / 16) + WindowHeight / 8 ; i++)
	{
		for (int j = ((int)(pos.x - minPoint.x) / 16)  - WindowWidth / 16 ; j < (int)(pos.x - minPoint.x)/16  + WindowWidth / 16 ; j++) // 
		{
			if (i < dirtblocks.size() && j < dirtblocks[i].size() && dirtblocks[i][j])
				dirtblocks[i][j]->DrawItem(0, Right, Placed);
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

	player.drawInv();

	EndDrawing();
}

void Manager::RemoveBlock(int i, int j)
{
	dirtblocks[i][j] = NULL;
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
