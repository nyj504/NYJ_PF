#pragma once

class BoxCollider;

class Cube : public GameObject<VertexUVNormalTangent>
{
private:
	typedef VertexUVNormalTangent VertexType;

public:
	Cube(Vector3 size = { 1, 1, 1 });

	void Update();
	void Render();
	void RenderInstanced(UINT count);

	void SetSize(Vector3 size) { this->size = size; }

	Vector3 GetSize() { return size; }

	void ApplyBlockUVMapping();
	void ApplyObjectUVMapping();

	virtual BoxCollider* GetCollider() { return collider; }

	string GetEditTag() { return editTag; }
	void SetEditTag(string editTag) { this->editTag = editTag;}
private:
	void MakeMesh();
	void MakeMesh(Vector2 startUV, Vector2 endUV);

private:
	Vector3 size;	
	string editTag;

	BoxCollider* collider;
};