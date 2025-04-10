#include "Framework.h"

Inventory::Inventory() : Quad((L"Resources/Textures/GUI/inventorySlot.png"))
{
	localPosition = { CENTER };
	UpdateWorld();

	tag = "Inventory";
	cloneIcon = new SlotIcon();
	cloneIcon->SetActive(false);
	CreateSlot();

	invenCamera = new Camera();
	invenCamera->SetTarget(PLAYER);
	invenCamera->TargetOptionLoad("RenderTargetMode");
	invenCamera->UpdateWorld();

	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil();

	Texture* texture = Texture::Add(L"Target", renderTarget->GetSRV());
	quad = new Quad(Vector2(100, 142));
	quad->GetMaterial()->SetShader(L"Basic/Texture.hlsl");	
	quad->GetMaterial()->SetDiffuseMap(texture);
	quad->SetLocalPosition(Vector3(CENTER.x - 75, CENTER.y + 163));
	quad->UpdateWorld();

	SetActive(false);

	AddItem(55, 36);
	AddItem(11, 10);
	AddItem(96, 10);
	AddItem(67, 1);
	
	AddItem(24, 64);
	AddItem(26, 10);
	AddItem(37, 2);
	AddItem(39, 3);
	
	AddItem(36, 1);
	AddItem(19, 64);
	AddItem(76, 1);
	AddItem(77, 1);
	
	AddItem(92, 1);
	AddItem(93, 1);
	AddItem(94, 1);
	AddItem(95, 1);
	AddItem(90, 1);

}

Inventory::~Inventory()
{
	for (InventorySlot* slot : slots)
		delete slot;
	delete cloneIcon;
	delete renderTarget;
	delete depthStencil;
	delete quad;
	delete invenCamera;
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

	if (isRenderTargetActivate)
	{
		invenCamera->Update();
	}
}

void Inventory::PreRender()
{
	if (!isActive && isRenderTargetActivate) return;

	renderTarget->Set(depthStencil);//렌더 타겟을 depthStencil로 설정
	invenCamera->SetView(); //invenCamera 뷰 설정
	PLAYER->Render();
	EquipManager::Get()->Render();
}

void Inventory::Render()
{
	if (!isActive) return;

	Quad::Render();

	for (InventorySlot* slot : slots)
		slot->Render();

	cloneIcon->Render();

	if(isRenderTargetActivate)
	quad->Render();
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
	if (fromSlot->IsPressShift() && cloneIcon->GetItemCount().second < 0) return;

	if (fromSlot != nullptr && isFirstSwap)
	{
		cloneIcon->SetActive(true);
		cloneIcon->UpdateFromSlot(fromSlot);
	}
}

void Inventory::Clear()
{
	if (toSlot->GetTag() == "SyncQuickSlot")
	{
		isRefreshQuickSlot = true;
	}

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
		if (slot->GetKey() == key && slot->GetCount() + count <= MAX_INVENTORY_STORAGE)
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

void Inventory::DecreaseItem(UINT count)
{
	if (count == 0) return;

	for (InventorySlot* slot : slots)
	{
		slot->DecreaseItem(count);
		
		if (slot->GetTag() == "SyncQuickSlot")
		{
			isRefreshQuickSlot = true;
		}
	}
}

void Inventory::ConsumeItem(UINT slotNum)
{
	if (slots[slotNum]->GetCount() <= 0) return;

	slots[slotNum]->ConsumeItem();
	isRefreshQuickSlot = true;
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
		Clear();
		return;
	}

	if (toSlot->IsPush() && fromCount > 0)
	{   //아이템 든 상태로 1개씩 나눌 때
		fromSlot->ConsumeItem();
		toSlot->SetItem(fromKey, 1);

		if (fromSlot->GetCount() <= 0)
		{
			Clear();
			return;
		}
		else 
		{
			fromSlot->GetIcon()->SetActive(false);
			toSlot->SetRest(true);
			cloneIcon->SetItem(fromKey, fromCount);
			UpdateCloneIcon();
			return;
		}
	}

	if (toKey == fromKey) //같으면 더하기
	{
		if (!fromSlot->IsPressShift())
		{
			toSlot->SetItem(fromKey, fromCount);
			fromSlot->SetItem(0, 0);
		}
		else
		{
			toSlot->SetItem(fromKey, cloneIcon->GetItemCount().second);
		}
		Clear();
		return;
	}

	if (toKey == 0) //빈슬롯이면 그 슬롯에 넣기
	{
		if (isFirstSwap)
		{
			fromSlot->SetItem(0, 0);
			toSlot->SetItem(fromKey, fromCount);
		}
		else
		{
			toSlot->SetItem(cloneIcon->GetItemCount().first, cloneIcon->GetItemCount().second);
		}
		Clear();
		return;
	}

	if (toKey != fromKey && toKey != 0) //서로 다른 아이템  
	{
		if (fromSlot->IsPressShift())
		{
			fromSlot->SetPressShift(false);
			toSlot->SetItem(cloneIcon->GetItemCount().first, cloneIcon->GetItemCount().second);
			cloneIcon->SetItem(toKey, toCount);
		}
		else if (isFirstSwap)
		{
			fromSlot->SetItem(0, 0); 
			toSlot->SetItem(fromKey, fromCount);
			isFirstSwap = false; 
			fromSlot = toSlot;
			cloneIcon->SetItem(toKey, toCount);
		}
		else
		{
			toSlot->SetItem(cloneIcon->GetItemCount().first, cloneIcon->GetItemCount().second);
			fromSlot = toSlot;
			cloneIcon->SetItem(toKey, toCount);
		}

		UpdateCloneIcon();
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
			EventManager::Get()->ExcuteEvent("ExcuteCrafting");
			
			UpdateCloneIcon();
		}
		return;
	}	

	if (fromSlot == nullptr && inventorySlot->GetKey() == 0) return;

	if (fromSlot == nullptr && !inventorySlot->IsPush())
	{
		fromSlot = inventorySlot;

		if (!fromSlot->IsPressShift()) 
		{
			fromSlot->GetIcon()->SetActive(false);
		}
		else //Shift를 누른 상태 (절반으로 나누기)
		{
			isFirstSwap = false; 
			UINT totalCount = fromSlot->GetCount();
			UINT cloneCount = (totalCount % 2 == 1) ? (totalCount / 2 + 1) : (totalCount / 2);
			UINT remainingCount = totalCount - cloneCount;

			fromSlot->DecreaseItem(cloneCount);
			fromSlot->GetIcon()->SetActive(remainingCount > 0);
			cloneIcon->SetItem(fromSlot->GetKey(), cloneCount);
		}
		UpdateCloneIcon();
		return;
	}
	else
	{
		inventorySlot->SetRest(true);
	}

	if (fromSlot != nullptr)
	{
		toSlot = inventorySlot;

		TransferItem();
	}
}

pair<UINT, UINT> Inventory::GetQuickSlotData(int index)
{
	return { slots[index]->GetKey(), slots[index]->GetCount() };
}

void Inventory::Save()
{
	string path = "Resources/Transforms/Inven.srt";
	BinaryWriter* writer = new BinaryWriter(path);

	int count = slots.size();
	writer->Data<int>(count);

	for (InventorySlot* slot : slots)
	{
		writer->Data<UINT>(slot->GetKey());
		writer->Data<UINT>(slot->GetCount());
	}

	delete writer;
}

void Inventory::Load()
{
	string path = "Resources/Transforms/Inven.srt";
	BinaryReader* reader = new BinaryReader(path);

	int count = reader->Data<int>();
	
	for (int i = 0; i < count; i++)
	{
		UINT key = reader->Data<UINT>();
		UINT count = reader->Data<UINT>();

		slots[i]->SetItem(key, count);
	}
	
	isRefreshQuickSlot = true;

	delete reader;
}
