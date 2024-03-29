#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <DirectXMath.h>
#include <list>

using namespace DirectX;

class OrthoCamera;
class Background;
class Ship;
class Bullet;
class Asteroid;
class Explosion;
class Collision;
class System;
class Graphics;
class GameEntity;
class UFO;

//Components
class PlayerInputComponent;

class Game
{
public:
	Game();
	~Game();

	void Update(System *system);
	void RenderBackgroundOnly(Graphics *graphics);
	void RenderEverything(Graphics *graphics);

	void InitialiseLevel(int numAsteroids);
	bool IsLevelComplete() const;
	bool IsGameOver() const;

	void DoCollision(GameEntity *a, GameEntity *b);

	const int& GetScore();
	void ResetScore();

	Ship*const GetPlayer() const;
	void WrapEntity(GameEntity *entity) const;
	void SpawnBullet(XMVECTOR position, XMVECTOR direction);
	
private:
	Game(const Game &);
	void operator=(const Game &);

	typedef std::list<Asteroid *> AsteroidList;
	typedef std::list<Explosion *> ExplosionList;

	void SpawnPlayer();
	void DeletePlayer();

	void UpdatePlayer(System *system);
	void UpdateAsteroids(System *system);
	void UpdateBullet(System *system);
	//Update Explosions
	void UpdateExplosions(System* system);

	void DeleteAllAsteroids();
	void DeleteAllExplosions();

	
	void DeleteExplosion(Explosion* explosion);
	//void DeleteBullet();

	void SpawnAsteroids(int numAsteroids);
	void SpawnAsteroidAt(XMVECTOR position, int size);
	bool IsAsteroid(GameEntity *entity) const;
	void AsteroidHit(Asteroid *asteroid);
	void DeleteAsteroid(Asteroid *asteroid);

	void UpdateCollisions();

	bool IsBullet(GameEntity *entity) const;

	//Decrease Player Lives
	void TaxPlayerLives();
	void RenderPlayerLives(Graphics *graphics) const;

	OrthoCamera *camera_;

	Background *background_;
	Ship *player_;
	//Object removed. See Bullet Logic below.
	//Bullet *bullet_;
	AsteroidList asteroids_;
	ExplosionList explosions_;

	Collision *collision_;

	// Bullet Logic
	typedef std::list<Bullet *> BulletList;
	BulletList bullets_;
	
	void DeleteBullet(Bullet *bullet);
	void DeleteAllBullets();

	PlayerInputComponent* playerInput;
	

	//Score Logic
	int score_;

	void RenderScore(const Graphics *const graphics) const;

	//UFO Logic
	UFO *ufo_;
	void SpawnUFO(const int&);
	void DeleteUFO();
	void UpdateUFO(System *system);
};

#endif // GAME_H_INCLUDED
