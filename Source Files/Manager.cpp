#include "..\Header files\Manager.h"
#include "../SimpleNoise/SimplexNoise.h"
#include "../Header files/Ammo.h"
#include "../Header files/World.h"
#include <thread>
//#include <iostream>
using namespace std;

Manager::Manager(UIInfo* p) : player(p)
{
	pUI = p;

	scrollingBack1 = 0.0f;
	scrollingBack2 = 0.0f;
	scrollingBack3 = 0.0f;
	scrollingBack4 = 0.0f;
	scrollingBackSun_Moon = 0.0f;

	Day = true;

	Menu[0] = "Load World";
	Menu[1] = "Generate World";
	Menu[2] = "Setting";
	Menu[3] = "Exit";

	Sizes[0] = "Small";
	Sizes[1] = "Medium";
	Sizes[2] = "Large";

	PauseMenu[0] = "Resume";
	PauseMenu[1] = "Settings";
	PauseMenu[2] = "Exit";

	SettingsMenu[0] = "Music";

	Name = "";

	ChosenSize = -1;
	
	screenstate = Main_Menu;

	

	camera = { 0 };
	//camera.target = Vector2{ player.GetPos().x + 20 , player.GetPos().y + 20 };;
	//camera.offset = Vector2{ 800 / 2.0f, 400 / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1;

	PlayMusicStream(pUI->BacgroundMusic);

	Music_On = true;

	
}

void Manager::Update(int WindowWidth, int WindowHeight)
{
	if(Music_On)
		UpdateMusicStream(pUI->BacgroundMusic);
	switch (screenstate)
	{
	case Main_Menu:


		scrollingBackSun_Moon += 200 * GetFrameTime();
		scrollingBack1 -= 0.3f;
		scrollingBack2 -= 0.6f;
		scrollingBack3 -= 2.1f;
		scrollingBack4 -= 3.0f;

		if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText("Load World",50) / 2,WindowHeight / 2.0f - (50 + 25), (float)MeasureText("Load World",50),50 }))
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
			{
				Worlds = GetDirectoryFiles("Worlds", &WorldCount);
				

				for (int i = 2; i < WorldCount; i++)
				{
					if (!IsFileExtension(Worlds[i], ".txt"))
					{
						char* temp = Worlds[i];
						Worlds[i] = Worlds[WorldCount - 1];
						Worlds[WorldCount - 1] = temp;
						WorldCount--;
						i--;
					}
				}
				screenstate = Worlds_List;
			}
		}


		if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText("Generate World",50) / 2,WindowHeight / 2.0f - 25, (float)MeasureText("Generate World",50),50 }))
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				screenstate = Generate_World;

			}

		}


		if (CheckCollisionPointRec(GetMousePosition(), Rectangle{  WindowWidth / 2.0f - MeasureText("Setting",50) / 2,WindowHeight / 2.0f + 25, (float)MeasureText("Setting",50),50 }))
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				Previous = Main_Menu;
				screenstate = Settings;
			}

		}

		if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText("Exit",50) / 2,WindowHeight / 2.0f + (50 + 25), (float)MeasureText("Exit",50),50 }))
		{

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				CloseWindow();
				CloseAudioDevice();
			}

		}

		break;


	case Worlds_List:


		scrollingBackSun_Moon += 200 * GetFrameTime();
		scrollingBack1 -= 0.3f;
		scrollingBack2 -= 0.6f;
		scrollingBack3 -= 2.1f;
		scrollingBack4 -= 3.0f;

		for (int i = 2; i < WorldCount; i++) {
			if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText(Worlds[i], 50) / 2, WindowHeight / 2 - 0.3f * WindowHeight + (i - 2) * 55, (float)MeasureText(Worlds[i],50),50 }))
			{
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					world = new World(pUI);
					//world->LoadWorld(Worlds[i]);
					string name = Worlds[i];
					name.erase(name.size() - 4, 4);
					int Progress = 0;
					thread Worker1(&World::LoadWorld, world, &Progress, name);

					while (Progress < WorldHeight * WorldWidth && !WindowShouldClose())
					{

						UpdateMusicStream(pUI->BacgroundMusic);
						BeginDrawing();
						ClearBackground(WHITE);
						DrawText("Loading World", GetScreenWidth() / 2 - MeasureText("Loading World", 40) / 2, GetScreenHeight() / 2 - 20, 40, BLACK);

						DrawRectangle(GetScreenWidth() / 2 - 200, GetScreenHeight() / 2 + 20, (float)Progress / (WorldHeight * WorldWidth) * 400, 70, RED);

						EndDrawing();
					}

					Worker1.join();

					dirtblocks = world->GetBlocks();
					wall = world->GetWall();
					WorldHeight = world->GetWorldHeight();
					WorldWidth = world->GetWorldWidth();

					Progress = 0;

					thread Worker2([&](int* Progress) {
						for (int i = 0; i < WorldHeight; i++)
						{
							vector<vector<Item*>> temp;
							//Pickables.push_back(vector<vector<Item*>>{});
							for (int j = 0; j < WorldWidth; j++)
							{
								temp.push_back(vector<Item*>{});
								(*Progress)++;
							}
							Pickables.push_back(temp);
						}

						}, &Progress);


					while (Progress < WorldHeight * WorldWidth)
					{


						UpdateMusicStream(pUI->BacgroundMusic);
						BeginDrawing();
						ClearBackground(WHITE);
						DrawText("Initializing Vectors", GetScreenWidth() / 2 - MeasureText("Initializing Vectors", 40) / 2, GetScreenHeight() / 2 - 20, 40, BLACK);

						DrawRectangle(GetScreenWidth() / 2 - 200, GetScreenHeight() / 2 + 20, (float)Progress / (WorldHeight * WorldWidth) * 400, 70, RED);

						EndDrawing();
					}

					Worker2.join();

					

					//	player = new Player(pUI);

					player.SetPos(world->GetSpawnPoint());
					minPoint = Vector2{ -WorldWidth / 2.0f * blockWidth ,-WorldHeight / 2.0f * blockHeight };
					maxPoint = Vector2{ WorldWidth / 2.0f * blockWidth ,WorldHeight / 2.0f * blockHeight };

					ClearDirectoryFiles();

					screenstate = Game;
				}
			}
		}
		if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText("Back", 50) / 2,  WindowHeight / 2 + 0.3f * WindowHeight + 50, (float)MeasureText("Back", 50),50 }))
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				ClearDirectoryFiles();
				screenstate = Main_Menu;
			}
		}
		
		break;


	case Loading:


		scrollingBackSun_Moon += 200 * GetFrameTime();
		scrollingBack1 -= 0.3f;
		scrollingBack2 -= 0.6f;
		scrollingBack3 -= 2.1f;
		scrollingBack4 -= 3.0f;
		break;

	case Game:


		player.Update(this);
		UpdateCam(WindowWidth, WindowHeight);


		//incrementing variables for background movement
		scrollingBackSun_Moon += 5 * GetFrameTime();

		if (player.GetSpeedX() < 0) {
			scrollingBack1 += 0.1f;
			scrollingBack2 += 0.2f;
			scrollingBack3 += 0.7f;
			scrollingBack4 += 1.0f;
		}
		else if (player.GetSpeedX() > 0) {
			scrollingBack1 -= 0.1f;
			scrollingBack2 -= 0.2f;
			scrollingBack3 -= 0.7f;
			scrollingBack4 -= 1.0f;
		}


		// updating each mined item for item falling effect when mined
		Vector2 CenterPos = GetScreenToWorld2D(Vector2{ WindowWidth / 2.0f,WindowHeight / 2.0f }, camera);
		for (int i = ((int)(CenterPos.y - minPoint.y) / blockHeight) - WindowHeight / blockHeight * 0.5; i < (((CenterPos.y - minPoint.y) / blockHeight) + WindowHeight / blockHeight * 0.5) + 3; i++)
		{
			for (int j = ((int)(CenterPos.x - minPoint.x) / blockWidth) - WindowWidth / blockWidth * 0.5 - 3; j < ((CenterPos.x - minPoint.x) / blockWidth + WindowWidth / blockWidth * 0.5) + 3; j++) // 
			{
				if (i < Pickables.size() && j < Pickables[i].size()) {

					if (Pickables[i][j].size() > 0)
						for (int k = 0; k < Pickables[i][j].size(); k++)
						{
							Pickables[i][j][k]->UpdateItem(this);
						}
				}
			}
		}

		for (auto it = FiredAmmo.cbegin(); it != FiredAmmo.cend(); )
		{
			if ((*it)->Hit())
			{
				Ammo* temp = *it;
				FiredAmmo.erase(it++);
				delete temp;
			}
			else
			{
				(*it)->UpdateItem(this);
				++it;
			}
		}
		if (IsKeyPressed(KEY_P))
		{
			screenstate = Pause;
		}
		break;


	case Pause:


		if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText("Resume",50) / 2,WindowHeight / 2.0f - 75, (float)MeasureText("Resume",50),50 }))
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				screenstate = Game;

			}

		}


		if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText("Setting",50) / 2,WindowHeight / 2.0f - 25, (float)MeasureText("Setting",50),50 }))
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				Previous = Pause;
				screenstate = Settings;
			}

		}

		if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText("Exit",50) / 2,WindowHeight / 2.0f  + 25, (float)MeasureText("Exit",50),50 }))
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				CloseWindow();
				CloseAudioDevice();
			}
		}

		break;


	case Settings:


		scrollingBackSun_Moon += 200 * GetFrameTime();
		scrollingBack1 -= 0.3f;
		scrollingBack2 -= 0.6f;
		scrollingBack3 -= 2.1f;
		scrollingBack4 -= 3.0f;

		if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText(TextFormat("Music : %s" , (Music_On) ? "On" : "Off"),50) / 2,WindowHeight / 2.0f - 75, (float)MeasureText(TextFormat("Music : %s" , (Music_On) ? "On" : "Off"),50),50 }))
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				Music_On = !Music_On;
			}
		}

		if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText("Back", 50) / 2,  WindowHeight / 2 + 0.3f * WindowHeight + 50, (float)MeasureText("Back", 50),50 }))
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				screenstate = Previous;
			}
		}
		break;


	case Generate_World:


		scrollingBackSun_Moon += 200 * GetFrameTime();
		scrollingBack1 -= 0.3f;
		scrollingBack2 -= 0.6f;
		scrollingBack3 -= 2.1f;
		scrollingBack4 -= 3.0f;

		//int nigger;
		//world->GenerateWorld(&nigger,Large);
		for (int i = 0; i < 3; i++) {
			if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText(Sizes[i], 50) / 2, WindowHeight / 2 - 0.3f * WindowHeight + i * 55, (float)MeasureText(Sizes[i], 50),50 }))
			{
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					ChosenSize = i;
				}
			}
		}

		if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText("Back", 50) / 2, WindowHeight - 0.1f * WindowHeight, (float)MeasureText("Back", 50),50 }))
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				screenstate = Main_Menu;
				Name = "";
				ChosenSize = -1;
			}
		}
		if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText("Generate", 50) / 2,  WindowHeight / 2 - 0.2f * WindowHeight + 2 * 55 + 100 + 0.03f * WindowHeight, (float)MeasureText("Generate", 50),50 }))
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				world = new World(pUI);
		//		world->GenerateWorld();
				switch ((WorldSize)ChosenSize)
				{
				case Small:
					WorldHeight = Small_WorldHeight;
					WorldWidth = Small_WorldWidth;
					break;
				case Medium:
					WorldHeight = Medium_WorldHeight;
					WorldWidth = Medium_WorldWidth;
					break;
				case Large:
					WorldHeight = Large_WorldHeight;
					WorldWidth = Large_WorldWidth;
					break;
				default:
					break;
				}

				minPoint = Vector2{ -WorldWidth / 2.0f * blockWidth ,-WorldHeight / 2.0f * blockHeight };
				maxPoint = Vector2{ WorldWidth / 2.0f * blockWidth ,WorldHeight / 2.0f * blockHeight };

				long Progress1 =0;
				long Progress2 = 0;
				
				thread Worker1(&World::GenerateWorld, world, &Progress1, (WorldSize)ChosenSize, Name);
				thread Worker2([&](long* Progress) {
					for (int i = 0; i < WorldHeight; i++)
					{
						vector<vector<Item*>> temp;
						//Pickables.push_back(vector<vector<Item*>>{});
						for (int j = 0; j < WorldWidth; j++)
						{
							temp.push_back(vector<Item*>{});
							(*Progress)++;
						}
						Pickables.push_back(temp);
					}

					}, &Progress2);

				while (Progress1 + Progress2 < 2*WorldHeight * WorldWidth && !WindowShouldClose())
				{

					UpdateMusicStream(pUI->BacgroundMusic);
					BeginDrawing();
					ClearBackground(WHITE);
					DrawText("Generating World", GetScreenWidth() / 2 - MeasureText("Generating World", 40) / 2, GetScreenHeight() / 2 - 20, 40, BLACK);

					DrawRectangle(GetScreenWidth() / 2 - 200, GetScreenHeight() / 2 + 20, (float)(Progress1 + Progress2) / (2*WorldHeight * WorldWidth) * 400, 70, RED);

					EndDrawing();
				}

				Worker1.join();
				Worker2.join();

				screenstate = Game;
				//player = new Player(pUI);
				wall = world->GetWall();
				dirtblocks = world->GetBlocks();
				player.SetPos(world->GetSpawnPoint());
			}
		}
		if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2 - 0.2f * WindowWidth, WindowHeight / 2 - 0.2f * WindowHeight + 2 * 55 + 10, 0.4f * WindowWidth, 100 }))
		{
			int key = GetCharPressed();
			while (key > 0)
			{
				// NOTE: Only allow keys in range [32..125]
				if ((key >= 32) && (key <= 125) && (Name.size() < 15))
				{
					Name.push_back((char)key);
				}

				key = GetCharPressed();  // Check next character in the queue
			}
			if (IsKeyPressed(KEY_BACKSPACE))
			{
				if (Name.size() > 0) Name.pop_back();
			}
		}
		break;
	default:
		break;
	}


	if (scrollingBackSun_Moon >= WindowWidth) // should change it so that the day change based on time passing instead of reaching end of screen
	{
		Day = !Day;
		//scrollingBackSun_Moon = 0;
	}


	switch (Day)
	{
		case 0:
			// went too far to left
			if (scrollingBack1 <= -pUI->Nightbackground1.width * WindowWidth / pUI->Morningbackground1.width) scrollingBack1 = 0;
			if (scrollingBack2 <= -pUI->Nightbackground1.width * WindowWidth / pUI->Morningbackground2.width) scrollingBack2 = 0;
			if (scrollingBack3 <= -pUI->Nightbackground1.width * WindowWidth / pUI->Morningbackground3.width) scrollingBack3 = 0;
			if (scrollingBack4 <= -pUI->Nightbackground1.width * WindowWidth / pUI->Morningbackground4.width) scrollingBack4 = 0;


			//too far to right
			if (scrollingBack1 >= pUI->Nightbackground1.width * WindowWidth / pUI->Morningbackground1.width) scrollingBack1 = 0;
			if (scrollingBack2 >= pUI->Nightbackground1.width * WindowWidth / pUI->Morningbackground2.width) scrollingBack2 = 0;
			if (scrollingBack3 >= pUI->Nightbackground1.width * WindowWidth / pUI->Morningbackground3.width) scrollingBack3 = 0;
			if (scrollingBack4 >= pUI->Nightbackground1.width * WindowWidth / pUI->Morningbackground4.width) scrollingBack4 = 0;

			if (scrollingBackSun_Moon >= pUI->Moon.width * WindowWidth / pUI->Moon.width) scrollingBackSun_Moon = 0;
			break;
		case 1:

			// went too far to left
			if (scrollingBack1 <= -pUI->Morningbackground1.width * WindowWidth / pUI->Morningbackground1.width) scrollingBack1 = 0;
			if (scrollingBack2 <= -pUI->Morningbackground2.width * WindowWidth / pUI->Morningbackground2.width) scrollingBack2 = 0;
			if (scrollingBack3 <= -pUI->Morningbackground3.width * WindowWidth / pUI->Morningbackground3.width) scrollingBack3 = 0;
			if (scrollingBack4 <= -pUI->Morningbackground4.width * WindowWidth / pUI->Morningbackground4.width) scrollingBack4 = 0;


			//too far to right
			if (scrollingBack1 >= pUI->Morningbackground1.width * WindowWidth / pUI->Morningbackground1.width) scrollingBack1 = 0;
			if (scrollingBack2 >= pUI->Morningbackground2.width * WindowWidth / pUI->Morningbackground2.width) scrollingBack2 = 0;
			if (scrollingBack3 >= pUI->Morningbackground3.width * WindowWidth / pUI->Morningbackground3.width) scrollingBack3 = 0;
			if (scrollingBack4 >= pUI->Morningbackground4.width * WindowWidth / pUI->Morningbackground4.width) scrollingBack4 = 0;

			if (scrollingBackSun_Moon >= pUI->Moon.width * WindowWidth / pUI->Moon.width) scrollingBackSun_Moon = 0;


			break;
		default:
			break;
	}

}



void Manager::UpdateCam(int WindowWidth, int WindowHeight)
{
	Vector2 pos = player.GetPos();
	camera.target=Vector2{ pos.x + 20 , pos.y + 20 };
	camera.offset = Vector2{ WindowWidth / 2.0f, WindowHeight / 2.0f };
	Vector2 max = GetWorldToScreen2D(Vector2{ maxPoint.x, maxPoint.y }, camera);
	Vector2 min = GetWorldToScreen2D(Vector2{ minPoint.x , minPoint.y }, camera);
	/*if (abs(pos.y - camera.target.y) > 10) 
		camera.target.y += 200 * GetFrameTime()* abs(pos.y - camera.target.y)/ (pos.y - camera.target.y);*/
	if (max.x < WindowWidth) camera.offset.x = WindowWidth - (max.x - WindowWidth / 2);
	if (max.y < WindowHeight) camera.offset.y = WindowHeight - (max.y - WindowHeight / 2);
	if (min.x > 0) camera.offset.x = WindowWidth / 2 - min.x;
	if (min.y > 0) camera.offset.y = WindowHeight / 2 - min.y;
}


bool Manager::isSurfaceTile(int row , int column) {
	// used to determine which tiles should be grass
	if (row == 0 ||column==0)
		return true;
	
	if (!dirtblocks[row-1][column])
		return true;

	return false;
}

bool Manager::IsBlock(Vector2 pos)
{
	if (pos.x > WorldWidth - 1 || pos.y > WorldHeight - 1 || pos.x < 0 || pos.y < 0)
	{
		return false;
	}
	return dirtblocks[pos.y][pos.x];
}

Item* Manager::GetBlock(Vector2 pos)
{
	return dirtblocks[pos.y][pos.x];
}

Vector2 Manager::GetCoordinate(int x, int y)
{
	return Vector2{ (float)((int)(x - minPoint.x) / blockWidth), (float)((int)(y - minPoint.y) / blockHeight) };
}

Vector2 Manager::GetCoordinate(Vector2 p)
{
	return  Vector2{ (float)((int)(p.x - minPoint.x) / blockWidth), (float)((int)(p.y - minPoint.y) / blockHeight) };
}

Vector2 Manager::GetCoordinateFromScreen(int x, int y)
{
	//need fix
	return Vector2{ (float)((int)(x - minPoint.x)/blockWidth ), (float)((int)(y - minPoint.y) / blockHeight ) };
}

Vector2 Manager::GetCoordinateFromScreen(Vector2 p)
{
	p = GetScreenToWorld2D(p, camera);
	return  Vector2{ (float)((int)(p.x - minPoint.x) / blockWidth ), (float)((int)(p.y - minPoint.y) / blockHeight ) };
}

Vector2 Manager::GetWorldXY(Vector2 pos)
{
	return GetScreenToWorld2D(pos,camera);
}

Vector2 Manager::GetScreenXY(Vector2 pos)
{
	return GetWorldToScreen2D(pos,camera);
}

void Manager::Draw(int WindowWidth, int WindowHeight)
{
	BeginDrawing();
	ClearBackground(LIGHTGRAY);
	//BeginScissorMode(0, 0, WindowWidth, WindowHeight);
	

	float backgroundWidth1;
	float backgroundWidth2;
	float backgroundWidth3;
	float backgroundWidth4;
	
	// drawing the background

	switch (Day)
	{
	case 0:
		backgroundWidth1 = pUI->Nightbackground1.width * WindowWidth / pUI->Nightbackground1.width + WindowWidth * 0.25;
		backgroundWidth2 = pUI->Nightbackground2.width * WindowWidth / pUI->Nightbackground2.width;
		backgroundWidth3 = pUI->Nightbackground3.width * WindowWidth / pUI->Nightbackground3.width;
		backgroundWidth4 = pUI->Nightbackground4.width * WindowWidth / pUI->Nightbackground4.width;

		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack1, 0, backgroundWidth1, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack1 + backgroundWidth1, 0, backgroundWidth1, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack1 - backgroundWidth1, 0, backgroundWidth1, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);



		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack2, 0, backgroundWidth2, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack2 + backgroundWidth2, 0, backgroundWidth2, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack2 - backgroundWidth2, 0, backgroundWidth2, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);


		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack3, 0, backgroundWidth3, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack3 + backgroundWidth3, 0, backgroundWidth3, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack3 - backgroundWidth3, 0, backgroundWidth3, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);


		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack4, 0, backgroundWidth4, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack4 + backgroundWidth4, 0, backgroundWidth4, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Nightbackground1, Rectangle{ 0,0,(float)pUI->Nightbackground1.width, (float)pUI->Nightbackground1.height }, Rectangle{ scrollingBack4 - backgroundWidth4, 0, backgroundWidth4, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);

		DrawTexturePro(pUI->Moon, Rectangle{ 0,0,(float)pUI->Moon.width, (float)pUI->Moon.height }, Rectangle{ scrollingBackSun_Moon - 60, 100, 200, 200 }, Vector2{ 0,0 }, 0.0f, WHITE);

		break;
	case 1:

		backgroundWidth1 = pUI->Morningbackground1.width * WindowWidth / pUI->Morningbackground1.width + WindowWidth * 0.25;
		backgroundWidth2 = pUI->Morningbackground2.width * WindowWidth / pUI->Morningbackground2.width;
		backgroundWidth3 = pUI->Morningbackground3.width * WindowWidth / pUI->Morningbackground3.width;
		backgroundWidth4 = pUI->Morningbackground4.width * WindowWidth / pUI->Morningbackground4.width;

		DrawTexturePro(pUI->Morningbackground1, Rectangle{ 0,0,(float)pUI->Morningbackground1.width, (float)pUI->Morningbackground1.height }, Rectangle{ scrollingBack1, 0, backgroundWidth1, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Morningbackground1, Rectangle{ 0,0,(float)pUI->Morningbackground1.width, (float)pUI->Morningbackground1.height }, Rectangle{ scrollingBack1 + backgroundWidth1, 0, backgroundWidth1, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Morningbackground1, Rectangle{ 0,0,(float)pUI->Morningbackground1.width, (float)pUI->Morningbackground1.height }, Rectangle{ scrollingBack1 - backgroundWidth1, 0, backgroundWidth1, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);


		DrawTexturePro(pUI->Sun, Rectangle{ 0,0,(float)pUI->Sun.width, (float)pUI->Sun.height }, Rectangle{ scrollingBackSun_Moon - 60, 100, 200, 200}, Vector2{ 0,0 }, 0.0f, WHITE);


		DrawTexturePro(pUI->Morningbackground2, Rectangle{ 0,0,(float)pUI->Morningbackground2.width, (float)pUI->Morningbackground2.height }, Rectangle{ scrollingBack2, 0, backgroundWidth2, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Morningbackground2, Rectangle{ 0,0,(float)pUI->Morningbackground2.width, (float)pUI->Morningbackground2.height }, Rectangle{ scrollingBack2 + backgroundWidth2, 0, backgroundWidth2, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Morningbackground2, Rectangle{ 0,0,(float)pUI->Morningbackground2.width, (float)pUI->Morningbackground2.height }, Rectangle{ scrollingBack2 - backgroundWidth2, 0, backgroundWidth2, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);


		DrawTexturePro(pUI->Morningbackground3, Rectangle{ 0,0,(float)pUI->Morningbackground3.width, (float)pUI->Morningbackground3.height }, Rectangle{ scrollingBack3, 0, backgroundWidth3, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Morningbackground3, Rectangle{ 0,0,(float)pUI->Morningbackground3.width, (float)pUI->Morningbackground3.height }, Rectangle{ scrollingBack3 + backgroundWidth3, 0, backgroundWidth3, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Morningbackground3, Rectangle{ 0,0,(float)pUI->Morningbackground3.width, (float)pUI->Morningbackground3.height }, Rectangle{ scrollingBack3 - backgroundWidth3, 0, backgroundWidth3, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);


		DrawTexturePro(pUI->Morningbackground4, Rectangle{ 0,0,(float)pUI->Morningbackground4.width, (float)pUI->Morningbackground4.height }, Rectangle{ scrollingBack4, 0, backgroundWidth4, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Morningbackground4, Rectangle{ 0,0,(float)pUI->Morningbackground4.width, (float)pUI->Morningbackground4.height }, Rectangle{ scrollingBack4 + backgroundWidth4, 0, backgroundWidth4, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
		DrawTexturePro(pUI->Morningbackground4, Rectangle{ 0,0,(float)pUI->Morningbackground4.width, (float)pUI->Morningbackground4.height }, Rectangle{ scrollingBack4 - backgroundWidth4, 0, backgroundWidth4, (float)WindowHeight }, Vector2{ 0,0 }, 0.0f, WHITE);



	default:
		break;
	}

	// drawing menus or the game
	switch (screenstate)
	{
	case Main_Menu:

		for (int i = 0; i < 4; i++)
		{

			if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText(Menu[i],50) / 2,WindowHeight / 2.0f + i*50 - 75, (float)MeasureText(Menu[i],50),50 }))
			{
				DrawText(Menu[i],WindowWidth/2 - MeasureText(Menu[i],60)/2,WindowHeight/2 + i * 50 - 75,60 ,YELLOW);
			}
			else
			{
				DrawText(Menu[i], WindowWidth / 2 - MeasureText(Menu[i], 50) / 2, WindowHeight / 2 + i * 50 - 75, 50, RAYWHITE);

			}
		}
		break;

	case Worlds_List:

		DrawRectangle( WindowWidth / 2 - 0.3*WindowWidth, WindowHeight / 2 - 0.3 * WindowHeight, 0.6 * WindowWidth, 0.6 * WindowHeight,Fade(BLUE,0.7));
		for (int i = 2; i < WorldCount; i++) {
			if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText(Worlds[i], 50) / 2, WindowHeight / 2 - 0.3f * WindowHeight + (i - 2) * 55, (float)MeasureText(Worlds[i], 50),50 }))
			{
				DrawText(Worlds[i], WindowWidth / 2 - MeasureText(Worlds[i], 60) / 2, WindowHeight / 2 - 0.3 * WindowHeight + (i - 2) * 55, 60, YELLOW);
			}
			else
			{
				DrawText(Worlds[i], WindowWidth / 2 - MeasureText(Worlds[i], 50) / 2, WindowHeight / 2 - 0.3 * WindowHeight + (i-2)*55, 50, RAYWHITE); // y value needs fixing
			}
		}

		if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText("Back", 50) / 2,  WindowHeight / 2 + 0.3f * WindowHeight + 50, (float)MeasureText("Back", 50),50 }))
		{
			DrawText("Back", WindowWidth / 2 - MeasureText("Back", 60)/2, WindowHeight / 2 + 0.3 * WindowHeight + 50, 60, YELLOW);
		}
		else
		{
			DrawText("Back", WindowWidth / 2 - MeasureText("Back", 50)/2, WindowHeight / 2 + 0.3 * WindowHeight + 50, 50, RAYWHITE); // y value needs fixing
		}
		break;

	case Loading:
		break;

	case Game:
	{
		BeginMode2D(camera);

		player.UpdateInventory(this);


		Vector2 CenterPos = GetScreenToWorld2D(Vector2{ WindowWidth / 2.0f,WindowHeight / 2.0f }, camera); //player.GetPos();

		// drawing the blocks that surrounds the center of the screen not the player since the camera is clamped at the edges 
		for (int i = ((int)(CenterPos.y - minPoint.y) / blockHeight) - WindowHeight / blockHeight * 0.5; i < (((CenterPos.y - minPoint.y) / blockHeight) + WindowHeight / blockHeight * 0.5) + 3; i++)
		{
			for (int j = ((int)(CenterPos.x - minPoint.x) / blockWidth) - WindowWidth / blockWidth * 0.5 - 3; j < ((CenterPos.x - minPoint.x) / blockWidth + WindowWidth / blockWidth * 0.5) + 3; j++) 
			{
				if (i >= 0 && j >= 0 && i < WorldHeight && j < WorldWidth) {
					if (wall[i][j])
					{
						DrawTexturePro(pUI->wall, Rectangle{ 21,75, 50,50 }, Rectangle{ (float)j * blockWidth + minPoint.x, (float)i * blockHeight + minPoint.y, blockWidth , blockHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
					}
					if (dirtblocks[i][j])
						dirtblocks[i][j]->DrawItem(0, Right, Placed);
				}
			}
		}

		// i need to draw the fallen items seperately so that they don't hidden behind other blocks 
		for (int i = ((int)(CenterPos.y - minPoint.y) / blockHeight) - WindowHeight / blockHeight * 0.5; i < (((CenterPos.y - minPoint.y) / blockHeight) + WindowHeight / blockHeight * 0.5) + 3; i++)
		{
			for (int j = ((int)(CenterPos.x - minPoint.x) / blockWidth) - WindowWidth / blockWidth * 0.5 - 3; j < ((CenterPos.x - minPoint.x) / blockWidth + WindowWidth / blockWidth * 0.5) + 3; j++) // 
			{
				if (i < WorldHeight && j < WorldWidth && i>0 && j>0)
					if (Pickables[i][j].size() > 0)
						Pickables[i][j][0]->DrawItem(0, Right, Mined);
			}
		}

		for (auto i = FiredAmmo.begin(); i != FiredAmmo.end(); i++)
		{
			(*i)->DrawItem(0, Right, Placed);
		}



		player.draw();
		EndMode2D();
		// inventory needs to be drawn here after ending 2D mode to have it at a fixed location at the top of the screen
		player.drawInv(); 
		break;
	}

	case Pause: // pretty much the same as the (Game) state 
	{
		BeginMode2D(camera);

		player.UpdateInventory(this);


		Vector2 CenterPos = GetScreenToWorld2D(Vector2{ WindowWidth / 2.0f,WindowHeight / 2.0f }, camera); //player.GetPos();


		for (int i = ((int)(CenterPos.y - minPoint.y) / blockHeight) - WindowHeight / blockHeight * 0.5; i < (((CenterPos.y - minPoint.y) / blockHeight) + WindowHeight / blockHeight * 0.5) + 3; i++)
		{
			for (int j = ((int)(CenterPos.x - minPoint.x) / blockWidth) - WindowWidth / blockWidth * 0.5 - 3; j < ((CenterPos.x - minPoint.x) / blockWidth + WindowWidth / blockWidth * 0.5) + 3; j++) // 
			{
				if (i >= 0 && j >= 0 && i < WorldHeight && j < WorldWidth) {
					if (wall[i][j])
					{
						DrawTexturePro(pUI->wall, Rectangle{ 21,75, 50,50 }, Rectangle{ (float)j * blockWidth + minPoint.x, (float)i * blockHeight + minPoint.y, blockWidth , blockHeight }, Vector2{ 0,0 }, 0.0f, WHITE);
					}
					if (dirtblocks[i][j])
						dirtblocks[i][j]->DrawItem(0, Right, Placed);
				}
			}
		}

		for (int i = ((int)(CenterPos.y - minPoint.y) / blockHeight) - WindowHeight / blockHeight * 0.5; i < (((CenterPos.y - minPoint.y) / blockHeight) + WindowHeight / blockHeight * 0.5) + 3; i++)
		{
			for (int j = ((int)(CenterPos.x - minPoint.x) / blockWidth) - WindowWidth / blockWidth * 0.5 - 3; j < ((CenterPos.x - minPoint.x) / blockWidth + WindowWidth / blockWidth * 0.5) + 3; j++) // 
			{
				if (i < WorldHeight && j < WorldWidth && i>0 && j>0)
					if (Pickables[i][j].size() > 0)
						Pickables[i][j][0]->DrawItem(0, Right, Mined);
			}
		}
		for (auto i = FiredAmmo.begin(); i != FiredAmmo.end(); i++)
		{
			(*i)->DrawItem(0, Right, Placed);
		}



		player.draw();
		EndMode2D();

		player.drawInv();
		for (int i = 0; i < 3; i++)
		{

			if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText(PauseMenu[i],50) / 2,WindowHeight / 2.0f + i * 50 - 75, (float)MeasureText(Menu[i],50),50 }))
			{
				DrawText(PauseMenu[i], WindowWidth / 2 - MeasureText(PauseMenu[i], 60) / 2, WindowHeight / 2 + i * 50 - 75, 60, YELLOW);
			}
			else
			{
				DrawText(PauseMenu[i], WindowWidth / 2 - MeasureText(PauseMenu[i], 50) / 2, WindowHeight / 2 + i * 50 - 75, 50, RAYWHITE);

			}
		}
	}
		break;


	case Settings:


		if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText(TextFormat("Music : %s" , (Music_On) ? "On" : "Off"),50) / 2,WindowHeight / 2.0f - 75, (float)MeasureText(TextFormat("Music : %s" , (Music_On) ? "On" : "Off"),50),50 }))
		{
			DrawText(TextFormat("Music : %s", (Music_On) ? "On" : "Off"), WindowWidth / 2 - MeasureText(TextFormat("Music : %s", (Music_On) ? "On" : "Off"), 60) / 2, WindowHeight / 2 - 75, 60, YELLOW);
		}
		else
		{
			DrawText(TextFormat("Music : %s" , (Music_On) ? "On" : "Off"), WindowWidth / 2 - MeasureText(TextFormat("Music : %s", (Music_On) ? "On" : "Off"), 50) / 2, WindowHeight / 2 - 75, 50, RAYWHITE);

		}

		if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText("Back", 50) / 2,  WindowHeight / 2 + 0.3f * WindowHeight + 50, (float)MeasureText("Back", 50),50 }))
		{
			DrawText("Back", WindowWidth / 2 - MeasureText("Back", 60) / 2, WindowHeight / 2 + 0.3 * WindowHeight + 50, 60, YELLOW);
		}
		else
		{
			DrawText("Back", WindowWidth / 2 - MeasureText("Back", 50) / 2, WindowHeight / 2 + 0.3 * WindowHeight + 50, 50, RAYWHITE); // y value needs fixing
		}
		break;


	case Generate_World:

		for (int i = 0; i < 3; i++) {
			if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText(Sizes[i], 50) / 2, WindowHeight / 2 - 0.3f * WindowHeight + i * 55, (float)MeasureText(Sizes[i], 50),50 }) || ChosenSize==i)
			{
				DrawText(Sizes[i], WindowWidth / 2 - MeasureText(Sizes[i], 60) / 2, WindowHeight / 2 - 0.3 * WindowHeight + i * 55, 60, YELLOW);
			}
			else
			{
				DrawText(Sizes[i], WindowWidth / 2 - MeasureText(Sizes[i], 50) / 2, WindowHeight / 2 - 0.3 * WindowHeight + i * 55, 50, RAYWHITE); // y value needs fixing
			}
		}

		DrawRectangle(WindowWidth / 2 - 0.2 * WindowWidth, WindowHeight / 2 - 0.2 * WindowHeight + 2 * 55 + 10, 0.4 * WindowWidth, 100, Fade(BLUE, 0.7));
		DrawText(Name.c_str(), WindowWidth / 2 - MeasureText(Name.c_str(), 50) / 2, WindowHeight / 2 - 0.2 * WindowHeight + 2 * 55 + 35, 50, WHITE);

		if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText("Generate", 50) / 2,  WindowHeight / 2 - 0.2f * WindowHeight + 2 * 55 + 100 + 0.03f * WindowHeight, (float)MeasureText("Generate", 50),50 }))
		{
			DrawText("Generate", WindowWidth / 2 - MeasureText("Generate", 60) / 2, WindowHeight / 2 - 0.2f * WindowHeight + 2 * 55 + 100 + 0.03f * WindowHeight, 60, YELLOW);
		}
		else
		{
			DrawText("Generate", WindowWidth / 2 - MeasureText("Generate", 50) / 2, WindowHeight / 2 - 0.2f * WindowHeight + 2 * 55 + 100  + 0.03f* WindowHeight, 50, RAYWHITE); // y value needs fixing
		}

		if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ WindowWidth / 2.0f - MeasureText("Back", 50) / 2, WindowHeight - 0.1f * WindowHeight, (float)MeasureText("Back", 50),50 }))
		{
			DrawText("Back", WindowWidth / 2 - MeasureText("Back", 60) / 2, WindowHeight - 0.1* WindowHeight, 60, YELLOW);
		}
		else
		{
			DrawText("Back", WindowWidth / 2 - MeasureText("Back", 50) / 2, WindowHeight - 0.1* WindowHeight, 50, RAYWHITE); // y value needs fixing
		}
		break;
	default:
		break;
	}

	DrawFPS(WindowWidth-70, 20);
	EndDrawing();
}

//void Manager::DrawBackground(int WindowWidth, int WindowHeight)
//{
//}

void Manager::AddPickable(int i, int j, Item* item) 
{
	if(find(Pickables[i][j].begin(), Pickables[i][j].end(),item) == Pickables[i][j].end())
		Pickables[i][j].push_back(item);	
}

void Manager::RemovePickable(int i, int j, Item* item)
{
	if (Pickables[i][j].size() > 0)
	{
		for (vector<Item*>::iterator k = Pickables[i][j].begin(); k != Pickables[i][j].end(); k++)
		{
			if (*k == item) {
				*k = nullptr;
				Pickables[i][j].erase(k);
				break;
			}
		}
	}
}

void Manager::RemoveBlock(int i, int j)
{
	world->RemoveBlock(i, j);
}

bool Manager::PlaceBlock(int i, int j, Item* item)
{
	return world->PlaceBlock(i,j,item);
}

bool Manager::PlaceBlock(Vector2 pos, Item* item)
{
	return PlaceBlock(pos.y,pos.x,item);
}

void Manager::AddFiredAmmo(Ammo* ammo)
{
	FiredAmmo.insert(ammo);
}

//void Manager::RemoveFiredAmmo(Item* ammo)
//{
//	if (FiredAmmo.find(ammo) != FiredAmmo.end()) {
//		FiredAmmo.erase(FiredAmmo.find(ammo));
//		delete ammo;
//	}
//}

Vector2 Manager::GetMinPoint()
{
	return minPoint;
}

Vector2 Manager::GetMaxPoint()
{
	return maxPoint;
}

vector<vector<Item*>>::const_iterator  Manager::GetDirtBlocks()
{
	return world->GetBlocks();
}

vector< vector<vector<Item*>>>::const_iterator Manager::GetPickables()
{
	return Pickables.begin();
}

Player* Manager::GetPlayer()
{
	return &player;
}

Vector2 Manager::GetSpawn()
{
	return world->GetSpawnPoint();
}

int Manager::GetWorldHeight()
{
	return world->GetWorldHeight();
}

int Manager::GetWorldWidth()
{
	return world->GetWorldWidth();
}

Manager::~Manager()
{
	delete world;
	for (int i = 0; i < WorldHeight; i++)
	{
		for (int j = 0; j < WorldWidth; j++)
		{
			if (Pickables[i][j].size() !=0)
			{
				for (int k = 0; k < Pickables[i][j].size(); k++) {
					delete Pickables[i][j][k];
				}
			}
		}
	}

}
