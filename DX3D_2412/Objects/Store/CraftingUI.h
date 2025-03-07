#pragma once
class CraftingUI
{
public:
	CraftingUI();
	~CraftingUI();

	void Update();
	void PostRender();

	void SetTopSlot(string type);
	void SetActive(bool isActive) { this->isActive = isActive; }

private:
	bool isActive = false;

	unordered_map<string, CraftBox*> craftBoxes;
	CraftBox* currentTopSlot;
};