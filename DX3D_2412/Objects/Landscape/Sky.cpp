#include "Framework.h"

bool Sky::isNight = false;

Sky::Sky()
{
	buffer = new FloatValueBuffer();

	material->SetShader(L"Landscape/Sky.hlsl");

	startSkyState = SkyState::SUNRIZE;
	stateValue = (int)startSkyState;
	endSkyState = (SkyState)((stateValue + 1) % (int)SkyState::COUNT);

	skies[SkyState::SUNRIZE] = "Resources/Textures/Skybox/SunRize_4096x2048.dds";
	skies[SkyState::MIDDAY] = "Resources/Textures/Skybox/Midday_4096x2048.dds";
	skies[SkyState::SUNSET] = "Resources/Textures/Skybox/Sunset_4096x2048.dds";
	skies[SkyState::DEEPDUSK] = "Resources/Textures/Skybox/DeepDusk_4096x2048.dds";
	skies[SkyState::MIDNIGHT] = "Resources/Textures/Skybox/Midnight_4096x2048.dds";
	skies[SkyState::DAYBREAK] = "Resources/Textures/Skybox/Night_4096x2048.dds";
	skies[SkyState::DAWN] = "Resources/Textures/Skybox/DeepDusk_4096x2048.dds";

	skyMapStart = Texture::Add(Utility::ToWString(skies[startSkyState]));
	skyMapEnd = Texture::Add(Utility::ToWString(skies[endSkyState]));

	rasterizerState[0] = new RasterizerState();
	rasterizerState[1] = new RasterizerState();
	rasterizerState[1]->FrontCounterClockwise(true);
	
	depthStencilState[0] = new DepthStencilState();
	depthStencilState[1] = new DepthStencilState();
	depthStencilState[1]->DepthEnable(false);
}

Sky::~Sky()
{
	delete buffer;
	delete rasterizerState[0];
	delete rasterizerState[1];

	delete depthStencilState[0];
	delete depthStencilState[1];
}

void Sky::Update()
{
	if (startSkyState == SkyState::MIDNIGHT || startSkyState == SkyState::DAYBREAK)
		isNight = true;
	else
		isNight = false;

	if (startSkyState == SkyState::SUNSET || startSkyState == SkyState::DEEPDUSK ||
		startSkyState == SkyState::DAWN)
		buffer->Get()[0] += DELTA * DAYBREAK_TIME;
	else
		buffer->Get()[0] += DELTA * WORLD_TIME;

	if (buffer->Get()[0] >= 1.0f)
	{
		startSkyState = (SkyState)stateValue;

		buffer->Get()[0] = 0.0f;

		skyMapStart = skyMapEnd;

		stateValue = (stateValue + 1) % (int)SkyState::COUNT;
		
		endSkyState = (SkyState)stateValue;
		
		skyMapEnd = Texture::Add(Utility::ToWString(skies[endSkyState]));
	}
}

void Sky::Render()
{
	buffer->SetPS(10);

	rasterizerState[1]->SetState();
	depthStencilState[1]->SetState();

	skyMapStart->PSSet(10);
	skyMapEnd->PSSet(11);

	Sphere::Render();

	rasterizerState[0]->SetState();
	depthStencilState[0]->SetState();
}

void Sky::Edit()
{
	material->Edit();
}

