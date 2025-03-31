#include "Framework.h"

Player::Player() : Character("SteveRigged")
{
	tag = "Player";

	clientCenterPos = { SCREEN_WIDTH >> 1, SCREEN_HEIGHT >> 1 };
	ClientToScreen(hWnd, &clientCenterPos);
	SetCursorPos(clientCenterPos.x, clientCenterPos.y);
	ShowCursor(false);
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

	Character::Update();
	UpdateWorld();
	BuildAndMining();
	SetCursor();
	Control();
	Move();
	CAM->UpdateWorld();
}

void Player::Render()
{
	Character::Render();
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
		if (jumpTime >= 0)
		{
			jumpTime -= DELTA;
		}
		else if (jumpTime <= 0)
		{
			SetPlayerState(FALL);
		}
		break;
	case Player::FALL:
		velocity.y -= GRAVITY * DELTA;
		break;
	case Player::LAND:
		velocity.y = 0;
		if (KEY->Down(VK_SPACE))
		{
			jumpTime = 0.2f;
			velocity.y += JUMP_POWER;
			SetPlayerState(JUMP);
		}
		break;
	case Player::TOUCH:
	default:
		break;
	}
}

void Player::SetLand()
{
	SetPlayerState(LAND);
}
	
void Player::SetFall()
{
	SetPlayerState(FALL);
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

void Player::Move()
{
	Translate(velocity * moveSpeed * DELTA);
}

void Player::BuildAndMining()
{
	isMining = false;

	if (UIManager::Get()->IsPopup()) return;
	Block* block = BlockManager::Get()->GetSelectedBlock();
	if (block == nullptr) return;
	
	if (KEY->Down(VK_RBUTTON))
	{
		if (BlockManager::Get()->GetSelectedBlock()->GetBlockType() == 1)
			BlockManager::Get()->InteractingBlock();
		else
		{	
			BlockManager::Get()->BuildBlock();
		}
	}
	if (KEY->Down(VK_LBUTTON))
	{
		if (BlockManager::Get()->GetSelectedBlock()->GetBlockType() == 1 && UIManager::Get()->IsCrafting())
			return;

		BlockManager::Get()->GetCrackEffect()->SetMining(block);
		BlockManager::Get()->MiningBlock();
		isMining = true;
	}
	if (KEY->Press(VK_LBUTTON))
	{
		isMining = true;
	}
	if (KEY->Up(VK_LBUTTON))
	{
		isMining = false;
		BlockManager::Get()->CallStopMining();
		BlockManager::Get()->GetCrackEffect()->ResetMining();
	}
}

void Player::SetCursor()
{
	if (UIManager::Get()->IsPopup()) return;
	if (CAM->IsQuaterView()) return;

	SetCursorPos(clientCenterPos.x, clientCenterPos.y);
}
