#include "../Header files/World.h"
#include "../Header files/Defs.h"
#include "../Header files/Dirt.h"
#include "../SimpleNoise/SimplexNoise.h"
#include <time.h>
#include <fstream>
#include <sstream>
using namespace std;



World::World(UIInfo* p) 
{
	pUI = p;
	/*minPoint = Vector2{ -WorldWidth / 2 * blockWidth ,-WorldHeight / 2 * blockHeight };
	maxPoint = Vector2{ WorldWidth / 2 * blockWidth ,WorldHeight / 2 * blockHeight };*/

}

std::vector<std::vector<Item*>>::const_iterator World::GetBlocks()
{
	// TODO: insert return statement here
	return Blocks.begin();
}

std::vector<std::vector<int>>& World::GetWall()
{
	// TODO: insert return statement here
	return wall;
}

Vector2 World::GetSpawnPoint()
{
	return SpawnPoint;
}

int World::GetWorldHeight()
{
	return WorldHeight;
}

int World::GetWorldWidth()
{
	return WorldWidth;
}



void World::GenerateWorld(long* BlocksFinished, WorldSize size, string Name)
{

	this->Name = Name;

	srand((unsigned int)time(NULL));
	float seed = rand();
	float smoothness = 320.0f;
	float modifier = 0.02; //0.03
	int height = 600;

	int first = 0;


	switch (size)
	{
	case Small:
		WorldHeight = Small_WorldHeight;
		WorldWidth = Small_WorldWidth;
		break;
	case Medium:
		WorldHeight = Medium_WorldHeight;
		WorldWidth = Medium_WorldWidth;
		break;
	case Large:
		WorldHeight = Large_WorldHeight;
		WorldWidth = Large_WorldWidth;
		break;
	default:
		break;
	}


	minPoint = Vector2{ -WorldWidth / 2.0f * blockWidth ,-WorldHeight / 2.0f * blockHeight };
	maxPoint = Vector2{ WorldWidth / 2.0f * blockWidth ,WorldHeight / 2.0f * blockHeight };

	for (int i = 0; i < WorldHeight; i++)
	{
		vector<Item*> temp;
		wall.push_back(vector<int>{});
		for (int j = 0; j < WorldWidth; j++)
		{
			int tempValue = (int)(SimplexNoise::noise(j / smoothness, seed) * height / blockHeight) * blockHeight;

			if (i * blockHeight + minPoint.y < -tempValue)
			{
				temp.push_back(NULL);
				wall[i].push_back(0);
			}
			else {
				wall[i].push_back(1);
				if (first == 0 && (j * blockWidth + minPoint.x >= -18 || j * blockWidth + minPoint.x <= 18))
				{

					SpawnPoint.x = j * blockWidth + minPoint.x;
					SpawnPoint.y = i * blockHeight + minPoint.y - 64;
					first++;
				}

				if ((SimplexNoise::noise(j * modifier + seed, i * modifier + seed)) < -0.3 && i * blockHeight + minPoint.y > 400 && i != WorldHeight - 1) {
					temp.push_back(NULL);
				}
				else {
					Item* d = new Dirt(pUI, Block, Vector2{ (float)j * blockWidth + minPoint.x, (float)i * blockHeight + minPoint.y });
					d->setState(Placed);
					temp.push_back(d);
				}
			}
			(*BlocksFinished)++;
		}
		Blocks.push_back(temp);
	}


}

void World::SaveWorld()
{
	ofstream Outputfile;
	Outputfile.open("Worlds/" + Name + ".txt", ios::out);

	Outputfile << WorldHeight << endl << WorldWidth << endl;
	Outputfile << SpawnPoint.x << " " << SpawnPoint.y << endl;
	for (int i = 0; i < WorldHeight; i++) {
		for (int j = 0; j < WorldWidth; j++)
		{
			if (Blocks[i][j]) {
				Outputfile << 1;
			}
			else
			{
				Outputfile << 0;
			}
			if (wall[i][j]) {
				Outputfile << 1;
			}
			else
			{
				Outputfile << 0;
			}
			Outputfile << ' ';
		}
		Outputfile << endl;
	}
	Outputfile.close();
}

void World::LoadWorld(int* Progress,const std::string name)
{
	ifstream inputfile;
	inputfile.open("Worlds/" + name, ios::in);
	Name = name;

	inputfile >> WorldHeight >> WorldWidth;
	inputfile >> SpawnPoint.x >> SpawnPoint.y;
	minPoint = Vector2{ -WorldWidth / 2.0f * blockWidth ,-WorldHeight / 2.0f * blockHeight };
	maxPoint = Vector2{ WorldWidth / 2.0f * blockWidth ,WorldHeight / 2.0f * blockHeight };
	for (int i = 0; i < WorldHeight; i++)
	{
		wall.push_back(vector<int>{});
		vector<Item*> temp;
		for (int j = 0; j < WorldWidth; j++) {
			int dummy;
			inputfile >> dummy;
			Item* d = NULL;
			switch (dummy)
			{
			case 00:
				temp.push_back(NULL);
				wall[i].push_back(0);
				break;
			case 01:
				temp.push_back(NULL);
				wall[i].push_back(1);
				break;
			case 10:
				d = new Dirt(pUI, Block, Vector2{ (float)j * blockWidth + minPoint.x, (float)i * blockHeight + minPoint.y });
				d->setState(Placed);
				temp.push_back(d);
				wall[i].push_back(0);
				break;
			case 11:
				d = new Dirt(pUI, Block, Vector2{ (float)j * blockWidth + minPoint.x, (float)i * blockHeight + minPoint.y });
				d->setState(Placed);
				temp.push_back(d);
				wall[i].push_back(1);
				break;
			default:
				break;
			}
			(*Progress)++;
		}

		Blocks.push_back(temp);
	}

	//int i = 0, j = 0;

	//string temp;
	//while(getline(inputfile,temp)){
	//	stringstream iss(temp);
	//	vector<Item*> temp;
	//	wall.push_back(vector<int>{});
	//	int dummy;

	//	while (iss >> dummy)
	//	{
	//		//inputfile >> dummy;
	//		Item* d = NULL;
	//		switch (dummy)
	//		{
	//		case 00:
	//			temp.push_back(NULL);
	//			wall[i].push_back(0);
	//			break;
	//		case 01:
	//			temp.push_back(NULL);
	//			wall[i].push_back(1);
	//			break;
	//		case 10:
	//			d = new Dirt(pUI, Block, Vector2{ (float)j * blockWidth + minPoint.x, (float)i * blockHeight + minPoint.y });
	//			d->setState(Placed);
	//			temp.push_back(d);
	//			wall[i].push_back(0);
	//			break;
	//		case 11:
	//			d = new Dirt(pUI, Block, Vector2{ (float)j * blockWidth + minPoint.x, (float)i * blockHeight + minPoint.y });
	//			d->setState(Placed);
	//			temp.push_back(d);
	//			wall[i].push_back(1);
	//			break;
	//		default:
	//			break;
	//		}
	//			j++;

	//	}
	//		i++; j = 0;

	//	Blocks.push_back(temp);
	//}
	//WorldHeight = i;
	//WorldWidth = i;

	inputfile.close();
}

bool World::PlaceBlock(int i, int j, Item* item)
{
	if (!Blocks[i + 1][j] && !Blocks[i][j + 1] && !Blocks[i - 1][j] && !Blocks[i][j - 1]) // cant place tile in space
		return false;
	if (item->GetItemType() != Block)
		return false;
	if (!Blocks[i][j])
	{
		item->setState(Placed);
		item->setPos(j * blockWidth + minPoint.x, i * blockHeight + minPoint.y);
		Blocks[i][j] = item;
		return true;
	}
	return false;
}

void World::RemoveBlock(int i, int j)
{
	Blocks[i][j]->setPos(j * blockWidth + minPoint.x, i * blockHeight + minPoint.y);
	Blocks[i][j]->setState(Mined);
	Blocks[i][j] = NULL;
}

void World::DrawWorld()
{

}

World::~World()
{
	SaveWorld();
	for (int i = 0; i < WorldHeight; i++)
	{
		for (int j = 0; j < WorldWidth; j++)
		{
			if (Blocks[i][j])
				delete Blocks[i][j];
		}
	}
}
