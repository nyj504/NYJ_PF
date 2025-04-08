#pragma once
class PauseMenuUI
{
public:
	PauseMenuUI();
	~PauseMenuUI();

	void Render();
	void Update();
	void Edit();
	void SetActive(bool isActive) { this->isActive = isActive; }

	void OnSaveAndExitButtonClick();
	void OnResumeButtonClick();
private:
	bool isActive = false;
	vector<Button*>buttons;
};