#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "Sprite.h"
#include "Label.h"
#include "Ball.h"
#include <glm/vec4.hpp>


class Player final : public Sprite
{
public:
	Player();
	~Player();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void stopMovingX();
	void stopMovingY();
	void stopMoving();

	bool isColliding(Sprite*);
	float getDistance(Sprite*);

	float mass = 20.0f;
	CollisionType shape = CollisionType::Rectangle;
	bool visibleForceField = true;
	glm::vec4 forceFieldColour = glm::vec4(0.25f, 1.0f, 0.25f, 1.0f);
private:
	const float ACCELERATION = 10.0f;
	glm::vec2 m_direction;
};

#endif /* defined (__PLAYER__) */