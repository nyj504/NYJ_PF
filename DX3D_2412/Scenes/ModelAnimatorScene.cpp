#include "Framework.h"
#include "ModelAnimatorScene.h"

ModelAnimationScene::ModelAnimationScene()
{
	test = new Transform();

	model = new ModelAnimator("SteveRigged");
	model->Load();
	model->SetParent(test);
	
	model->ReadClip("Idle"); //0
	model->ReadClip("Walk");
	model->ReadClip("Run");
	model->ReadClip("Mining");
	model->ReadClip("Dying"); //1
	model->ReadClip("Dance"); //5
	model->CreateTexture();

	collider = new BoxCollider();
	collider->SetTag("PlayerCollider");
	collider->Load();
	collider->SetParent(test);
}

ModelAnimationScene::~ModelAnimationScene()
{
	delete model;
	delete collider;
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

	model->Update();
	collider->UpdateWorld();
}

void ModelAnimationScene::PreRender()
{
}

void ModelAnimationScene::Render()
{
	model->Render();
	//collider->Render();
}

void ModelAnimationScene::PostRender()
{
}

void ModelAnimationScene::GUIRender()
{
	model->Edit();
	collider->Edit();
}
