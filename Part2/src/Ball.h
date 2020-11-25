#pragma once
#ifndef __BALL__
#define __BALL__

#include "Sprite.h"

class Ball : public Sprite
{
public:
	Ball();
	~Ball();
	float bounciness = -0.9f;

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	bool isColliding(Sprite*);
	float getDistance(Sprite*);
};

#endif


