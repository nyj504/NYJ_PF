#include "Framework.h"
#include "TerrainScene.h"

TerrainScene::TerrainScene()
{
	terrain = new Terrain();
}

TerrainScene::~TerrainScene()
{
	delete terrain;
}

void TerrainScene::Update()
{
	terrain->UpdateWorld();
}

void TerrainScene::PreRender()
{
}

void TerrainScene::Render()
{
	terrain->Render();
}

void TerrainScene::PostRender()
{
}

void TerrainScene::GUIRender()
{
	terrain->GetMaterial()->Edit();
}
