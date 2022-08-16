#pragma once
#include "Item.h"
#include "Dirt.h"
#include "Player.h"
#include <vector>
#include <unordered_set>
#include <unordered_map>

class Ammo;

class Manager
{
	UIInfo* pUI;
	Texture2D DirtTex;
	//Dirt* dirt;
	//vector<Vector2> dirtblocks;
	Camera2D camera;
	Vector2 minPoint;
	Vector2 maxPoint;
	Vector2 SpawnPoint;
	Player player;
	std::vector<std::vector<Item*>> dirtblocks; // change name to blocks
	std::vector<std::vector<int>> wall;		 // change to vector<vector<**SomeType WAll***>> when dealing with multiple biomes and walls
	std::unordered_map <int, std::vector<Item*>> Pickables; // maybe change it to vector<vector<vector<Item*>>> to have multiple items at same location
									 // at each coordinate a vector of items that can be picked
	std::unordered_set<Ammo*> FiredAmmo;

	// for handling background movement
	float scrollingBack1;
	float scrollingBack2;
	float scrollingBack3;
	float scrollingBack4;
	float scrollingBackSun_Moon; 
	
	bool Day; //day or night

	/*void UpdatePlayer(int WindowWidth, int WindowHeight);*/
	void UpdateCam(int WindowWidth, int WindowHeight);
	void GenerateWorld(int* BlocksFinished);

public:
	Manager(UIInfo* p);

	void Update(int WindowWidth, int WindowHeight);
	void Draw(int WindowWidth, int WindowHeight);
	void DrawBackground(int WindowWidth, int WindowHeight);
	void AddPickable(int i,int j,Item* item);
	void RemovePickable(int i, int j, Item* item);
	void RemoveBlock(int i, int j);
	bool PlaceBlock(int x, int y,Item* item); //
	bool PlaceBlock(Vector2 pos, Item* item);
	void AddFiredAmmo(Ammo* ammo);
	void RemoveFiredAmmo(Ammo* ammo);

	Vector2 GetMinPoint();
	Vector2 GetMaxPoint();
	std::vector< std::vector< Item* > >::const_iterator GetDirtBlocks();
	std::unordered_map <int, std::vector<Item*>>* GetPickables();
	Player* GetPlayer();


	bool isSurfaceTile(int row, int column);

	// from world x-y coordinates to world blocks coodinate
	Vector2 GetCoordinate(int x, int y);
	Vector2 GetCoordinate(Vector2 p);

	// from screen coordinates to world blocks coodinate
	Vector2 GetCoordinateFromScreen(int x, int y);
	Vector2 GetCoordinateFromScreen(Vector2 p);
	Vector2 GetWorldXY(Vector2 pos);
	Vector2 GetScreenXY(Vector2 pos);

	~Manager();
};