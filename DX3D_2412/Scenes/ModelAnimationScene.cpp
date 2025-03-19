#include "Framework.h"
#include "ModelAnimationScene.h"

ModelAnimationScene::ModelAnimationScene()
{
	model = new ModelAnimator("SteveRigged");

	model->ReadClip("Walk");
	model->ReadClip("Run");
	model->ReadClip("BowCharging");
	model->ReadClip("BowShoot");
	model->ReadClip("Jump");
	model->ReadClip("Mining");
	model->ReadClip("Dance");
	model->ReadClip("Swimming");
	model->ReadClip("HandAttack");
	model->ReadClip("Dying");

	model->CreateTexture();
}

ModelAnimationScene::~ModelAnimationScene()
{
	delete model;
}

void ModelAnimationScene::Update()
{
	if (KEY->Down('1'))
	{
		model->PlayClip(0);
	}
	if (KEY->Down('2'))
	{
		model->PlayClip(1);
	}
	if (KEY->Down('3'))
	{
		model->PlayClip(2);
	}
	if (KEY->Down('4'))
	{
		model->PlayClip(3);
	}
	if (KEY->Down('5'))
	{
		model->PlayClip(4);
	}
	if (KEY->Down('6'))
	{
		model->PlayClip(5);
	}
	if (KEY->Down('7'))
	{
		model->PlayClip(6);
	}
	if (KEY->Down('8'))
	{
		model->PlayClip(7);
	}
	if (KEY->Down('9'))
	{
		model->PlayClip(8);
	}
	if (KEY->Down('0'))
	{
		model->PlayClip(9);
	}
	if (KEY->Down('U'))
	{
		model->PlayClip(10);
	}

	model->Update();
}

void ModelAnimationScene::PreRender()
{
}

void ModelAnimationScene::Render()
{
	model->Render();
}

void ModelAnimationScene::PostRender()
{
}

void ModelAnimationScene::GUIRender()
{
	model->Edit();
}
