#pragma once
#include "Ammo.h"
class Arrow :
    public Ammo
{
 
public:
    Arrow(UIInfo* p, Vector2 position);

    virtual void UpdateItem(Manager* pMmanager);
    virtual void DrawItem(int rotation, PlayerOrientaion orientation, ItemState State, Vector2 pos = { 0,0 });
    virtual void DrawName(Vector2 pos);

    virtual bool Hit();
    virtual void ApplyEffect();

    virtual bool UseItem(Manager* pMmanager);
};

