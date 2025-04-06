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
	delete playerHUD;
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

void UIManager::PreRender()
{
	InventorySingleton::Get()->PreRender();
}

void UIManager::PostRender()
{
	craftingUI->PostRender();
	InventorySingleton::Get()->Render();
	quickSlot->Render();
	pauseMenuUI->Render();
	playerHUD->Render();

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
	
	playerHUD = new PlayerHUD();
}
