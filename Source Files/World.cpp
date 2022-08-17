#include "../Header files/World.h"
#include "../Header files/Defs.h"
#include "../Header files/Dirt.h"
#include "../SimpleNoise/SimplexNoise.h"
using namespace std;




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

	
	float smoothness = 320.0f;
	for (int x = 0; x < WorldWidth; x++) {
		arr[x] = (int)(SimplexNoise::noise(x / smoothness, seed) * 600 / blockHeight) * blockHeight;
	}

	int first = 0;

	for (int i = 0; i < WorldHeight ; i++)
	{
		vector<Dirt*> temp;
		for (int j = 0; j < WorldWidth ; j++)
		{
			if (i * blockHeight + minPoint.y < -arr[j]) 
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


}

void World::DrawWorld()
{

}
