#pragma once
class DropQuad : public DropItem
{
public:
	DropQuad();
	~DropQuad();

	void Update();
	void Render();
	void SetImage(string path) override;

private:
	Quad* quad;
	RectCollider* collider;
	RasterizerState* rasterizerState[2];
};