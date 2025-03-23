#include "Framework.h"

Player::Player() : Character("Resources/Models/Player.model")
{
	tag = "Player";

	clientCenterPos = { SCREEN_WIDTH >> 1, SCREEN_HEIGHT >> 1 };
	ClientToScreen(hWnd, &clientCenterPos);
	SetCursorPos(clientCenterPos.x, clientCenterPos.y);
	ShowCursor(false);

	modelAnimator->SetParent(this);
	modelAnimator->UpdateWorld();
}

Player::~Player()
{
	delete weapon;
	delete weaponSocket;
	delete item;
}

void Player::Update()
{
	//if (KEY->Down('C'))
	//{
	//	isCreativeMode = !isCreativeMode;
	//}
	//
	//if (isCreativeMode)
	//{
	//	velocity = Vector3(0, 0, 0);
	//
	//	velocity.y = 0;
	//}

	modelAnimator->Update();
	collider->UpdateWorld();

	UpdateWorld();
	
	BuildAndMining();
	SetCursor();
	Control();
	Jump();
	Move();
	CAM->UpdateWorld();
}

void Player::Render()
{
	modelAnimator->Render();
	collider->Render();
}

void Player::PostRender()
{
}

void Player::SetPlayerState(PlayerState state)
{
	this->playerState = state;

	switch (playerState)
	{
	case Player::IDLE:
		modelAnimator->PlayClip(0);
		break;
	case Player::MOVE:
		break;
	case Player::JUMP:
		modelAnimator->PlayClip(2);

		if (jumpTime >= 0)
		{
			jumpTime -= DELTA;
		}
		break;
	case Player::FALL:
		velocity.y -= GRAVITY * DELTA;
		break;
	case Player::LAND:
		velocity.y = 0;
		break;
	case Player::TOUCH:
		modelAnimator->PlayClip(7);
	default:
		break;
	}
}

void Player::SetLand()
{
	if (jumpTime <= 0)
	SetPlayerState(LAND);
}
	
void Player::SetFall()
{
	if (jumpTime <= 0)
	{
		SetPlayerState(FALL);
	}
}

void Player::Control()
{
	Vector3 dir;

	if (KEY->Press('W'))
	{
		dir += GetForward();
	}
	if (KEY->Press('S'))
	{
		dir += GetBack();
	}
	if (KEY->Press('A'))
	{
		dir += GetLeft();
	}
	if (KEY->Press('D'))
	{
		dir += GetRight();
	}

	//if (isCreativeMode) 
	//{
	//	if (KEY->Press(VK_SPACE)) 
	//		dir.y += 1.0f;
	//	if (KEY->Press(VK_SHIFT)) 
	//		dir.y -= 1.0f;
	//}

	dir.Normalize();

	velocity.x = dir.x;
	velocity.z = dir.z;

	if (CAM->IsFPSView() && !UIManager::Get()->IsPopup())
	{
		Vector3 delta = mousePos - CENTER;
		Rotate(Vector3::Up(), delta.x * rotSpeed * DELTA);
		CAM->Rotate(Vector3::Left(), delta.y * rotSpeed * DELTA);
	}
}

void Player::Jump()
{
	if (KEY->Down(VK_SPACE))
	{
		velocity.y += JUMP_POWER;
		jumpTime = 0.2f;

		SetPlayerState(JUMP);
	}
}

void Player::Move()
{
	Translate(velocity * moveSpeed * DELTA);
}

void Player::BuildAndMining()
{
	if (UIManager::Get()->IsPopup()) return;
	Block* block = BlockManager::Get()->GetSelectedBlock();
	if (BlockManager::Get()->GetSelectedBlock() == nullptr) return;
	
	if (KEY->Down(VK_RBUTTON))
	{
		if (BlockManager::Get()->GetSelectedBlock()->GetBlockType() == 1)
			BlockManager::Get()->InteractingBlock();
		else
		{	
			SetPlayerState(TOUCH);
			BlockManager::Get()->BuildBlock();
		}
	}
	else if (KEY->Down(VK_LBUTTON))
	{
		if (BlockManager::Get()->GetSelectedBlock()->GetBlockType() == 1 && UIManager::Get()->IsCrafting())
			return;

		SetPlayerState(TOUCH);
		BlockManager::Get()->MiningBlock();
	}
}

void Player::SetCursor()
{
	if (UIManager::Get()->IsPopup()) return;
	if (CAM->IsQuaterView()) return;

	SetCursorPos(clientCenterPos.x, clientCenterPos.y);
}
