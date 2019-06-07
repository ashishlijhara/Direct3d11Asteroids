#ifndef BACKGROUND_H_INCLUDED
#define BACKGROUND_H_INCLUDED

#include "GameEntity.h"
#include "ImmediateModeVertex.h"

class Background : public GameEntity
{
public:
	Background(float width, float height);

	void Update(System *system);
	void Render(Graphics *graphics) const;
	void UpdateMU(const float& mu);
	
private:

	const float& Gaussian(const float& mu, const float& sigma,const float& val)const;
	enum { NUM_STARS = 256 };
	void UpdateStars();
	ImmediateModeVertex stars_[NUM_STARS];
	float mu_;
	float width_, height_;
};

#endif // BACKGROUND_H_INCLUDED
