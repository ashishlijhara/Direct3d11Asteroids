#include "PlayerInputComponent.h"
#include "Maths.h"
#include "Game.h"
#include "Ship.h"
#include "System.h"
#include "Keyboard.h"

PlayerInputComponent::PlayerInputComponent():elapsedTime_(0),
spawnBulletAfter_(1),
shootMode_(ShootMode::Single)
{
}


PlayerInputComponent::~PlayerInputComponent() 
{
}

void PlayerInputComponent::Update(Game *game, System* system) {
	Ship* player_ = game->GetPlayer();
	Keyboard *keyboard = system->GetKeyboard();

	float acceleration = 0.0f;
	if (keyboard->IsKeyHeld(VK_UP) || keyboard->IsKeyHeld('W'))
	{
		acceleration = 1.0f;
	}
	else if (keyboard->IsKeyHeld(VK_DOWN) || keyboard->IsKeyHeld('S'))
	{
		acceleration = -1.0f;
	}

	float rotation = 0.0f;
	if (keyboard->IsKeyHeld(VK_RIGHT) || keyboard->IsKeyHeld('D'))
	{
		rotation = -1.0f;
	}
	else if (keyboard->IsKeyHeld(VK_LEFT) || keyboard->IsKeyHeld('A'))
	{
		rotation = 1.0f;
	}

	player_->SetControlInput(acceleration, rotation);
	player_->Update(system);
	game->WrapEntity(player_);

	//Switch Shoot Modes
	if (keyboard->IsKeyPressed(VK_NUMPAD1)) {
		shootMode_ = ShootMode::Single;
		spawnBulletAfter_ = 1;
	}
	else if (keyboard->IsKeyPressed(VK_NUMPAD2)) {
		shootMode_ = ShootMode::Spiral;
		spawnBulletAfter_ = 0.25;
	}
	else if (keyboard->IsKeyPressed(VK_NUMPAD3)) {
		shootMode_ = ShootMode::MultiSpiral;
		spawnBulletAfter_ = 0.05;
	}
	else if (keyboard->IsKeyPressed(VK_NUMPAD4)) {
		shootMode_ = ShootMode::Three;
		spawnBulletAfter_ = 0.5;
	}

	//if (keyboard->IsKeyPressed(VK_SPACE))
	//if(keyboard->IsKeyHeld(VK_SPACE) && (elapsedTime_>spawnBulletAfter_))
	if ((keyboard->IsKeyHeld(VK_LBUTTON) || keyboard->IsKeyHeld(VK_SPACE)) && (elapsedTime_ > spawnBulletAfter_))
	{
		elapsedTime_ = 0;
		XMVECTOR playerForward = player_->GetForwardVector();
		XMVECTOR bulletPosition = player_->GetPosition() + playerForward * 10.0f;
		switch (shootMode_)
		{
		case ShootMode::Single:
			game->SpawnBullet(bulletPosition, playerForward);
			break;
		case ShootMode::Spiral:
			shootRot_ += 25;
			game->SpawnBullet(bulletPosition, RotateVectorBy(shootRot_, player_->GetRotation()));
			break;
		case ShootMode::MultiSpiral:
			shootRot_ += 1.5;
			game->SpawnBullet(bulletPosition, RotateVectorBy(shootRot_, player_->GetRotation()));
			break;
		case ShootMode::Three:
			game->SpawnBullet(bulletPosition, playerForward);
			game->SpawnBullet(bulletPosition, RotateVectorBy(45, player_->GetRotation()));
			game->SpawnBullet(bulletPosition, RotateVectorBy(-45, player_->GetRotation()));
			break;
		}
	}
	elapsedTime_ += (double)system->GetFrameDeltaTime() / 100;
}

XMVECTOR PlayerInputComponent::RotateVectorBy(const float& angle, const float& roataion) const {
	float rotation = Maths::WrapModulo(roataion + angle, Maths::TWO_PI);
	XMFLOAT3 playerLeft;
	XMMATRIX rotationMatrix = XMMatrixRotationZ(rotation);
	XMVECTOR rotatedVec = XMVector3TransformNormal(XMVectorSet(0.f, 1.0f, 0.0f, 0.0f), rotationMatrix);
	rotatedVec = XMVector3Normalize(rotatedVec);
	return rotatedVec;
}
