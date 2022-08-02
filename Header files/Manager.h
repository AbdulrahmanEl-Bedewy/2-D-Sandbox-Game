#pragma once
#include "Item.h"
#include "Dirt.h"
#include <vector>
using namespace std;

struct Nibba
{
	Rectangle rec;
	int Yspeed;
	int Xspeed;
	bool InAir;
};

class Manager
{
	Texture2D DirtTex;
	Nibba nibba;
	Dirt* dirt;
	vector<Vector2> dirtblocks;
	Camera2D camera;
	Vector2 minPoint;
	Vector2 maxPoint;

	void UpdatePlayer(int WindowWidth, int WindowHeight);
	void UpdateCam(int WindowWidth, int WindowHeight);

public:
	Manager(Dirt* d);

	void Update(int WindowWidth, int WindowHeight);
	void Draw(int WindowWidth, int WindowHeight);
};

