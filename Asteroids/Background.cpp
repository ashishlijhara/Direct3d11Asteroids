#include "Background.h"
#include "System.h"
#include "Graphics.h"
#include "ImmediateMode.h"
#include "Random.h"
#include "Maths.h"

const float& Background::Gaussian(const float& mu, const float& sigma, const float& val)const {
	return ((1 / (sigma*sqrtf(Maths::TWO_PI))*exp(-(pow((val - mu), 2) / (2 * pow(sigma, 2))))));
}

Background::Background(float width, float height): width_(width), height_(height)
{
	UpdateStars();
}



void Background::UpdateStars() 
{
	float halfWidth = width_ * 0.5f;
	float halfHeight = height_ * 0.5f;

	float mu = 0.85f, sigma = 0.6f;

	for (int i = 0; i < NUM_STARS; i++)
	{
			stars_[i].x = 2.1f*Gaussian(mu_, sigma, Random::GetFloat(-1, 1))*width_-halfWidth;
			stars_[i].y = 2.1f*Gaussian(mu_, sigma, Random::GetFloat(-1, 1))*height_-halfHeight;
			stars_[i].z = 100.0f;
			stars_[i].diffuse = 0xffffffff;
	}
}

void Background::Update(System *systems)
{
}

void Background::Render(Graphics *graphics) const
{
	graphics->ClearFrame(0.0f, 0.0f, 0.0f, 0.0f);
	graphics->GetImmediateMode()->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
		&stars_[0],
		NUM_STARS);
}

void Background::UpdateMU(const float& mu) 
{
	mu_ = mu;
	UpdateStars();
}
