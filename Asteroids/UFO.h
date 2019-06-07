#pragma once

#include "GameEntity.h"
class Ship;

class UFO : public GameEntity
{
public:
	UFO(XMVECTOR position,
		float velocity,
		int size, Ship* player);

	void Update(System *system);
	void Render(Graphics *graphics) const;

	const float & GetVelocity() const;
	int GetSize() const;

	void SetBlueMode(const bool&);

private:

	float velocity_;
	XMFLOAT3 axis_;
	XMVECTOR direction_;

	float angle_;
	float angularSpeed_;
	int size_;

	Ship *const player_;

	bool blueMode_;
};

