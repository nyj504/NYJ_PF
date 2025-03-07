#pragma once
class ObjectEditor
{
public:
	ObjectEditor();
	~ObjectEditor();

	void Update();
	void Render();
	void Edit();

private:
	ObjectModel* model;
};