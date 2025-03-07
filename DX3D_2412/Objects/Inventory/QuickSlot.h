#pragma once
class QuickSlot : public Quad
{
private:
	const int MAX_SLOTSIZE = 9;
public:
	QuickSlot();
	~QuickSlot();

	void Update();
	void Render();
	void CreateSlot();

	void HighlightSelectedQuickSlot();
	
	void OnMouseWheel(int delta);
	void SyncWithInventory();

	pair<UINT, UINT> GetSelectedIndexData() { return quickSlotIcons[selectedIndex]->GetItemCount(); }

private:
	int selectedIndex = 0;

	Quad* hotBarSelect;

	vector<SlotIcon*>quickSlotIcons;
	unordered_map<UINT, int> itemCounts; 

	UINT selectKey;
};