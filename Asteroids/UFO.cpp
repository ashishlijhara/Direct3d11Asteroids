#include "UFO.h"
#include "Ship.h"
#include "Random.h"
#include "Maths.h"
#include "Graphics.h"
#include "ImmediateMode.h"
#include "ImmediateModeVertex.h"
#include <algorithm>

UFO::UFO(XMVECTOR position,
	float velocity,
	int size, Ship *player) :
	angle_(0.0f),
	size_(size),
	player_(player),
	velocity_(velocity),
	blueMode_(false)
{
	SetPosition(position);

	//XMStoreFloat3(&velocity_, velocity);

	axis_.x = Random::GetFloat(-1.0f, 1.0f);
	axis_.y = Random::GetFloat(-1.0f, 1.0f);
	axis_.z = Random::GetFloat(-1.0f, 1.0f);
	XMStoreFloat3(&axis_, XMVector3Normalize(XMLoadFloat3(&axis_)));

	const float MAX_ROTATION = 0.3f;
	angularSpeed_ = Random::GetFloat(-MAX_ROTATION, MAX_ROTATION);
}

void UFO::Update(System *system)
{
	blueMode_ = false;
	XMVECTOR position = GetPosition();
	direction_ = XMVector3Normalize(player_->GetPosition() - position);
	position = XMVectorAdd(position, direction_*velocity_);
	SetPosition(position);

	angle_ = 0;//Maths::WrapModulo(angle_ + angularSpeed_, Maths::TWO_PI);
}

void UFO::Render(Graphics *graphics) const
{
	const float RADIUS_MULTIPLIER = 5.0f;

	ImmediateModeVertex square[10] =
	{
		{-1.0f, -1.0f, 0.0f, 0xffffffff},
		{-1.0f,  1.0f, 0.0f, 0xffffffff},
		{ 1.0f,  1.0f, 0.0f, 0xffffffff},
		{ 1.0f, -1.0f, 0.0f, 0xffffffff},
		{-1.0f, -1.0f, 0.0f, 0xffffffff},
		{-1.5f, -0.5f, 0.0f, 0xffffffff},
		{-1.5f,  0.5f, 0.0f, 0xffffffff},
		{ 1.5f,  0.5f, 0.0f, 0xffffffff},
		{ 1.5f, -0.5f, 0.0f, 0xffffffff},
		{-1.5f, -0.5f, 0.0f, 0xffffffff},
	};

	ImmediateModeVertex squareSemi[10] =
	{
		{-1.0f, -1.0f, 0.0f, 0xffffff88},
		{-1.0f,  1.0f, 0.0f, 0xffffff88},
		{ 1.0f,  1.0f, 0.0f, 0xffffff88},
		{ 1.0f, -1.0f, 0.0f, 0xffffff88},
		{-1.0f, -1.0f, 0.0f, 0xffffff88},
		{-1.5f, -0.5f, 0.0f, 0xffffff88},
		{-1.5f,  0.5f, 0.0f, 0xffffff88},
		{ 1.5f,  0.5f, 0.0f, 0xffffff88},
		{ 1.5f, -0.5f, 0.0f, 0xffffff88},
		{-1.5f, -0.5f, 0.0f, 0xffffff88},
	};


	XMMATRIX scaleMatrix = XMMatrixScaling(
		size_ * RADIUS_MULTIPLIER,
		size_ * RADIUS_MULTIPLIER,
		size_ * RADIUS_MULTIPLIER);

	XMMATRIX rotationMatrix = XMMatrixRotationAxis(
		XMLoadFloat3(&axis_),
		angle_);

	XMVECTOR position = GetPosition();
	XMMATRIX translationMatrix = XMMatrixTranslation(
		XMVectorGetX(position),
		XMVectorGetY(position),
		XMVectorGetZ(position));

	XMMATRIX asteroidTransform = scaleMatrix *
		rotationMatrix *
		translationMatrix;

	ImmediateMode *immediateGraphics = graphics->GetImmediateMode();

	immediateGraphics->SetModelMatrix(asteroidTransform);

	if (blueMode_)
		std::copy(std::begin(squareSemi), std::end(squareSemi), std::begin(square));
	immediateGraphics->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		&square[0],
		10);
	immediateGraphics->SetModelMatrix(XMMatrixIdentity());
}



const float& UFO::GetVelocity() const
{
	return velocity_;
}

int UFO::GetSize() const
{
	return size_;
}

void UFO::SetBlueMode(const bool &value)
{
	blueMode_ = value;
}
