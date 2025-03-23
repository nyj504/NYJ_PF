#include "Framework.h"

Inventory::Inventory() : Quad((L"Resources/Textures/GUI/inventorySlot.png"))
{
	localPosition = { CENTER };
	UpdateWorld();

	tag = "Inventory";
	cloneIcon = new SlotIcon();
	cloneIcon->SetActive(false);
	CreateSlot();


	AddItem(91, 1); // 무기 실험 다이아 검 
	AddItem(68, 1); // 돌삽 
	AddItem(63, 1); // 활 
	AddItem(81, 1); // 황금곡괭이
	AddItem(59, 1); // 나무도끼
	AddItem(73, 1); 
	AddItem(89, 1);

	AddItem(1, 64);
	AddItem(2, 64);
	AddItem(3, 64);
	AddItem(4, 64);
	AddItem(5, 64);
	AddItem(24, 64);
	AddItem(7, 1);
	AddItem(8, 1);
	AddItem(9, 1);
	AddItem(10, 1);
	AddItem(11, 1);
	AddItem(65, 1);
	AddItem(35, 1);
	AddItem(37, 1);
	AddItem(38, 1);
	AddItem(93, 1);
	AddItem(74, 1);
	AddItem(86, 1);


	SetActive(false);
}

Inventory::~Inventory()
{
	for (InventorySlot* slot : slots)
		delete slot;
	delete cloneIcon;
}

void Inventory::Update()
{
	if (!isActive) return;

	for (InventorySlot* slot : slots)
	{
		slot->Update();
	}

	if (cloneIcon->IsActive() && cloneIcon->GetItemCount().second != 0)
	{
		cloneIcon->SetGlobalPosition(mousePos);
		cloneIcon->UpdateWorld();
	}
}

void Inventory::Render()
{
	if (!isActive) return;

	Quad::Render();

	for (InventorySlot* slot : slots)
		slot->Render();

	cloneIcon->Render();
}

void Inventory::CreateSlot()
{
	slots.resize(MAX_SLOTSIZE);

	int rowCount = MAX_SLOTSIZE / INVEN_COL;

	Vector2 interval = { 36, 36 };

	int row = MAX_SLOTSIZE / INVEN_COL;
	int col = MAX_SLOTSIZE % INVEN_COL;

	for (int i = 0; i < 9; i++)
	{
		Vector3 startPos = { CENTER.x - 144, CENTER.y - 47};

		InventorySlot* slot = new InventorySlot();
		slot->SetTag("SyncQuickSlot");

		Vector3 pos = { startPos.x + i * interval.x, startPos.y - row};
		slot->SetLocalPosition(pos);
		slot->UpdateWorld();

		slots[i] = slot;
	}

	for (int i = 9; i < MAX_SLOTSIZE; i++)
	{
		Vector3 startPos = { CENTER.x - 144, CENTER.y + 101};

		InventorySlot* slot = new InventorySlot();

		int row = i / INVEN_COL;
		int col = i % INVEN_COL;

		Vector3 pos = { startPos.x + col * interval.x, startPos.y - row * interval.y};

		slot->SetLocalPosition(pos);
		slot->UpdateWorld();

		slots[i] = slot;
	}
}

void Inventory::UpdateCloneIcon()
{
	if (fromSlot->IsPressShift()) return;

	if (fromSlot != nullptr && isFirstSwap)
	{
		cloneIcon->SetActive(true);
		cloneIcon->UpdateFromSlot(fromSlot);
	}
}

void Inventory::Clear()
{
	cloneIcon->SetActive(false);
	toSlot->SetRest(true);
	fromSlot->SetRest(true);
	toSlot = nullptr;
	fromSlot = nullptr;
	isFirstSwap = true;
	isExcuteCrafting = false;
}

void Inventory::AddItem(UINT key, UINT count)
{
	if (key == 0 || count == 0) return;

	for (InventorySlot* slot : slots)
	{
		if (slot->GetKey() == key)
		{
			slot->SetItem(key, count);
			if (slot->GetTag() == "SyncQuickSlot")
			{
				isRefreshQuickSlot = true;
			}
			return;
		}
		else if(slot->GetKey() == 0)
		{
			slot->SetItem(key, count);
			
			if (slot->GetTag() == "SyncQuickSlot")
			{
				isRefreshQuickSlot = true;
			}
			return;
		}	
	}
}

void Inventory::DecreaseItem(UINT key, UINT count)
{
	if (key == 0 || count == 0) return;

	for (InventorySlot* slot : slots)
	{
		slot->DecreaseItem(key, count);
		
		if (slot->GetTag() == "SyncQuickSlot")
		{
			isRefreshQuickSlot = true;
		}
	}
}

void Inventory::TransferItem()
{
	UINT fromKey = fromSlot->GetKey();
	UINT fromCount = fromSlot->GetCount();
	UINT toKey = toSlot->GetKey();
	UINT toCount = toSlot->GetCount();
	UINT cloneIconKey = cloneIcon->GetItemCount().first;
	UINT cloneIconCount = cloneIcon->GetItemCount().second;

	if (toSlot == fromSlot)
	{
		fromSlot->SetItem(0, 0);
		toSlot->SetItem(fromKey, fromCount);
		Clear();
		return;
	}

	if (toSlot->GetTag() == "CraftSlot")
	{
		if (!fromSlot->IsPressShift())
		{
			toSlot->SetItem(fromKey, fromCount);
			
			fromSlot->SetItem(0, 0);
			Clear();
			return;
		}
		else
		{
			toSlot->SetItem(cloneIconKey, cloneIconCount);
			
			Clear();
			return;
		}
	}

	if(fromSlot->IsPressShift())
		isFirstSwap = false;

	if (toKey == fromKey && !fromSlot->IsPressShift()) //같으면 더하기
	{
		toSlot->SetItem(fromKey, fromCount);
		fromSlot->SetItem(0, 0);
		Clear();
		return;
	}

	if (toKey == fromKey && fromSlot->IsPressShift()) //같으면 더하기
	{
		toSlot->SetItem(fromKey, cloneIcon->GetItemCount().second);
		Clear();	
		return;
	}	

	if (toKey == 0 && isFirstSwap) //빈슬롯이면 그 슬롯에 넣기 //쉬프트 누른애 
	{
		if (fromSlot->IsPressShift())
		{
			toSlot->SetItem(cloneIcon->GetItemCount().first, cloneIcon->GetItemCount().second);
		}

		else
		{
			toSlot->SetItem(fromKey, fromCount);
			fromSlot->SetItem(0, 0);
		}
		Clear();
		return;
	}
	if (toKey == 0 && !isFirstSwap)
	{
		toSlot->SetItem(cloneIcon->GetItemCount().first, cloneIcon->GetItemCount().second);
		Clear();
		return;
	}

	if (toKey != fromKey && toKey != 0) //서로 다른 아이템  
	{
		if (fromSlot->IsPressShift())
		{
			toSlot->SetItem(cloneIcon->GetItemCount().first, cloneIcon->GetItemCount().second);
			isFirstSwap = false;
			//fromSlot->SetRest(true);
			toSlot = nullptr;
			cloneIcon->SetItem(toKey, toCount);
			UpdateCloneIcon();
			return;
		}
		else if (isFirstSwap)
		{
			fromSlot->SetItem(0, 0); 
			toSlot->SetItem(fromKey, fromCount);
			isFirstSwap = false; 
			fromSlot = toSlot;
			toSlot = nullptr;
			cloneIcon->SetItem(toKey, toCount);
			UpdateCloneIcon();
		}
		else
		{
			toSlot->SetItem(cloneIcon->GetItemCount().first, cloneIcon->GetItemCount().second);
			fromSlot = toSlot;
			toSlot = nullptr;
			cloneIcon->SetItem(toKey, toCount);
			UpdateCloneIcon();
		}
	}
	
}

void Inventory::OnSelectSlot(InventorySlot* inventorySlot)
{
	if (fromSlot == nullptr && inventorySlot->GetTag() == "ResultSlot" && inventorySlot->GetCount() > 0)
	{
		if (inventorySlot->GetKey() != 0)
		{
			fromSlot = inventorySlot;
			fromSlot->GetIcon()->SetActive(false);
			isExcuteCrafting = true;
			fromSlot->SetChanged(true);
			
			UpdateCloneIcon();
		}
		return;
	}	

	if (fromSlot == nullptr && inventorySlot->GetKey() == 0) return;

	if (fromSlot == nullptr)
	{
		fromSlot = inventorySlot;
		if (!fromSlot->IsPressShift())
		{
			fromSlot->GetIcon()->SetActive(false);
		}
		else
		{
			UINT totalCount = fromSlot->GetCount();
			UINT cloneCount = (totalCount % 2 == 1) ? (totalCount / 2 + 1) : (totalCount / 2);
			UINT remainingCount = totalCount - cloneCount;

			fromSlot->DecreaseItem(fromSlot->GetKey(), cloneCount);
			fromSlot->GetIcon()->SetActive(remainingCount > 0);
			cloneIcon->SetItem(fromSlot->GetKey(), cloneCount);
			cloneIcon->SetActive(true);
		}
		UpdateCloneIcon();
		return;
	}
	if (fromSlot != nullptr)
	{
		toSlot = inventorySlot;
		TransferItem();
	}
}

InventorySlot* Inventory::FindSlotAtPosition()
{
	for (InventorySlot* slot : slots)
	{
		if (slot->IsPointCollision(mousePos))  
		{
			return slot;  
		}
	}
	return nullptr;
}

pair<UINT, UINT> Inventory::GetQuickSlotData(int index)
{
	if (index < 0 || index >= 9) 
		return { 0, 0 };

	if (slots[index]->GetTag() == "SyncQuickSlot")
		return { slots[index]->GetKey(), slots[index]->GetCount() };

	return { 0, 0 };
}