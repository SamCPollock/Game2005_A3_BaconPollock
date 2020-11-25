#pragma once
#ifndef __BRICK_SCENE__
#define __BRICK_SCENE__

#include "Scene.h"
#include "Player.h"
#include "Button.h"
#include "Ball.h"

class BrickScene : public Scene
{
public:
	BrickScene();
	~BrickScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
	void GUI_Function();
private:
	glm::vec2 m_mousePosition;

	// Controls
	bool m_mouseMove = true;
	float m_mouseMoveStrength = 1.0f;

	// Player
	Uint32 hp_flashTimerDuration = 5000;
	Uint32 hp_flashTimerPeriod = 1000;
	Uint32 hp_flashTimerStart = 0;
	Uint32 hp_flashTimer = 0;
	int current_hp = 5;
	Player* m_pPlayer;
	
	// Balls
	int m_numberOfBalls = 1;
	std::vector<Ball*> m_pBallVec;
	void SpawnBall();
	void DespawnBall();

	// Walls
	int m_screenWidth = 800;
	int m_screenHeight = 600;
	float m_wallCollisionEnergyLossFactor = 0.9f;

	// Forces
	float gravity = 0.0f;
	float gravityScale = 4.0f;
	float wind = 0.0f;
	float windFluctuation = 0.0f;

	Button* m_pBackButton;
	Button* m_pNextButton;
};

#endif /* defined (__BRICK_SCENE__) */