#pragma once 
#include "raylib.h"
#include "Defs.h"
#include "Inventory.h"


class Manager;

class Player
{
	Vector2 pos; // player position
	float health;
	float Xspeed;
	float Yspeed;
	float frameDuration; // the current duration a single walking animation frame has been dispayed
	int walkFrame; // the index of current walking animation frame (max. 3)
	PlayerOrientaion orientation; //player looking left or right
	bool isWalking; // if walking it animates player walking
	bool InAir; // whether the player is standing on ground or falling
	UIInfo* pUI;
	Texture2D texture;
	Inventory inventory;

public:
	Player(UIInfo* pUI);

	Vector2 GetPos() const;
	float GetSpeedX() const;
	void SetPos(Vector2 position);


	// update player movement and takes care of collisions and picking 
	void Update(Manager* pManager);
	void UpdateInventory(Manager* pManager);
	// draw player at the appropriate position and animates when walking
	void draw();
	// draws items in the inventory 
	void drawInv();
};

