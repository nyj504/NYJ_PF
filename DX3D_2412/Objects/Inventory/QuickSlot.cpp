#include "Framework.h"

QuickSlot::QuickSlot() : Quad(L"Resources/Textures/UI/HotBar.png")
{
    localPosition = { CENTER.x, CENTER.y - 330, 0 };
    UpdateWorld();

	CreateSlot();
    EventManager::Get()->AddMouseWheelEvent([this](int delta) 
        {
        this->OnMouseWheel(delta);
        });
    Font::Get()->AddColor("White", 1, 1, 1);
    Font::Get()->AddStyle("Default", L"µ¸¿òÃ¼");

    SetActive(true);
}

QuickSlot::~QuickSlot()
{
	for (SlotIcon* quickSlotIcon : quickSlotIcons)
		delete quickSlotIcon;

    delete hotBarSelect;
}

void QuickSlot::Update()
{
    hotBarSelect->UpdateWorld();
   
    SyncWithInventory();
}

void QuickSlot::Render()
{
    if (!isActive) return;

    Quad::Render();

    for (SlotIcon* quickSlotIcon : quickSlotIcons)
        quickSlotIcon->Render();
    
    hotBarSelect->Render();
}

void QuickSlot::CreateSlot()
{
    quickSlotIcons.resize(MAX_SLOTSIZE);

    Vector3 startPos = { CENTER.x - 240, CENTER.y - 330, 0 };
    int interval = 60;

    for (int i = 0; i < MAX_SLOTSIZE; i++)
    {
        SlotIcon* quickSlot = new SlotIcon();

        Vector3 pos = { startPos.x + interval * i, startPos.y, 0 };

        quickSlot->SetLocalPosition(pos);
        quickSlot->SetLocalScale(Vector3(2.5f, 2.5f, 0.0f));
        quickSlot->SetActive(true);
        quickSlot->SetTag("SyncQuickSlot");
        quickSlot->UpdateWorld();

        quickSlotIcons[i] = quickSlot;
    }

    hotBarSelect = new Quad(L"Resources/Textures/UI/HotBarSelect.png");
    hotBarSelect->SetLocalPosition(quickSlotIcons[0]->GetGlobalPosition());
    hotBarSelect->UpdateWorld();
}

void QuickSlot::OnMouseWheel(int delta)
{
    if (delta > 0)
    {
        selectedIndex = (selectedIndex + 1) % MAX_SLOTSIZE;
    }
    else if (delta < 0)
    {
        selectedIndex = (selectedIndex - 1 + MAX_SLOTSIZE) % MAX_SLOTSIZE;
    }

    pair<UINT, UINT>quickSlotData = GetSelectedIndexData();

    ItemData itemData = DataManager::Get()->GetItemData(quickSlotData.first);

    if (itemData.itemType == "Weapon")
    {
        WeaponType type = WeaponType::SWORD;
        EquipmentData data = DataManager::Get()->GetEquipmentData(quickSlotData.first);

        if (data.equipParts == "Sword")
            type = WeaponType::SWORD;
        else if (data.equipParts == "Axe")
            type = WeaponType::AXE;
        else if (data.equipParts == "PickAxe")
            type = WeaponType::PICKAXE;
        else if (data.equipParts == "Shovel")
            type = WeaponType::SHOVEL;
        else if (data.equipParts == "Bow")
            type = WeaponType::BOW;

        EquipManager::Get()->EquipWeapon(type, data.equipType);
    }

    HighlightSelectedQuickSlot();
}

void QuickSlot::SyncWithInventory()
{
    if (!INVEN->IsRefreshQuickSlot()) return;

    for (int i = 0; i < quickSlotIcons.size(); i++)
    {
        pair<UINT, UINT> quickSlotData = INVEN->GetQuickSlotData(i); 

        if (quickSlotData.first == 0)
            quickSlotIcons[i]->Clear();
        else
            quickSlotIcons[i]->SetItem(quickSlotData.first, quickSlotData.second);
    }
        
    INVEN->SetRefreshQuickSlot(false);
    
}

void QuickSlot::HighlightSelectedQuickSlot()
{
    for (int i = 0; i < MAX_SLOTSIZE; i++)
    {
        if (i == selectedIndex)
        {
            hotBarSelect->SetLocalPosition(quickSlotIcons[i]->GetLocalPosition());
        }
    }
}