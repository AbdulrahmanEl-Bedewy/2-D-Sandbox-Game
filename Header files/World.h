#pragma once
#include"raylib.h"
#include <vector>
#include <string>


class Item;
struct UIInfo;
enum WorldSize;



class World
{
	std::vector<std::vector<Item*>> Blocks;
	std::vector<std::vector<int>> wall;
	Vector2 minPoint;
	Vector2 maxPoint;
	Vector2 SpawnPoint;
	std::string Name;
	int WorldHeight;
	int  WorldWidth;
	UIInfo* pUI;

public:
	World(UIInfo* p);

	std::vector< std::vector< Item* > >::const_iterator GetBlocks();
	std::vector<std::vector<int>>& GetWall();
	Vector2 GetSpawnPoint();
	int GetWorldHeight();
	int GetWorldWidth();

	void GenerateWorld(long* BlocksFinished, WorldSize size,std::string Name);
	void SaveWorld();
	void LoadWorld(int* Progress,const std::string name);
	bool PlaceBlock(int i, int j, Item* item);
	void RemoveBlock(int i, int j);

	void DrawWorld();

	~World();
};

