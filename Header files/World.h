#pragma once
#include"raylib.h"
#include <vector>
#include <string>


class Dirt;
struct UIInfo;

// class not used yet in manager.

class World
{
	std::vector<std::vector<Dirt*>> Blocks;
	Vector2 minPoint;
	Vector2 maxPoint;
	UIInfo* pUI;

public:
	World(UIInfo* p);

	std::vector<std::vector<Dirt*>>::const_iterator getBlocks();

	void GenerateWorld();
	void SaveWorld();
	void LoadWorld(const std::string name);

	void DrawWorld();
};

