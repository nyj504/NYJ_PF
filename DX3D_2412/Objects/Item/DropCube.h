#pragma once
class DropCube : public DropItem
{
public:
	DropCube();
	DropCube(bool isMultiBlock);
	~DropCube();

	void Update();
	void Render();
	void SetImage(string path) override;

private:
	Cube* cube;
};