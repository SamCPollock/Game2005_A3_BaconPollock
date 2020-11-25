#pragma once
#ifndef __RAIN_SCENE__
#define __RAIN_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Enemy.h"
#include "Bullet.h"
#include "BulletPool.h"
//#include "BulletManager.h"

class RainScene : public Scene
{
public:
	RainScene();
	~RainScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
	void GUI_Function();
private:
	glm::vec2 m_mousePosition;

	// Player
	Uint32 hp_flashTimerDuration = 5000;
	Uint32 hp_flashTimerPeriod = 1000;
	Uint32 hp_flashTimerStart = 0;
	Uint32 hp_flashTimer = 0;
	int current_hp = 5;
	Player* m_pPlayer;
	
	// Bullets
	float bulletSpawnDuration = 1000.0f;
	float bulletSpawnTimerStart;
	int bulletPoolSize = 10;
	float gravity = 9.8f;
	float gravityScale = 4.0f;
	float wind = 0.0f;
	float windFluctuation = 0.0f;
	BulletPool* m_pBulletPool;
	void SpawnBullet();


	Button* m_pBackButton;
	Button* m_pNextButton;
};

#endif /* defined (__RAIN_SCENE__) */