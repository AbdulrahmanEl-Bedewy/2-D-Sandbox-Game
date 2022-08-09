#pragma once
#include"raylib.h"
#include <vector>
#include <string>
using namespace std;

class Dirt;
struct UIInfo;

class World
{
	vector<vector<Dirt*>> Blocks;
	Vector2 minPoint;
	Vector2 maxPoint;
	UIInfo* pUI;

public:
	World(UIInfo* p);

	vector<vector<Dirt*>>::const_iterator getBlocks();

	void GenerateWorld();
	void SaveWorld();
	void LoadWorld(const string name);

	void DrawWorld();
};

