#pragma once
class UIManager : public Singleton<UIManager>
{
private:
	friend class Singleton;

	UIManager();
	~UIManager();

public:
	void Update();
	void PreRender();
	void PostRender();
	void Edit();

	void SetTopSlot(string type);
	QuickSlot* GetQuickSlot() { return quickSlot; }
	HUDBar* GetArmorBar() { return armorBar; }

	void SetCursorState();

	bool IsPopup() { return isPopup; }
	void SetPopup(bool isPopup) { this->isPopup = isPopup; }
	bool IsCrafting() { return isCrafting; }
private:
	void CreateInGameUI();

private:
	Quad* cursor;
	Block* selectBlock;

	bool isPopup = false;
	bool isCrafting = false;

	CraftingUI* craftingUI;
	QuickSlot* quickSlot;
	PauseMenuUI* pauseMenuUI;

	HUDBar* hpBar;
	HUDBar* armorBar;
	HUDBar* hungerBar;
};