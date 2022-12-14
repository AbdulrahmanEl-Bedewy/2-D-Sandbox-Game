#pragma once
#include "Item.h"

class Dirt :
    public Item
{
    float Yspeed;
public:
    Dirt(UIInfo* p, ItemClass itemclass, Vector2 position);

    virtual void UpdateItem(Manager* pMmanager);
    virtual void DrawItem(int rotation, PlayerOrientaion orientation, ItemState State, Vector2 pos = { 0,0 });
    virtual void DrawName(Vector2 pos);

    virtual bool UseItem(Manager* pMmanager);
};