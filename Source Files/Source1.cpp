#include "..\Header files\Manager.h"
#include "raylib.h"


int main() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 400, "trial");
	SetTargetFPS(60);


	UIInfo ui_Info;
	ui_Info.dirtDes = string{ "Block can be placed" };
	ui_Info.dirtName = string{ "DirtBlock" };
	ui_Info.dirtTex = LoadTexture("textures/grass.png");
	ui_Info.player = LoadTexture("textures/NPCSprites/LesanUrchinMan.png");
	ui_Info.background1 = LoadTexture("textures/parallax_mountain_pack/layers/parallax-mountain-bg.png");
	ui_Info.background2 = LoadTexture("textures/parallax_mountain_pack/layers/parallax-mountain-montain-far.png");
	ui_Info.background3 = LoadTexture("textures/parallax_mountain_pack/layers/parallax-mountain-mountains.png");
	ui_Info.background4 = LoadTexture("textures/parallax_mountain_pack/layers/parallax-mountain-trees.png");
	ui_Info.background5 = LoadTexture("textures/parallax_mountain_pack/layers/parallax-mountain-foreground-trees.png");
	ui_Info.blockHeight = /*28*/ 16;
	ui_Info.blockWidth = 16;

	//Dirt dirt(&ui_Info, Block);
	Manager manager(&ui_Info);


	while (!WindowShouldClose())
	{
		int height = GetScreenHeight();
		int width = GetScreenWidth();
		manager.Update(width, height);
		manager.Draw(width, height);
		//BeginDrawing();
		//ClearBackground(LIGHTGRAY);
		//DrawTextureRec(tex, Rectangle{ 0,64,32,32 }, Vector2{ 0,368 }, WHITE);
		//EndDrawing();
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