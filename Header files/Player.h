#pragma once 
#include "raylib.h"
#include "Defs.h"
using namespace std;

class Manager;

class Player
{
	Vector2 pos;
	float health;
	float Xspeed;
	float Yspeed;
	float frameDuration;
	int walkFrame;
	PlayerOrientaion orientation;
	bool isWalking;
	bool InAir;
	UIInfo* pUI;
	Texture2D texture;

public:
	Player(UIInfo* pUI);

	Vector2 getPos();

	void Update(Manager* pManager);
	void draw();
};

