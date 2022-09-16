#include "..\Header files\Manager.h"
#include "raylib.h"
#include <thread>
#include <iostream>
using namespace std;


int main() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 400, "Terraria 2.0");
	MaximizeWindow();
	InitAudioDevice();
	SetTargetFPS(60);

// Loading Textures & Music Used

	UIInfo ui_Info;
	ui_Info.dirtDes = string{ "Block can be placed" };
	ui_Info.dirtName = string{ "DirtBlock" };
	ui_Info.dirtTex = LoadTexture("textures/Tiles/Tile_14.png");
	ui_Info.wall = LoadTexture("textures/Tiles/dirt2_wall.png");
	ui_Info.player = LoadTexture("textures/player/Conrad.png");
	ui_Info.Morningbackground1 = LoadTexture("textures/Backgrounds/Backgound hills/_PNG/background1.png");
	ui_Info.Morningbackground2 = LoadTexture("textures/Backgrounds/Backgound hills/_PNG/background2.png");
	ui_Info.Morningbackground3 = LoadTexture("textures/Backgrounds/Backgound hills/_PNG/background3.png");
	ui_Info.Morningbackground4 = LoadTexture("textures/Backgrounds/Backgound hills/_PNG/background3.png");
	ui_Info.Sun = LoadTexture("textures/Backgrounds/Backgound hills/_PNG/sun3.png");
	ui_Info.Nightbackground1 = LoadTexture("textures/Backgrounds/background_set/background1.png");
	ui_Info.Nightbackground2 = LoadTexture("textures/Backgrounds/background_set/background2.png");
	ui_Info.Nightbackground3 = LoadTexture("textures/Backgrounds/background_set/background3.png");
	ui_Info.Nightbackground4 = LoadTexture("textures/Backgrounds/background_set/background4.png");
	ui_Info.Moon = LoadTexture("textures/Backgrounds/background_set/Moon.png");
	ui_Info.Bow = LoadTexture("textures/player/Weapons/WEAPON_bow.png");
	ui_Info.Sword = LoadTexture("textures/player/Weapons/WEAPON_longsword.png");
	ui_Info.Arrow = LoadTexture("textures/player/Weapons/WEAPON_arrow.png");
	ui_Info.Pickaxe = LoadTexture("textures/player/Weapons/IconsPJ.png");
	ui_Info.BacgroundMusic = LoadMusicStream("Textures/Road to Dazir.mp3");


	


	Manager manager(&ui_Info);


	while (!WindowShouldClose())
	{
		int height = GetScreenHeight();
		int width = GetScreenWidth();



		manager.Update(width, height);
		if (!WindowShouldClose())
			manager.Draw(width, height);

	}

	// Unload All Loaded Textures & Music

	UnloadTexture(ui_Info.dirtTex);
	UnloadTexture(ui_Info.wall);
	UnloadTexture(ui_Info.player);
	UnloadTexture(ui_Info.Morningbackground1);
	UnloadTexture(ui_Info.Morningbackground2);
	UnloadTexture(ui_Info.Morningbackground3);
	UnloadTexture(ui_Info.Morningbackground4);
	UnloadTexture(ui_Info.Sun);
	UnloadTexture(ui_Info.Nightbackground1);
	UnloadTexture(ui_Info.Nightbackground2);
	UnloadTexture(ui_Info.Nightbackground3);
	UnloadTexture(ui_Info.Nightbackground4);
	UnloadTexture(ui_Info.Moon);

	UnloadMusicStream(ui_Info.BacgroundMusic);

	if (IsWindowReady()) {
		CloseAudioDevice();
		CloseWindow();
	}
}














//#include "..\Header files\Manager.h"
//#include "raylib.h"
//
//
//int main() {
//	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
//	InitWindow(800, 400, "trial");
//	Texture2D tex = LoadTexture("textures/all tiles.png");
//	Dirt dirt(tex,Block);
//	Manager manager(&dirt);
//	SetTargetFPS(60);
//	while (!WindowShouldClose())
//	{	
//		int height = GetScreenHeight();
//		int width = GetScreenWidth();
//		manager.Update(width, height);
//		manager.Draw(width, height);
//		//BeginDrawing();
//		//ClearBackground(LIGHTGRAY);
//		//DrawTextureRec(tex, Rectangle{ 0,64,32,32 }, Vector2{ 0,368 }, WHITE);
//		//EndDrawing();
//	}
// }