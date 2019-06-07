#include "Game.h"
#include "System.h"
#include "OrthoCamera.h"
#include "Background.h"
#include "Ship.h"
#include "UFO.h"
#include "Asteroid.h"
#include "Explosion.h"
#include "Keyboard.h"
#include "Random.h"
#include "Maths.h"
#include "Bullet.h"
#include "Collision.h"
#include "FontEngine.h"
#include "Graphics.h"
#include <crtdbg.h>
#include <string>
#include <algorithm>

Game::Game() :
	camera_(0),
	background_(0),
	player_(0),
	collision_(0),
	//bullet_(0)
	elapsedTime_(0),
	spawnBulletAfter_(1),
	shootMode_(ShootMode::Single),
	score_(0),
	ufo_(0)
{
	camera_ = new OrthoCamera();
	camera_->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	camera_->SetFrustum(800.0f, 600.0f, -100.0f, 100.0f);
	background_ = new Background(800.0f, 600.0f);
	collision_ = new Collision();
}

Game::~Game()
{
	delete camera_;
	delete background_;
	delete player_;
	delete ufo_;
	//DeleteBullet();
	DeleteAllBullets();
	DeleteAllAsteroids();
	DeleteAllExplosions();
	delete collision_;
}

const int& Game::GetScore() { return score_; }

void Game::ResetScore() { score_ = 0; }

void Game::Update(System *system)
{
	UpdatePlayer(system);
	UpdateAsteroids(system);
	UpdateBullet(system);
	UpdateExplosions(system);
	UpdateUFO(system);
	UpdateCollisions();

}

void Game::RenderBackgroundOnly(Graphics *graphics)
{
	camera_->SetAsView(graphics);
	background_->Render(graphics);
}

void Game::RenderEverything(Graphics *graphics)
{
	camera_->SetAsView(graphics);

	background_->Render(graphics);

	if (player_)
	{
		player_->Render(graphics);
		RenderPlayerLives(graphics);
		RenderScore(graphics);
	}

	for (AsteroidList::const_iterator asteroidIt = asteroids_.begin(),
		end = asteroids_.end();
		asteroidIt != end;
		++asteroidIt)
	{
		(*asteroidIt)->Render(graphics);
	}

	for (BulletList::const_iterator bulletIt = bullets_.begin(),end = bullets_.end();
		bulletIt != end;
		++bulletIt)
	{
		if (IsBullet(*bulletIt))
			(*bulletIt)->Render(graphics);
	}
		//bullet_->Render(graphics);

	for (ExplosionList::const_iterator explosionIt = explosions_.begin(),
		end = explosions_.end();
		explosionIt != end;
		++explosionIt)
	{
		(*explosionIt)->Render(graphics);
	}

	if (ufo_) {
		ufo_->Render(graphics);
	}
}

void Game::RenderPlayerLives(Graphics *graphics)  const
{
	FontEngine *fontEngine = graphics->GetFontEngine();

	char lives[10] = "Lives: ";
	char lifeint[2];
	sprintf_s(lifeint, sizeof(lifeint),"%d",player_->PlayerLives());
	strcat_s(lives, lifeint);
	const char *livesText = lives;
	int textWidth = fontEngine->CalculateTextWidth(livesText, FontEngine::FONT_TYPE_SMALL);
	int textX = textWidth-10;
	int textY = 22;
	fontEngine->DrawText(livesText, textX, textY, 0xff00ffff, FontEngine::FONT_TYPE_SMALL);
}

void Game::RenderScore(Graphics *graphics)  const
{
	FontEngine *fontEngine = graphics->GetFontEngine();

	char score[1000] = "Score: ";
	char scoreint[4];
	sprintf_s(scoreint, sizeof(scoreint), "%d", score_);
	strcat_s(score, scoreint);
	const char *livesText = score;
	int textWidth = fontEngine->CalculateTextWidth(livesText, FontEngine::FONT_TYPE_SMALL);
	int textX = 800 - textWidth*2;
	int textY = 22;
	fontEngine->DrawText(livesText, textX, textY, 0xff00ffff, FontEngine::FONT_TYPE_SMALL);
}

void Game::InitialiseLevel(int level)
{
	int numAsteroids = level;
	DeleteAllAsteroids();
	DeleteAllExplosions();
	//DeleteBullets on Screen
	DeleteAllBullets();

	SpawnPlayer();
	SpawnAsteroids(numAsteroids);
	SpawnUFO(level);
}

bool Game::IsLevelComplete() const
{
	return (asteroids_.empty() && explosions_.empty() && !ufo_);
}

bool Game::IsGameOver() const
{
	return (player_ == 0 && explosions_.empty());
}

bool Game::IsBullet(GameEntity *entity) const
{
	return (std::find(bullets_.begin(), bullets_.end(), entity) != bullets_.end());
}

void Game::DoCollision(GameEntity *a, GameEntity *b)
{
	Ship *player = static_cast<Ship *>(a == player_ ? a : (b == player_ ? b : 0));
	Bullet *bullet = static_cast<Bullet *>(IsBullet(a) ? a : ((IsBullet(b)) ? b : 0));
	//Bullet *bullet = static_cast<Bullet *>(a == bullet_ ? a : (b == bullet_ ? b : 0));
	Asteroid *asteroid = static_cast<Asteroid *>(IsAsteroid(a) ? a : (IsAsteroid(b) ? b : 0));
	UFO* ufo = static_cast<UFO*>(a == ufo_ ? a : (b == ufo_ ? b : 0));

	if (player && asteroid)
	{
		AsteroidHit(asteroid);
		//DeletePlayer();
		TaxPlayerLives();
	}

	if (bullet && asteroid)
	{
		score_++;
		AsteroidHit(asteroid);
		DeleteBullet(bullet);
	}

	if (player && ufo) 
	{
		TaxPlayerLives();
		DeleteUFO();
	}
	
	if (asteroid && ufo)
	{
		//UFO Has the ability of going through Asteroids.
		ufo->SetBlueMode(true);
	}

	if (bullet && ufo)
	{
		score_++;
		DeleteUFO();
	}
}

void Game::TaxPlayerLives() {
	int lives = player_->PlayerLives(true);
	if (lives == 0) {
		DeletePlayer();
		return;
	}
	player_->Reset();
}

void Game::SpawnPlayer()
{
	DeletePlayer();
	player_ = new Ship();
	player_->EnableCollisions(collision_, 10.0f);
}



void Game::DeletePlayer()
{
	delete player_;
	player_ = 0;
}

void Game::UpdatePlayer(System *system)
{
	if (player_ == 0)
		return;

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
	WrapEntity(player_);

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
			SpawnBullet(bulletPosition, playerForward);
			break;
		case ShootMode::Spiral:
			shootRot_ += 25;
			SpawnBullet(bulletPosition, RotateVectorBy(shootRot_));
			break;
		case ShootMode::MultiSpiral:
			shootRot_ += 1.5;
			SpawnBullet(bulletPosition, RotateVectorBy(shootRot_));
			break;
		case ShootMode::Three:
			SpawnBullet(bulletPosition, playerForward);
			SpawnBullet(bulletPosition,RotateVectorBy(45));
			SpawnBullet(bulletPosition, RotateVectorBy(-45));
			break;
		}
	}
	elapsedTime_ += (double)system->GetFrameDeltaTime()/100;
}

XMVECTOR Game::RotateVectorBy(float angle) const{
	float rotation = Maths::WrapModulo(player_->GetRotation() + angle, Maths::TWO_PI);
	XMFLOAT3 playerLeft;
	XMMATRIX rotationMatrix = XMMatrixRotationZ(rotation);
	XMVECTOR rotatedVec = XMVector3TransformNormal(XMVectorSet(0.f, 1.0f, 0.0f, 0.0f), rotationMatrix);
	rotatedVec = XMVector3Normalize(rotatedVec);
	return rotatedVec;
}

void Game::UpdateAsteroids(System *system)
{
	for (AsteroidList::const_iterator asteroidIt = asteroids_.begin(),
		end = asteroids_.end();
		asteroidIt != end;
	++asteroidIt)
	{
		(*asteroidIt)->Update(system);
		WrapEntity(*asteroidIt);
	}
}

void Game::UpdateBullet(System *system)
{
	BulletList bulletList_(bullets_.begin(), bullets_.end());
	for (BulletList::const_iterator bulletIt = bulletList_.begin(),end = bulletList_.end();
		bulletIt != end;
		++bulletIt)
	{
		(*bulletIt)->Update(system);
		WrapEntity(*bulletIt);
		
		if ((*bulletIt)->HasCompletedLifeSpan())
			DeleteBullet(*bulletIt);
	}

	/*if (bullet_)
	{
		bullet_->Update(system);
		WrapEntity(bullet_);
	}*/
}

void Game::UpdateExplosions(System* system) {
	ExplosionList explosionList(explosions_.begin(), explosions_.end());
	for (ExplosionList::const_iterator explosionIt = explosionList.begin(),
		end = explosionList.end();
		explosionIt != end;
		++explosionIt)
	{
		(*explosionIt)->Update(system);
		if ((*explosionIt)->HasCompleteLifeSpan()) {
			DeleteExplosion(*explosionIt);
		}
	}
}

void Game::WrapEntity(GameEntity *entity) const
{
	XMFLOAT3 entityPosition;
	XMStoreFloat3(&entityPosition, entity->GetPosition());
	entityPosition.x = Maths::WrapModulo(entityPosition.x, -400.0f, 400.0f);
	entityPosition.y = Maths::WrapModulo(entityPosition.y, -300.0f, 300.0f);
	entity->SetPosition(XMLoadFloat3(&entityPosition));
}

void Game::DeleteAllAsteroids()
{
	for (AsteroidList::const_iterator asteroidIt = asteroids_.begin(),
		end = asteroids_.end();
		asteroidIt != end;
		++asteroidIt)
	{
		delete (*asteroidIt);
	}

	asteroids_.clear();
}

void Game::DeleteAllExplosions()
{
	for (ExplosionList::const_iterator explosionIt = explosions_.begin(),
		end = explosions_.end();
		explosionIt != end;
	++explosionIt)
	{
		delete (*explosionIt);
	}
	explosions_.clear();
}

void Game::SpawnBullet(XMVECTOR position, XMVECTOR direction)
{
	//DeleteBullet();
	Bullet *bullet_ = new Bullet(position, direction);
	bullet_->EnableCollisions(collision_, 3.0f);
	bullets_.push_back(bullet_);
}

void Game::DeleteAllBullets()
{
	for (BulletList::const_iterator bulletIt = bullets_.begin(), end = bullets_.end();
		bulletIt != end;
		++bulletIt)
	{

		delete (*bulletIt);
	}

	bullets_.clear();

	/*delete bullet_;
	bullet_ = 0;*/
}


void Game::DeleteBullet( Bullet *bullet)
{
		bullets_.remove(bullet);
		delete bullet;
}

void Game::DeleteExplosion(Explosion* explosion) {
	explosions_.remove(explosion);
	delete explosion;
}

void Game::SpawnAsteroids(int numAsteroids)
{
	float halfWidth = 800.0f * 0.5f;
	float halfHeight = 600.0f * 0.5f;
	for (int i = 0; i < numAsteroids; i++)
	{
		float x = Random::GetFloat(-halfWidth, halfWidth);
		float y = Random::GetFloat(-halfHeight, halfHeight);
		XMVECTOR position = XMVectorSet(x, y, 0.0f, 0.0f);
		SpawnAsteroidAt(position, 3);
	}
}

void Game::SpawnUFO(const int &level) {
	DeleteUFO();
	if (level % 2 == 0) {
		float width = 800.0f;
		float height = 600.0f;
		float x = Random::GetFloat(-width, width);
		float y = Random::GetFloat(-height, height);
		XMVECTOR position = XMVectorSet(x, y, 0.0f, 0.0f);
		float angle = Random::GetFloat(Maths::TWO_PI);
		XMMATRIX randomRotation = XMMatrixRotationZ(angle);
		float velocity = 0.5f+level/2;
		ufo_ = new UFO(position, velocity, 1, player_);
		ufo_->EnableCollisions(collision_, 1.5f);
	}
}

void Game::UpdateUFO(System *system)
{
	if (ufo_) 
	{
		ufo_->Update(system);
	}
}

void Game::DeleteUFO()
{
		delete ufo_;
		ufo_ = 0;
}


void Game::SpawnAsteroidAt(XMVECTOR position, int size)
{
	const float MAX_ASTEROID_SPEED = 1.0f;

	float angle = Random::GetFloat(Maths::TWO_PI);
	XMMATRIX randomRotation = XMMatrixRotationZ(angle);
	XMVECTOR velocity = XMVectorSet(0.0f, Random::GetFloat(MAX_ASTEROID_SPEED), 0.0f, 0.0f);
	velocity = XMVector3TransformNormal(velocity, randomRotation);

	Asteroid *asteroid = new Asteroid(position, velocity, size);
	asteroid->EnableCollisions(collision_, size * 5.0f);
	asteroids_.push_back(asteroid);
}

bool Game::IsAsteroid(GameEntity *entity) const
{
	return (std::find(asteroids_.begin(),
		asteroids_.end(), entity) != asteroids_.end()); 
}

void Game::AsteroidHit(Asteroid *asteroid)
{
	int oldSize = asteroid->GetSize();
	if (oldSize > 1)
	{
		int smallerSize = oldSize -1;
		XMVECTOR position = asteroid->GetPosition();
		SpawnAsteroidAt(position, smallerSize);
		SpawnAsteroidAt(position, smallerSize);
	}
	explosions_.push_back(new Explosion(asteroid->GetPosition()));
	DeleteAsteroid(asteroid);
}

void Game::DeleteAsteroid(Asteroid *asteroid)
{
	asteroids_.remove(asteroid);
	delete asteroid;
}

void Game::UpdateCollisions()
{
	collision_->DoCollisions(this);
}
