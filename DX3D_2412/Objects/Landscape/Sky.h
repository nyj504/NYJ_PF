#pragma once
class Sky : public Sphere
{
private:
	enum class SkyState
	{
		SUNRIZE, MIDDAY, SUNSET, DEEPDUSK, MIDNIGHT, DAYBREAK, DAWN, COUNT
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
	SkyState startSkyState;
	SkyState endSkyState;

	int stateValue;
	
	FloatValueBuffer* buffer;

	Texture* skyMapStart;
	Texture* skyMapEnd;

	RasterizerState* rasterizerState[2];
	DepthStencilState* depthStencilState[2];

	unordered_map<SkyState, string>skies;
};