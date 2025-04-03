#pragma once
class Inventory : public Quad
{
public:
	struct InventoryItem
	{
		UINT key = 0;   
		UINT count = 0; 

		ItemData GetFullData() const
		{
			return DataManager::Get()->GetItemData(key);
		}
	};
private:
	const int MAX_SLOTSIZE = 36;
	const int MAX_INVENTORY_STORAGE = 64;
	const int INVEN_COL = 9;

public:
	Inventory();
	~Inventory();

	void Update();
	void PreRender();
	void Render();
	void CreateSlot();

	void UpdateCloneIcon();
	void Clear();

	void AddItem(UINT key, UINT count);
	void DecreaseItem(UINT key, UINT count);

	InventorySlot* FindSlotAtPosition();

	void OnSelectSlot(InventorySlot* inventorySlot);

	void TransferItem();
	bool IsExcuteCrafting() { return isExcuteCrafting; }
	bool IsRefreshQuickSlot() { return isRefreshQuickSlot; }

	void SetRefreshQuickSlot(bool isRefreshQuickSlot) { this->isRefreshQuickSlot = isRefreshQuickSlot; }
	
	SlotIcon* GetCloneIcon() { return cloneIcon; }
	pair<UINT, UINT> GetQuickSlotData(int index);

private:
	vector<InventorySlot*> slots;
	Camera* invenCamera;

	bool isExcuteCrafting = false;
	bool isFirstSwap = true;
	bool isRefreshQuickSlot = false;

	unordered_map<UINT, InventoryItem>itemDatas;
	SlotIcon* cloneIcon;

	InventorySlot* fromSlot = nullptr;
	InventorySlot* toSlot = nullptr;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	Quad* quad;
};
