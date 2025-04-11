#include "Framework.h"
#include "InventorySlot.h"

InventorySlot::InventorySlot() 
	: RectCollider({ 32, 32 })
{
	tag = "InventorySlot";

	slot = new Quad(L"Resources/Textures/GUI/slot.png");
	slot->SetParent(this);
	
	icon = new SlotIcon();
	icon->SetParent(this);
}

InventorySlot::~InventorySlot()
{
}

void InventorySlot::Update()
{
	if (isRest)
		SetSlotState(REST);

	bool isLButtonDown = KEY->Down(VK_LBUTTON);
	bool isRButtonDown = KEY->Down(VK_RBUTTON);
	bool isShiftButtonDown = KEY->Press(VK_SHIFT);

	if (IsPointCollision(mousePos))
	{
		SetSlotState(HOVERED);
		
		if (isLButtonDown)
		{
			SetSlotState(CLICK);
	
			if (isShiftButtonDown)
			{
				isPressShift = true;
			}
			INVEN->OnSelectSlot(this);
		}
	
		if (isRButtonDown)
		{
			isPush = true;
			SetSlotState(CLICK);
			INVEN->OnSelectSlot(this);
		}
	}

	UpdateWorld();
	slot->UpdateWorld();
	icon->UpdateWorld();
}

void InventorySlot::Render()
{
	Collider::Render();
	slot->Render();
	
	if(itemCount != 0)
	icon->Render();
}

void InventorySlot::SetItem(UINT key, UINT count)
{
	if (slotKey == key)
	{
		itemCount += count;
	}
	else
	{
		slotKey = key;
		itemCount = count;
	}
	isChanged = true;

	icon->UpdateFromSlot(this);
}

void InventorySlot::DecreaseItem(UINT count)
{
	if (itemCount <= 0)
	{
		slotKey = 0;
		icon->UpdateFromSlot(this);
		isChanged = true;
		return;
	}
	if (itemCount - count < 0) return;

	itemCount -= count;
	
	icon->UpdateFromSlot(this);

	isChanged = true;
}

void InventorySlot::ConsumeItem()
{
	itemCount--;
	icon->UpdateFromSlot(this);

	isChanged = true;
}

void InventorySlot::SetSlotState(SlotState newState)
{
	this->state = newState;

	switch (state)
	{
	case NORMAL:
		slot->GetMaterial()->GetData()->diffuse = NORMAL_COLOR;
		icon->UpdateFromSlot(this);
		isRest = false;
		break;
	case HOVERED:
		slot->GetMaterial()->GetData()->diffuse = HOVERED_COLOR;
		break;
	case CLICK:
		slot->GetMaterial()->GetData()->diffuse = CLICK_COLOR;
		break;
	case HOLD:
		slot->GetMaterial()->GetData()->diffuse = CLICK_COLOR;
		isHold = true;
		break;
	case DROP:
		slot->GetMaterial()->GetData()->diffuse = NORMAL_COLOR;
		icon->GetMaterial()->GetData()->diffuse = NORMAL_COLOR;
		break;
	case REST:
		if (itemCount <= 0)
		{
			slotKey = 0;
		}
		isPush = false;
		isPressShift = false;
		isChanged = false;
		icon->UpdateFromSlot(this);
		INVEN->SetRefreshQuickSlot(true);
		SetSlotState(NORMAL);
		break;
	}
}


