#pragma once
#ifndef __BALL__
#define __BALL__
#include "Sprite.h"

class Ball : public Sprite
{
public:
	Ball();
	~Ball();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	void collisionResponse(float contact_bound, glm::vec2 normal, float velocity, float mass, float energyLossFactor);
public:
	bool active;
	bool isColliding(Sprite*);
	float getDistance(Sprite*);
};

#endif /* defined (__BALL__) */