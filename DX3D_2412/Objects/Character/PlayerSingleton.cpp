#include "Framework.h"

PlayerSingleton::PlayerSingleton()
{
	player = new Player("SteveRigged");
}

PlayerSingleton::~PlayerSingleton()
{
	delete player;
}

void PlayerSingleton::Update()
{
	player->Update();
}

void PlayerSingleton::Render()
{
	player->Render();
}
