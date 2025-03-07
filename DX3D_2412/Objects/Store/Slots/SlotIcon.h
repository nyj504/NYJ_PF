#pragma once

class SlotIcon : public EventQuad
{
public:
	SlotIcon();
	~SlotIcon();

	void Render();

	void UpdateFromSlot(class InventorySlot* slot);

	void Clear();
	void SetItem(UINT key, UINT count);
	
	pair<UINT, UINT>GetItemCount() { return itemCounts; }

private:
	pair<UINT, UINT>itemCounts = { 0,0 };
};