#pragma once
#include "Item.h"

class Bow :
    public Item
{
    float Damage;
    float FireSpeed;
    float FiringTimer;
    float timer;
    int frame;
    bool animate;
    float Yspeed;
public:
    Bow(UIInfo* p, Vector2 position);

    virtual void UpdateItem(Manager* pMmanager);
    virtual void DrawItem(int rotation, PlayerOrientaion orientation, ItemState State, Vector2 pos = { 0,0 });
    virtual void DrawName(Vector2 pos);

    virtual bool UseItem(Manager* pMmanager);
};

