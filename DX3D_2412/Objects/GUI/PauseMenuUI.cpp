#include "Framework.h"
#include "PauseMenuUI.h"

PauseMenuUI::PauseMenuUI()
{
	Button* button = new Button(L"Resources/Textures/UI/longButton.png");
	button->SetLocalPosition(Vector3(CENTER.x, CENTER.y + 150));
	button->SetText("Back to Game");
	button->SetEvent(bind(&PauseMenuUI::OnResumeButtonClick, this));
	button->UpdateWorld();

	buttons.push_back(button);

	Button* button1 = new Button(L"Resources/Textures/UI/longButton.png");
	button1->SetLocalPosition(Vector3(CENTER.x, CENTER.y + 100));
	button1->SetText("Save and Quit to Title");
	button1->SetEvent(bind(&PauseMenuUI::OnExitButtonClick, this));
	button1->UpdateWorld();
	
	buttons.push_back(button1);
}

PauseMenuUI::~PauseMenuUI()
{
	for (Button* button : buttons)
		delete button;
}

void PauseMenuUI::Render()
{
	if (!isActive) return;

	for (Button* button : buttons)
		button->Render();

	FONT->RenderText("GameMenu", { CENTER.x, CENTER.y + 200 });
}

void PauseMenuUI::Update()
{
	if (!isActive) return;

	for (Button* button : buttons)
		button->Update();
}

void PauseMenuUI::Edit()
{
	for (Button* button : buttons)
		button->Edit();
}

void PauseMenuUI::OnExitButtonClick()
{
	//MAP->Save();
	SceneManager::Get()->Add("Lobby");
	SceneManager::Get()->Remove("Test");
}

void PauseMenuUI::OnResumeButtonClick()
{
	SetActive(false);
	UIManager::Get()->SetPopup(false);
}

