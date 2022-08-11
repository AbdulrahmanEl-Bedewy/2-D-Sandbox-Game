#include "../Header files/World.h"
#include "../Header files/Defs.h"
#include "../Header files/Dirt.h"
#include "../SimpleNoise/SimplexNoise.h"





World::World(UIInfo* p)
{
	pUI = p;
	minPoint = Vector2{ -WorldWidth / 2 * blockWidth ,-WorldHeight / 2 * blockHeight };
	maxPoint = Vector2{ WorldWidth / 2 * blockWidth ,WorldHeight / 2 * blockHeight };
}

void World::GenerateWorld()
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
	for (int x = 0; x < WorldWidth; x++) {
		//if (test % 80 == 0)
		//	smoothness = 10;
		//else
		//	smoothness = 320;
		arr[x] = (int)(SimplexNoise::noise(x / smoothness, seed) * 600 / blockHeight) * blockHeight;
	}

	int first = 0;

	for (int i = 0; i < WorldHeight /*112*/; i++)
	{
		vector<Dirt*> temp;
		for (int j = 0; j < WorldWidth /*344*/; j++)
		{
			if (i * blockHeight + minPoint.y < -arr[j]) /*if(j*blockHeight -5000>0)*/
				temp.push_back(NULL);
			else
				if (first < 51 && j * blockHeight + minPoint.x > 0) // mined blocks to test picking up 
				{
					Dirt* d = new Dirt(pUI, Block, Vector2{ (float)j * blockHeight + minPoint.x, (float)i * blockHeight + minPoint.y });
					d->setState(Mined);
					temp.push_back(d);
					first++;
				}
				else
				{
					Dirt* d = new Dirt(pUI, Block, Vector2{ (float)j * blockHeight + minPoint.x, (float)i * blockHeight + minPoint.y });
					d->setState(Placed);
					temp.push_back(d);
				}
		}
		Blocks.push_back(temp);
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

void World::DrawWorld()
{

}
