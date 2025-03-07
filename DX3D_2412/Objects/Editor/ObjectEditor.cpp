#include "Framework.h"
#include "ObjectEditor.h"

ObjectEditor::ObjectEditor()
{
	model = new ObjectModel();

}

ObjectEditor::~ObjectEditor()
{
	delete model;
}

void ObjectEditor::Update()
{
	model->Update();
}

void ObjectEditor::Render()
{
	model->Render();
}

void ObjectEditor::Edit()
{
	model->Edit();
}
