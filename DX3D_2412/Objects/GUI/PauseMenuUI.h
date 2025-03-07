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

	void OnExitButtonClick();
	void OnResumeButtonClick();
private:
	bool isActive = false;
	vector<Button*>buttons;
};