#include "Framework.h"
#include "EditScene.h"

EditScene::EditScene()
{
	editor = new ObjectEditor();
}

EditScene::~EditScene()
{
	delete editor;
}

void EditScene::Update()
{
	editor->Update();
}

void EditScene::PreRender()
{
}

void EditScene::Render()
{
	editor->Render();
}

void EditScene::PostRender()
{
}

void EditScene::GUIRender()
{
	editor->Edit();
}
