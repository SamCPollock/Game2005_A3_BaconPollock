#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "Sprite.h"
#include "Label.h"

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

private:
	const float ACCELERATION = 10.0f;
	glm::vec2 m_direction;
};

#endif /* defined (__PLAYER__) */