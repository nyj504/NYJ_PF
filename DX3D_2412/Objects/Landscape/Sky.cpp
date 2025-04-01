#include "Framework.h"

Sky::Sky()
{
	buffer = new FloatValueBuffer();

	material->SetShader(L"Landscape/Sky.hlsl");

	skyMapStart = Texture::Add(L"Resources/Textures/Skybox/SunRize_4096x2048.dds");
	skyMapEnd = Texture::Add(L"Resources/Textures/Skybox/Midday_4096x2048.dds");

	skies[SkyState::SUNRIZE] = "Resources/Textures/Skybox/SunRize_4096x2048.dds";
	skies[SkyState::MIDDAY] = "Resources/Textures/Skybox/Midday_4096x2048.dds";
	skies[SkyState::SUNSET] = "Resources/Textures/Skybox/Sunset_4096x2048.dds";
	skies[SkyState::DEEPDUSK] = "Resources/Textures/Skybox/DeepDusk_4096x2048.dds";
	skies[SkyState::NIGHT] = "Resources/Textures/Skybox/Night_4096x2048.dds";
	skies[SkyState::MIDNIGHT] = "Resources/Textures/Skybox/Night_4096x2048.dds";

	rasterizerState[0] = new RasterizerState();
	rasterizerState[1] = new RasterizerState();
	rasterizerState[1]->FrontCounterClockwise(true);
	
	depthStencilState[0] = new DepthStencilState();
	depthStencilState[1] = new DepthStencilState();
	depthStencilState[1]->DepthEnable(false);

	//Skybox* sky = new Skybox(L"Resources/Textures/Skybox/SkyboxBlueSunset_4096x2048");
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
	if (skyState == SkyState::SUNRIZE || skyState == SkyState::DEEPDUSK ||
		skyState == SkyState::SUNSET)
	{
		buffer->Get()[0] += DELTA * DAYBREAK_TIME;
	}
	else
	{
		buffer->Get()[0] += DELTA * WORLD_TIME;
	}
	
	if (skyState == SkyState::MIDNIGHT)
	{
		stateValue = 0;
	}

	if (buffer->Get()[0] >= 1.0f)
	{
		buffer->Get()[0] = 0.0f;
		stateValue++;
		
		skyState = (SkyState)stateValue;
		skyMapStart = skyMapEnd;
		skyMapEnd = Texture::Add(Utility::ToWString(skies[skyState]));
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

