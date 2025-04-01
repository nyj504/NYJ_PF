#pragma once
class Sky : public Sphere
{
private:
	enum class SkyState
	{
		SUNRIZE = 1, MIDDAY, SUNSET, DEEPDUSK, NIGHT, MIDNIGHT
	};
	const float WORLD_TIME = 0.01f;
	const float DAYBREAK_TIME = 0.1f;
public:
	Sky();
	~Sky();

	void Update();
	void Render();
	void Edit();

private:
	SkyState skyState = SkyState::SUNRIZE;
	int stateValue = 0;
	
	FloatValueBuffer* buffer;

	Texture* skyMapStart;
	Texture* skyMapEnd;

	RasterizerState* rasterizerState[2];
	DepthStencilState* depthStencilState[2];

	unordered_map<SkyState, string>skies;
};