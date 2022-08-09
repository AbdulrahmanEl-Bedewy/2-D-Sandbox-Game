#pragma once
#include "Item.h"
#include "Dirt.h"
#include "Player.h"
#include <vector>
using namespace std;



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
	vector<vector<Dirt*>> dirtblocks;
	/*vector<vector<Dirt*>> Pickable;*/
	float scrollingBack1;
	float scrollingBack2;
	float scrollingBack3;
	float scrollingBack4;
	float scrollingBack5;
	

	/*void UpdatePlayer(int WindowWidth, int WindowHeight);*/
	void UpdateCam(int WindowWidth, int WindowHeight);
	void GenerateWorld();

public:
	Manager(UIInfo* p);

	void Update(int WindowWidth, int WindowHeight);
	void Draw(int WindowWidth, int WindowHeight);
	void AddPickable(Item* item);
	void RemoveBlock(int i, int j);

	Vector2 getminPoint();
	Vector2 getmaxPoint();
	vector<vector<Dirt*>>::const_iterator getDirtBlocks();


	~Manager();
};