#pragma once
#include <string>
#include "Defs.h"
#include "raylib.h"

class Manager;

class Item {
protected:
	ItemClass itemClass;
	Vector2 pos;
	//string name;
	//string description;
	//int damage;
	ItemState itemstate;
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
	void setState(ItemState state);
	Vector2 GetPos();
	ItemClass GetItemType();
	ItemState GetItemState();

	virtual void UpdateItem(Manager* pMmanager) = 0;
	virtual bool UseItem(Manager* pMmanager) = 0;

	//Drawing Functions
	virtual void DrawItem(/*Vector2 pos, */int rotation, PlayerOrientaion orientation, ItemState State, Vector2 pos = {0,0}) = 0;
	virtual void DrawName(Vector2 pos) = 0;

	virtual ~Item();
};