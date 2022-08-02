#include "..\Header files\Manager.h"
#include "raylib.h"




int main() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 400, "trial");
	Texture2D tex = LoadTexture("textures/all tiles.png");
	UIInfo ui_Info;
	ui_Info.dirtDes = string{ "Block can be placed" };
	ui_Info.dirtName = string{ "DirtBlock" };
	ui_Info.dirtTex = LoadTexture("textures/all tiles.png");
	ui_Info.player = LoadTexture("textures/NPC Sprites/LesanUrchinMan.png");


	Dirt dirt(&ui_Info, Block);
	//Dirt dirt(tex, Block);
	Manager manager(&dirt);
	SetTargetFPS(60);
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






//int main() {
//	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
//	InitWindow(800, 400, "trial");
//	SetTargetFPS(60);
//
//
//	UIInfo ui_Info;
//	ui_Info.dirtDes = string{ "Block can be placed" };
//	ui_Info.dirtName = string{ "DirtBlock" };
//	ui_Info.dirtTex = LoadTexture("textures/all tiles.png");
//	ui_Info.player = LoadTexture("textures/NPC Sprites/LesanUrchinMan.png");
//
//
//	Dirt dirt(&ui_Info, Block);
//	Manager manager(&ui_Info);
//
//
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
//}












