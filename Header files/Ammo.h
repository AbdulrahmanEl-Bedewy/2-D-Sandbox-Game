#pragma once
#include "Item.h"
class Ammo :
    public Item
{
protected:
    float Damage;
    int FireSpeed;
    bool Fired;
    float Yspeed;
    float Xspeed;
    bool hit;
    float timer; // to disapear after hitting
public:
    virtual bool Hit() = 0;
    virtual void ApplyEffect() = 0;
};

