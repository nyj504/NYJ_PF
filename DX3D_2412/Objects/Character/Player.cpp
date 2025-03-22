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

	armor = new Armor(modelAnimator);
	armor->EquipArmor(AmoType::CHESTPLATE, "Diamond");
	armor->EquipArmor(AmoType::HELMET, "Iron");
	armor->EquipArmor(AmoType::LEGGINGS, "Gold");
	armor->EquipArmor(AmoType::BOOTS, "Diamond");

	weapon = new Model("DiamondPickAxe");
	weapon->Load();

	weaponSocket = new Transform();
	weapon->SetParent(weaponSocket);
}

Player::~Player()
{
	delete weapon;
	delete weaponSocket;
	delete armor;
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
	weaponSocket->SetWorld(modelAnimator->GetTransformByNode(18));
	modelAnimator->Update();
	collider->UpdateWorld();

	weapon->UpdateWorld();
	UpdateWorld();
	
	PlayerStateMachine();
	//BuildAndMining();

	SetCursor();
	Control();
	Move();
	CAM->UpdateWorld();
	armor->Update();

}

void Player::Render()
{
	modelAnimator->Render();
	collider->Render();
	weapon->Render();
	armor->Render();
}

void Player::PostRender()
{
}

void Player::PlayerStateMachine()
{
	switch (playerState)
	{
	case Player::IDLE:
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
		Jump();
		break;
	default:
		break;
	}
}

void Player::SetPlayerState(PlayerState state)
{
	this->playerState = state;
}

void Player::SetLand()
{
	if (jumpTime <= 0)
	SetPlayerState(LAND);

	modelAnimator->PlayClip(0);
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

		modelAnimator->PlayClip(2);
		SetPlayerState(JUMP);
	}
}

void Player::Move()
{
	if (velocity.x == 0 && velocity.z == 0)
	{
		isMove = false;
		modelAnimator->PlayClip(0);
	}

	if (velocity.x > 0 || velocity.z > 0 || isMove != true)
	{
		isMove = true;
		modelAnimator->PlayClip(5);
	}
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
			modelAnimator->PlayClip(7);
			BlockManager::Get()->BuildBlock();
		}
	}
	else if (KEY->Down(VK_LBUTTON))
	{
		if (BlockManager::Get()->GetSelectedBlock()->GetBlockType() == 1 && UIManager::Get()->IsCrafting())
			return;

		modelAnimator->PlayClip(7);
		BlockManager::Get()->MiningBlock();
	}
}

void Player::SetCursor()
{
	if (UIManager::Get()->IsPopup()) return;
	if (CAM->IsQuaterView()) return;

	SetCursorPos(clientCenterPos.x, clientCenterPos.y);
}
