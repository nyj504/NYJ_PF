#include "Framework.h"

UIManager::UIManager()
{
	craftingUI = new CraftingUI();
	quickSlot = new QuickSlot();
	pauseMenuUI = new PauseMenuUI();

	CreateInGameUI();
}

UIManager::~UIManager()
{
	delete craftingUI;
	delete quickSlot;
	delete pauseMenuUI;

	delete hpBar;
	delete armorBar;
	delete hungerBar;
}

void UIManager::Update()
{
	if (KEY->Down('I') || KEY->Down(VK_TAB))
	{
		SetTopSlot("CraftInventory");
	}

	if (KEY->Down(VK_ESCAPE))
	{
		isPopup = !isPopup;
		SetCursorState();
		pauseMenuUI->SetActive(isPopup);
	}

	if (CAM->IsQuaterView())
	{
		cursor->SetGlobalPosition(mousePos);
		cursor->UpdateWorld();
	}

	InventorySingleton::Get()->Update();
	craftingUI->Update();
	quickSlot->Update();
	pauseMenuUI->Update();
}

void UIManager::PostRender()
{
	InventorySingleton::Get()->Render();
	
	craftingUI->PostRender();
	quickSlot->Render();
	pauseMenuUI->Render();

	hpBar->Render();
	armorBar->Render();
	hungerBar->Render();

	if(!isPopup)
		cursor->Render();
}

void UIManager::Edit()
{
	pauseMenuUI->Edit();
}

void UIManager::SetTopSlot(string type)
{
	isPopup = !isPopup;
	isCrafting = !isCrafting;
	SetCursorState();
	craftingUI->SetTopSlot(type);
}

void UIManager::SetCursorState()
{
	ShowCursor(isPopup);
}

void UIManager::CreateInGameUI()
{
	cursor = new Quad(L"Resources/Textures/UI/cursor.png");
	cursor->SetLocalPosition(CENTER);
	cursor->UpdateWorld();

	hpBar = new HUDBar();
	hpBar->Create("Resources/Textures/UI/heart.png", Vector3(CENTER.x - 260, CENTER.y - 280));

	armorBar = new HUDBar();
	armorBar->Create("Resources/Textures/UI/armor_empty.png", Vector3(CENTER.x - 260, CENTER.y - 256));
	armorBar->UpdateArmorBar(0);

	hungerBar = new HUDBar();
	hungerBar->Create("Resources/Textures/UI/hunger_full.png", Vector3(CENTER.x + 86, CENTER.y - 280));
}
