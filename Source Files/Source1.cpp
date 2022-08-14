#include "..\Header files\Manager.h"
#include "raylib.h"
#include <thread>
using namespace std;


int main() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 400, "trial");
	SetTargetFPS(60);


	UIInfo ui_Info;
	ui_Info.dirtDes = string{ "Block can be placed" };
	ui_Info.dirtName = string{ "DirtBlock" };
	ui_Info.dirtTex = LoadTexture("textures/Tiles/Forest/Tiles/Sliced/Tile_14.png"); //LoadTexture("textures / dirt.png");
	ui_Info.wall = LoadTexture("textures/dirt2_wall.png"); //LoadTexture("textures/Text2.png");
	ui_Info.player = LoadTexture("textures/NPCSprites/LesanUrchinMan.png");
	ui_Info.Morningbackground1 = LoadTexture("textures/Backgrounds/Backgound hills/_PNG/background1.png");  //LoadTexture("textures/parallax_mountain_pack/layers/parallax-mountain-bg.png");
	ui_Info.Morningbackground2 = LoadTexture("textures/Backgrounds/Backgound hills/_PNG/background2.png");  //LoadTexture("textures/parallax_mountain_pack/layers/parallax-mountain-montain-far.png");
	ui_Info.Morningbackground3 = LoadTexture("textures/Backgrounds/Backgound hills/_PNG/background3.png");	//LoadTexture("textures/parallax_mountain_pack/layers/parallax-mountain-mountains.png");
	ui_Info.Morningbackground4 = LoadTexture("textures/Backgrounds/Backgound hills/_PNG/background3.png");	//LoadTexture("textures/parallax_mountain_pack/layers/parallax-mountain-trees.png");
	ui_Info.Sun = LoadTexture("textures/Backgrounds/Backgound hills/_PNG/sun3.png");					//LoadTexture("textures/parallax_mountain_pack/layers/parallax-mountain-foreground-trees.png");
	ui_Info.Nightbackground1 = LoadTexture("textures/Backgrounds/background_set/background1.png"); 
	ui_Info.Nightbackground2 = LoadTexture("textures/Backgrounds/background_set/background2.png");
	ui_Info.Nightbackground3 = LoadTexture("textures/Backgrounds/background_set/background3.png");
	ui_Info.Nightbackground4 = LoadTexture("textures/Backgrounds/background_set/background4.png");
	ui_Info.Moon = LoadTexture("textures/Backgrounds/background_set/Moon.png");
	ui_Info.Bow = LoadTexture("textures/player/lpc_entry/Weapons/WEAPON_bow.png");
	ui_Info.Sword = LoadTexture("textures/player/lpc_entry/Weapons/WEAPON_longsword.png");

	/*Camera2D camera;
	camera = { 0 };
	camera.target = Vector2{ 0 ,0 };;
	camera.offset = Vector2{ 800 / 2.0f, 400 / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1;*/


	//ui_Info.blockHeight = /*28*/ 22;
	//ui_Info.blockWidth = 22;

	//Dirt dirt(&ui_Info, Block);
	Manager manager(&ui_Info);


	while (!WindowShouldClose())
	{
		int height = GetScreenHeight();
		int width = GetScreenWidth();

		/*BeginDrawing();
		ClearBackground(LIGHTGRAY);

		manager.DrawBackground(width, height);
		
		BeginMode2D(camera);*/

		manager.Update(width, height);
		manager.Draw(width, height);

	/*	EndMode2D();
		EndDrawing();*/

		//BeginDrawing();
		//ClearBackground(LIGHTGRAY);
		//DrawTextureRec(tex, Rectangle{ 0,64,32,32 }, Vector2{ 0,368 }, WHITE);
		//EndDrawing();
	}

	UnloadTexture(ui_Info.dirtTex);
	UnloadTexture(ui_Info.wall);
	UnloadTexture(ui_Info.player);
	UnloadTexture(ui_Info.Morningbackground1);
	UnloadTexture(ui_Info.Morningbackground2) ;
	UnloadTexture(ui_Info.Morningbackground3);
	UnloadTexture(ui_Info.Morningbackground4);
	UnloadTexture(ui_Info.Sun);
	UnloadTexture(ui_Info.Nightbackground1);
	UnloadTexture(ui_Info.Nightbackground2);
	UnloadTexture(ui_Info.Nightbackground3);
	UnloadTexture(ui_Info.Nightbackground4);
	UnloadTexture(ui_Info.Moon);

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