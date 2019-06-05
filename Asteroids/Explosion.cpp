#include "Explosion.h"
#include "Graphics.h"
#include "ImmediateMode.h"
#include "Random.h"
#include "Maths.h"

Explosion::Explosion(const XMVECTOR& position): lifetime_(40) {
	
	const XMVECTOR up = { 0,1,0 };

	for (int i = 0; i < NUM_PARTICLES; i++)
	{
		vert_[i].x = XMVectorGetX(position);
		vert_[i].y = XMVectorGetY(position);
		vert_[i].z = 10.0f;
		vert_[i].diffuse = 0xa87c55ff;
		dust_[i].velocity = Random::GetFloat(0, 2);
		dust_[i].normal = XMVector3Normalize(XMVector3TransformNormal(up, XMMatrixRotationZ(Random::GetFloat(Maths::TWO_PI))));
	}
}

void Explosion::Update(System *systems)
{
	--lifetime_;

	for (int i = 0; i < NUM_PARTICLES; i++) 
	{
		vert_[i].x += dust_[i].velocity * XMVectorGetX(dust_[i].normal);
		vert_[i].y+= dust_[i].velocity * XMVectorGetY(dust_[i].normal);
	}
}

void Explosion::Render(Graphics *graphics) const
{
	graphics->GetImmediateMode()->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
		&vert_[0],
		NUM_PARTICLES);
}

const bool& Explosion::HasCompleteLifeSpan() const {
	return (lifetime_<=0);
}
