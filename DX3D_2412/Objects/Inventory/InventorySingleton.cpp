#include "Framework.h"
#include "InventorySingleton.h"

InventorySingleton::InventorySingleton()
{
	inventory = new Inventory();
}

InventorySingleton::~InventorySingleton()
{
	delete inventory;
}

void InventorySingleton::Update()
{
	inventory->Update();
}

void InventorySingleton::PreRender()
{
	inventory->PreRender();
}

void InventorySingleton::Render()
{
	inventory->Render();
}
