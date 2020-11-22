#pragma once
#ifndef __BULLET__
#define __BULLET__
#include "Sprite.h"

class Bullet : public Sprite
{
public:
	Bullet();
	~Bullet();
	void reset();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
public:
	bool active = false;
	bool isColliding(Sprite*);
	float getDistance(Sprite*);
};

#endif //(*__BULLET__)