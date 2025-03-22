#pragma once
class SlotIcon;

class InventorySlot : public RectCollider
{
public:
	enum SlotState
	{
		NORMAL, HOVERED ,CLICK, HOLD, PUSH, DROP, REST
	};

	const Float4 NORMAL_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
	const Float4 HOVERED_COLOR = { 0.9f, 0.9f, 0.9f, 0.5f };
	const Float4 CLICK_COLOR = { 0.5f, 0.5f, 0.5f, 1.0f };

public:
	InventorySlot();
	~InventorySlot();

	void Update();
	void Render();

	void SetItem(UINT key, UINT count);
	void DecreaseItem(UINT key, UINT count);

	UINT GetKey() { return slotKey; }
	int GetCount() { return itemCount; }
	
	void SetSlotState(SlotState newState);
	SlotIcon* GetIcon() { return icon; }

	bool IsChanged() { return isChanged; }
	void SetChanged(bool isChanged) { this->isChanged = isChanged; }
	bool IsHold() { return isHold; }
	bool IsPush() { return isPush; }
	bool IsPressShift() { return isPressShift; }

	void SetRest(bool isRest) { this->isRest = isRest; }

protected:
	bool isChanged = false;
	UINT slotKey = 0;
	UINT itemCount = 0;

private:
	bool isHold = false;
	bool isPush = false;
	bool isPressShift = false;
	bool isRest = false;

	SlotState state = REST;

	Quad* slot;
	SlotIcon* icon;
};