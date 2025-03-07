#pragma once

class ComplexBlock : public Block
{
public:
	ComplexBlock(UINT key);
	~ComplexBlock();

	//BoxCollider* GetCollider() override { return boundingBoxCollider; }

	void Load(string path);
private:
	vector<Cube*>components;
	BoxCollider* boundingBoxCollider;
};