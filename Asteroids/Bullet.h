#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "GameEntity.h"

class Bullet : public GameEntity
{
public:
	Bullet(XMVECTOR position,
		XMVECTOR direction);

	void Update(System *system);
	void Render(Graphics *graphics) const;
	bool HasCompletedLifeSpan();

private:

	XMFLOAT3 velocity_;
	const double lifeSpan_ = 0.5;
	double timeActive_; 
};

#endif // BULLET_H_INCLUDED
