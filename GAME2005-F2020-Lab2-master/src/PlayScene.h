#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Enemy.h"
#include "Bullet.h"
#include "BulletPool.h"
//#include "BulletManager.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
private:
	glm::vec2 m_mousePosition;

	Plane* m_pPlaneSprite;
	Uint32 hp_flashTimerDuration = 5000;
	Uint32 hp_flashTimerPeriod = 1000;
	Uint32 hp_flashTimerStart = 0;
	Uint32 hp_flashTimer = 0;
	int current_hp = 5;
	Player* m_pPlayer;
	Enemy* m_pEnemy;
	
	// Bullets
	float bulletSpawnDuration = 3000.0f;
	float bulletSpawnTimerStart;
	BulletPool* m_pBulletPool;
	void SpawnBullet();


	Button* m_pBackButton;
	Button* m_pNextButton;
};

#endif /* defined (__PLAY_SCENE__) */