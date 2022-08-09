#pragma once
#include "Item.h"

class Dirt :
    public Item
{
public:
    Dirt(UIInfo* p, ItemClass itemclass, Vector2 position);
    virtual void DrawItem(int rotation, PlayerOrientaion orientation, ItemState State, Vector2 pos = { 0,0 });
    virtual void DrawText(Vector2 pos);
};