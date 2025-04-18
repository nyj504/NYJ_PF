#pragma once
class InventorySingleton : public Singleton<InventorySingleton>
{
private:
	friend class Singleton;

	InventorySingleton();
	~InventorySingleton();

public:
	void Update();
	void PreRender();
	void Render();

	Inventory* GetInventory() { return inventory; }

private:
	Inventory* inventory;
};