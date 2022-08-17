#include "..\Header files\Manager.h"
#include "../SimpleNoise/SimplexNoise.h"
#include "../Header files/Ammo.h"
#include <thread>
//#include <iostream>
using namespace std;

Manager::Manager(UIInfo* p) :player(p)
{
	pUI = p;

	minPoint = Vector2{ -WorldWidth / 2 * blockWidth ,-WorldHeight / 2 * blockHeight };
	maxPoint = Vector2{ WorldWidth / 2 * blockWidth ,WorldHeight / 2 * blockHeight };
	

	int Progress = 0;

	thread Worker1(&Manager::GenerateWorld, this, &Progress);

	while (Progress < WorldHeight * WorldWidth && !WindowShouldClose())
	{
		
		UpdateMusicStream(pUI->BacgroundMusic);
		BeginDrawing();
		ClearBackground(WHITE);
		DrawText("Generating World", GetScreenWidth() / 2 - MeasureText("Generating World", 40) / 2, GetScreenHeight() / 2 - 20, 40, BLACK);

		DrawRectangle(GetScreenWidth() / 2 - 200, GetScreenHeight() / 2 + 20, (float)Progress / (WorldHeight * WorldWidth) * 400, 70, RED);

		EndDrawing();
	}

	Worker1.join();




	//initialize vector
	Progress = 0;

	thread Worker2([&] (int* Progress){
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

		},&Progress);


	while (Progress < WorldHeight * WorldWidth )
	{

	
		UpdateMusicStream(pUI->BacgroundMusic);
		BeginDrawing();
		ClearBackground(WHITE);
		DrawText("Initializing Vectors", GetScreenWidth() / 2 - MeasureText("Initializing Vectors", 40) / 2, GetScreenHeight() / 2 - 20, 40, BLACK);

		DrawRectangle(GetScreenWidth() / 2 - 200, GetScreenHeight() / 2 + 20, (float)Progress / (WorldHeight * WorldWidth) * 400, 70, RED);

		EndDrawing();
	}

	Worker2.join();


	scrollingBack1 = 0.0f;
	scrollingBack2 = 0.0f;
	scrollingBack3 = 0.0f;
	scrollingBack4 = 0.0f;
	scrollingBackSun_Moon = 0.0f;

	Day = true;
	
	

	camera = { 0 };
	camera.target = Vector2{ player.GetPos().x + 20 , player.GetPos().y + 20 };;
	camera.offset = Vector2{ 800 / 2.0f, 400 / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1;

	player.SetPos(SpawnPoint);
}

void Manager::Update(int WindowWidth, int WindowHeight)
{

	player.Update(this);
	UpdateCam(WindowWidth, WindowHeight);


	//incrementing variables for background movement
	scrollingBackSun_Moon += 5* GetFrameTime();
	if (scrollingBackSun_Moon >= 1600)
	{
		Day = !Day;
		scrollingBackSun_Moon = 0;
	}

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

	for (auto it = FiredAmmo.cbegin(); it != FiredAmmo.cend() ; )
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




void Manager::GenerateWorld(int* BlocksFinished)
{
	float seed = rand();
	float smoothness = 320.0f;
	float modifier = 0.02; //0.03
	int height = 600;

	int first = 0;

	for (int i = 0; i < WorldHeight ; i++)
	{
		vector<Item*> temp;
		wall.push_back(vector<int>{});
		for (int j = 0; j < WorldWidth ; j++)
		{
			int tempValue = (int)(SimplexNoise::noise(j / smoothness, seed) * height / blockHeight) * blockHeight;

			if (i * blockHeight + minPoint.y < -tempValue) 
			{
				temp.push_back(NULL);
				wall[i].push_back(0);
			}
			else {
				wall[i].push_back(1);
				if (first == 0 && j * blockWidth + minPoint.x > 0) 
				{
					
					SpawnPoint.x = j * blockWidth + minPoint.x;
					SpawnPoint.y = i * blockHeight + minPoint.y - 64;
					first++;
				}

				if ((SimplexNoise::noise(j * modifier + seed, i * modifier + seed)) < 0.3 && i * blockHeight + minPoint.y > 400 && i != WorldHeight - 1) {
					temp.push_back(NULL);
				}
				else {
					Item* d = new Dirt(pUI, Block, Vector2{ (float)j * blockWidth + minPoint.x, (float)i * blockHeight + minPoint.y });
					d->setState(Placed);
					temp.push_back(d);
				}
			}
			(*BlocksFinished)++;
		}
		dirtblocks.push_back(temp);
	}


	
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

	



	BeginMode2D(camera);

	player.UpdateInventory(this);

	
	Vector2 CenterPos = GetScreenToWorld2D(Vector2{ WindowWidth / 2.0f,WindowHeight / 2.0f }, camera); //player.GetPos();
	

	for (int i = ((int)(CenterPos.y - minPoint.y )/blockHeight )  - WindowHeight /blockHeight * 0.5 ; i < (((CenterPos.y - minPoint.y) / blockHeight) + WindowHeight / blockHeight * 0.5) +3 ; i++)
	{
		for (int j = ((int)(CenterPos.x - minPoint.x) / blockWidth )  - WindowWidth / blockWidth * 0.5 - 3; j < ((CenterPos.x - minPoint.x)/ blockWidth + WindowWidth / blockWidth *0.5 ) +3; j++) // 
		{
			if (i < dirtblocks.size() && j < dirtblocks[i].size()) {
				if (wall[i][j])
				{
					DrawTexturePro(pUI->wall, Rectangle{ 21,75, 50,50 }, Rectangle{ (float)j * blockWidth + minPoint.x, (float)i * blockHeight + minPoint.y, blockWidth , blockHeight }, Vector2{ 0,0 }, 0.0f, WHITE);

					
				}
				if(dirtblocks[i][j])
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
	DrawFPS(WindowWidth-70, 20);
	//EndScissorMode();
	EndDrawing();
}

void Manager::DrawBackground(int WindowWidth, int WindowHeight)
{
}

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
	dirtblocks[i][j]->setPos(j * blockWidth + minPoint.x, i * blockHeight + minPoint.y);
	dirtblocks[i][j]->setState(Mined);
	dirtblocks[i][j] = NULL;
}

bool Manager::PlaceBlock(int i, int j, Item* item)
{
	if (!dirtblocks[i+1][j] && !dirtblocks[i][j + 1] && !dirtblocks[i - 1][j] && !dirtblocks[i][j - 1]) // cant place tile in space
		return false;
	if (item->GetItemType() != Block)
		return false;
	if (!dirtblocks[i][j])
	{
		item->setState(Placed);
		item->setPos(j * blockWidth + minPoint.x, i * blockHeight + minPoint.y);
		dirtblocks[i][j] = item;
		return true;
	}
	return false;
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
	return dirtblocks.begin();
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
	return SpawnPoint;
}

Manager::~Manager()
{

	for (int i = 0; i < WorldHeight; i++)
	{
		for (int j = 0; j < WorldWidth; j++)
		{
			if (dirtblocks[i][j])
				delete dirtblocks[i][j];
			if (Pickables[i][j].size() !=0)
			{
				for (int k = 0; k < Pickables[i][j].size(); k++) {
					delete Pickables[i][j][k];
				}
			}
		}
	}

}
