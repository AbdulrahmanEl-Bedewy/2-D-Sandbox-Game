#pragma once
#include <string>
#include "Defs.h"
#include "raylib.h"
#include "Defs.h"
using namespace std;

class Item {
protected:
	ItemClass itemClass;
	Vector2 pos;
	//string name;
	//string description;
	//int damage;
	int rotation;
	UIInfo* pUI;
public:


	//Setters
	//void setTexture(Texture2D t);
	//void setName(string n);
	//void setDescription(string d);
	//void setDamage(int d);
	void setPos(Vector2 p);
	void setPos(float x, float y);

	//Drawing Functions
	virtual void DrawItem(Vector2 pos, int rotation, PlayerOrientaion orientation, ItemState State) = 0;
	virtual void DrawText(Vector2 pos) = 0;

	virtual ~Item();
};