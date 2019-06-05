#ifndef EXPLOSION_H_INCLUDED
#define EXPLOSION_H_INCLUDED

#include "GameEntity.h"
#include <DirectXMath.h>
#include "ImmediateModeVertex.h"


struct Particle {
	float velocity;
	XMVECTOR normal;
};

class Explosion : public GameEntity
{
public:
	Explosion(const float& x, const float& y);
	Explosion(const XMVECTOR&);

	void Update(System *system);
	void Render(Graphics *graphics) const;

	const bool& HasCompleteLifeSpan() const;

private:

	enum { NUM_PARTICLES = 25 };

	ImmediateModeVertex vert_[NUM_PARTICLES];
	Particle dust_[NUM_PARTICLES];

	int lifetime_;

};

#endif // EXPLOSION_H_INCLUDED
