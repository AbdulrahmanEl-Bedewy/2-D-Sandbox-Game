#pragma once
#include "Item.h"
#include "Dirt.h"
#include "Player.h"
#include <vector>



class Manager
{
	UIInfo* pUI;
	Texture2D DirtTex;
	//Dirt* dirt;
	//vector<Vector2> dirtblocks;
	Camera2D camera;
	Vector2 minPoint;
	Vector2 maxPoint;
	Player player;
	vector<vector<Item*>> dirtblocks; // change name to blocks
	vector<vector<int>> wall;		 // change to vector<vector<**SomeType WAll***>> when dealing with multiple biomes and walls
	vector<vector<Item*>> Pickables; // maybe change it to vector<vector<vector<Item*>>> to have multiple items at same location
									 // at each coordinate a vector of items that can be picked
	
	// for handling background movement
	float scrollingBack1;
	float scrollingBack2;
	float scrollingBack3;
	float scrollingBack4;
	float scrollingBackSun_Moon; 
	
	bool Day; //day or night

	/*void UpdatePlayer(int WindowWidth, int WindowHeight);*/
	void UpdateCam(int WindowWidth, int WindowHeight);
	void GenerateWorld();

public:
	Manager(UIInfo* p);

	void Update(int WindowWidth, int WindowHeight);
	void Draw(int WindowWidth, int WindowHeight);
	void AddPickable(int i,int j,Item* item);
	void RemoveBlock(int i, int j);
	bool PlaceBlock(int x, int y,Item* item); //
	bool PlaceBlock(Vector2 pos, Item* item);

	Vector2 GetMinPoint();
	Vector2 GetMaxPoint();
	vector<vector<Item*>>::const_iterator GetDirtBlocks();
	vector<vector<Item*>>::const_iterator GetPickables();
	const Player* GetPlayer();


	bool isSurfaceTile(int row, int column);

	// from world x-y coordinates to world blocks coodinate
	Vector2 GetCoordinate(int x, int y);
	Vector2 GetCoordinate(Vector2 p);

	// from screen coordinates to world blocks coodinate
	Vector2 GetCoordinateFromScreen(int x, int y);
	Vector2 GetCoordinateFromScreen(Vector2 p);
	Vector2 GetWorldXY(Vector2 pos);

	~Manager();
};