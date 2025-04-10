#include "Framework.h"

Player::Player(string name) : Character(name)
{
	tag = "Player";

	modelAnimator = new ModelAnimator(name);
	modelAnimator->Load();

	modelAnimator->ReadClip("Idle"); //0
	modelAnimator->ReadClip("Walk");
	modelAnimator->ReadClip("Run");
	modelAnimator->ReadClip("Mining");
	modelAnimator->ReadClip("Dying"); //1
	modelAnimator->ReadClip("Dance"); //5
	modelAnimator->CreateTexture();
	modelAnimator->SetParent(this);

	collider = new BoxCollider();
	collider->SetTag("PlayerCollider");
	collider->SetParent(this);
	collider->Load();

	clientCenterPos = { SCREEN_WIDTH >> 1, SCREEN_HEIGHT >> 1 };
	ClientToScreen(hWnd, &clientCenterPos);
	SetCursorPos(clientCenterPos.x, clientCenterPos.y);
	ShowCursor(false);
}

Player::~Player()
{
}

void Player::Update()
{
	SetCursor();
	Character::Update();
	UpdateWorld();
	
	if (KEY->Down(VK_SPACE))
	{
		velocity.y += JUMP_POWER;
	}

	BuildAndMining();
	Control();
	CAM->UpdateWorld();
}

void Player::Render()
{
	Character::Render();
}

void Player::Damaged(float damage, Character* target)
{
	Character::Damaged(damage, target);

	bool hitSound = GameMath::Random(0, 2);

	if (hitSound)
		Audio::Get()->Play("Steve_hurt");

	Audio::Get()->Play("Steve_hit");
}

void Player::SetPlayerState(PlayerState state)
{
	if (state == playerState) return;

	playerState = state;
	int clipNum = (int)playerState;

	if (state == MINING)
	{
		modelAnimator->PlayClip(clipNum, 2.0f, 0.2f);
	}
	else
	{
		modelAnimator->PlayClip(clipNum);
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
	dir.Normalize();

	if (KEY->Press(VK_SHIFT))
	{
		SetPlayerState(RUN);
		dir.x *= 2.0f;
		dir.z *= 2.0f;
	}
	else if(dir != 0.0f)
	{
		SetPlayerState(WALK);
	}

	velocity.x = dir.x;
	velocity.z = dir.z;

	if (velocity.x <= 0.1f && velocity.z <= 0.1f && playerState != MINING)
	{
		SetPlayerState(IDLE);
	}

	if (!UIManager::Get()->IsPopup())
	{
		Vector3 delta = mousePos - CENTER;
		Rotate(Vector3::Up(), delta.x * rotSpeed * DELTA);
		CAM->Rotate(Vector3::Left(), delta.y * rotSpeed * DELTA);
	}

	Move();
}

void Player::Move()
{
	Translate(velocity * characterData.moveSpeed * DELTA);
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
			SetPlayerState(MINING);
			BlockManager::Get()->BuildBlock();
		}
	}
	if (KEY->Up(VK_RBUTTON))
	{
		SetPlayerState(IDLE);
	}

	if (KEY->Down(VK_LBUTTON))
	{
		Audio::Get()->Play("Steve_sweep");

		SetPlayerState(MINING);

		if (BlockManager::Get()->GetSelectedBlock()->GetBlockType() == 1 && UIManager::Get()->IsCrafting())
			return;

		BlockManager::Get()->GetCrackEffect()->SetMining(block);
		BlockManager::Get()->MiningBlock();
		isMining = true;
	}
	if (KEY->Up(VK_LBUTTON))
	{
		SetPlayerState(IDLE);
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
