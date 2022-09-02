#pragma once
#include <string>
#include "raylib.h"


#define Small_WorldWidth 150 // in blocks
#define Small_WorldHeight 150 //in blocks
#define Medium_WorldWidth 750 // in blocks
#define Medium_WorldHeight 750 //in blocks
#define Large_WorldWidth 1500 // in blocks
#define Large_WorldHeight 1500 //in blocks
#define blockWidth 18
#define blockHeight 18
#define SectorWidth 2000
#define MaxStackSize 10
#define StorageSize 50

enum ItemClass
{
	Block,
	MeleeWeaopon,
	RangedWeapon,
	MageWeapon,
	tool,
	ammo,
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

enum ScreenState {
	Worlds_List,
	Generate_World,
	Settings,
	Main_Menu,
	Loading,
	Game,
	Pause
};

enum WorldSize {
	Small,
	Medium,
	Large
};

struct UIInfo
{
	//int WorldWidth;
	std::string dirtName;
	std::string dirtDes;
	Texture2D dirtTex;
	Texture2D wall;
	Texture2D player;
	Texture2D mob;
	Texture2D Bow;
	Texture2D Arrow;
	Texture2D Sword;
	Texture2D Pickaxe;
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
	Music BacgroundMusic;
};