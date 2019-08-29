#pragma once
#include "InputComponent.h"
#include <DirectXMath.h>

using namespace DirectX;
class PlayerInputComponent :
	public InputComponent
{
public:
	PlayerInputComponent();
	~PlayerInputComponent();

	void Update(Game *game, System* system) override;
private:
	// Bullet Shoot Mode Logic
	enum ShootMode {
		Single,
		Spiral,
		MultiSpiral,
		Three
	};

	ShootMode shootMode_;
	float shootRot_ = 0;
	double spawnBulletAfter_;

	double elapsedTime_;

	XMVECTOR RotateVectorBy(const float& angle, const float& playerRoataion) const;
};

