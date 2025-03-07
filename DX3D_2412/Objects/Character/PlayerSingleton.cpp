#include "Framework.h"

PlayerSingleton::PlayerSingleton()
{
	player = new Player();
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
