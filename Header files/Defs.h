#pragma once
#include <string>
#include "raylib.h"
using namespace std;

#define WorldWidth 1500 // in blocks
#define WorldHeight 156 //in blocks
#define blockWidth 18
#define blockHeight 18
#define SectorWidth 2000
#define MaxStackSize 10

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
	Mined,
	Picked
};

enum PlayerOrientaion
{
	Left,
	Right
};

struct UIInfo
{
	//int WorldWidth;
	string dirtName;
	string dirtDes;
	Texture2D dirtTex;
	Texture2D wall;
	Texture2D player;
	Texture2D mob;
	Texture2D Bow;
	Texture2D Morningbackground1;
	Texture2D Morningbackground2;
	Texture2D Morningbackground3;
	Texture2D Morningbackground4;
	Texture2D Sun;
	Texture2D Nightbackground1;
	Texture2D Nightbackground2;
	Texture2D Nightbackground3;
	Texture2D Nightbackground4;
	Texture2D Moon;
};