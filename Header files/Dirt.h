#pragma once
#include "Item.h"

class Dirt :
    public Item
{
public:
    Dirt(UIInfo* p, ItemClass);
    virtual void DrawItem(Vector2 pos, int rotation, PlayerOrientaion orientation, ItemState State);
    virtual void DrawText(Vector2 pos);
};

