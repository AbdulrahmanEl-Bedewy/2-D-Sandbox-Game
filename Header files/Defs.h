#pragma once
#include <string>
#include "raylib.h"
using namespace std;

#define WorldWidth 3000
#define WorldHeight 1000
#define SectorWidth 2000

enum ItemClass
{
	Block,
	MeleeWeaopon,
	RangedWeapon,
	MageWeapon
};

enum ItemState
{
	Onhand,
	Placed,
	Mined
};

enum PlayerOrientaion
{
	Left,
	Right
};

struct UIInfo
{
	string dirtName;
	string dirtDes;
	Texture2D dirtTex;
	Texture2D player;
	Texture2D mob;
	Texture2D background1;
	Texture2D background2;
	Texture2D background3;
	Texture2D background4;
	Texture2D background5;
	float blockWidth;
	float blockHeight;
};