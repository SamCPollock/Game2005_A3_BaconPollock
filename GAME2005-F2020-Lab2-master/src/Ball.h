#pragma once
#ifndef __BALL__
#define __BALL__
#include "Sprite.h"
#include <vector>

enum class CollisionType { Circle, Rectangle, Polygonal };

//struct Line
//{
//	glm::vec2 startPoint;
//	glm::vec2 endPoint;
//};
//
//struct Shape
//{
//	std::vector<Line> lines;
//};

class Ball : public Sprite
{
public:
	Ball();
	~Ball();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	void makePolygonal(float startingAngle, unsigned int sides);
	void collisionResponse(float contact_bound, glm::vec2 normal, float velocity, float mass, float energyLossFactor);
public:
	bool active;
	float bounciness = -0.9f;
	float mass = 10.0f;
	CollisionType shape = CollisionType::Circle;
	std::vector<glm::vec2> shapePoints;
	bool isColliding(Sprite* pOther, CollisionType otherCollisionType);
	float getDistance(Sprite*);
};

#endif /* defined (__BALL__) */