#include "Ball.h"
#include "TextureManager.h"
#include "Util.h"
#include "CollisionManager.h"

Ball::Ball()
{
	TextureManager::Instance()->load("../Assets/textures/bullet.png", "ball");
	auto size = TextureManager::Instance()->getTextureSize("ball");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 200.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(BALL);
	active = true;
}

Ball::~Ball() = default;

void Ball::draw()
{
	if(shape == CollisionType::Circle)
	{
		TextureManager::Instance()->draw("ball", getTransform()->position.x, getTransform()->position.y, 0, 255, true);
	}
	// Where are our lines?
	else if(shape == CollisionType::Polygonal)
	{
		for(size_t i = 0; i < shapePoints.size() - 1; i++)
			Util::DrawLine(getTransform()->position + shapePoints.at(i), getTransform()->position + shapePoints.at(i + 1));
		Util::DrawLine(getTransform()->position + shapePoints.back(), getTransform()->position + shapePoints.front());
	}
}

void Ball::update()
{
	float deltaTime = 1.0f / 60.0f;
	getRigidBody()->velocity += getRigidBody()->acceleration * deltaTime;
	getTransform()->position += getRigidBody()->velocity * deltaTime;

	if(Util::magnitude(getRigidBody()->velocity) < 0.1f)
		active = false;
}

void Ball::clean()
{
}

void Ball::makePolygonal(float startingAngle, unsigned int sides)
{
	if(sides < 3)
		return;

	shape = CollisionType::Polygonal;
	shapePoints.clear();
	float angleIncrement = 360.0f / sides;
	float angle;
	float radius = getHeight() * 4.5f;
	for(unsigned int i = 0; i < sides; i++)
	{
		angle = angleIncrement * i + startingAngle;
		shapePoints.push_back(glm::vec2(
			radius * 0.5f * cos(glm::radians(angle)),
			radius * 0.5f * sin(glm::radians(angle))));
	}
}

void Ball::collisionResponse(float contact_bound, glm::vec2 normal, float velocity, float mass, float energyLossFactor)
{
	getRigidBody()->velocity *= energyLossFactor;
	getRigidBody()->velocity *= normal;
}

bool Ball::isColliding(Sprite* pOther, CollisionType otherCollisionType)
{
	getRigidBody()->isColliding = false;

	if(shape == CollisionType::Circle && otherCollisionType == CollisionType::Circle)
	{
		float myRadius = getWidth() * 0.5f;
		float otherRadius = pOther->getWidth() * 0.5f;
		getRigidBody()->isColliding = (getDistance(pOther) <= myRadius + otherRadius);
	}
	else if(shape == CollisionType::Rectangle && otherCollisionType == CollisionType::Circle)
	{
		getRigidBody()->isColliding = CollisionManager::circleAABBCheck(pOther, this);
	}
	else if(shape == CollisionType::Circle && otherCollisionType == CollisionType::Rectangle)
	{
		getRigidBody()->isColliding = CollisionManager::circleAABBCheck(this, pOther);
	}
	else if(shape == CollisionType::Rectangle && otherCollisionType == CollisionType::Rectangle)
	{
		getRigidBody()->isColliding = CollisionManager::AABBCheck(this, pOther);
	}
	else if(shape == CollisionType::Polygonal && otherCollisionType == CollisionType::Rectangle)
	{
		glm::vec2 realPos = glm::vec2((pOther->getTransform()->position.x - pOther->getWidth() * 0.5f), (pOther->getTransform()->position.y - pOther->getHeight() * 0.5f));
		
		// Where are our lines?
		for(size_t i = 0; getRigidBody()->isColliding == false, i < shapePoints.size() - 1; i++)
		{
			getRigidBody()->isColliding = CollisionManager::lineRectCheck(getTransform()->position + shapePoints.at(i), getTransform()->position + shapePoints.at(i + 1), realPos, pOther->getWidth(), pOther->getHeight());
			if(getRigidBody()->isColliding)
				std::cout << i << ": " << getRigidBody()->isColliding << " ";
		}
		if(getRigidBody()->isColliding == false)
		{
			getRigidBody()->isColliding = CollisionManager::lineRectCheck(getTransform()->position + shapePoints.back(), getTransform()->position + shapePoints.front(), realPos, pOther->getWidth(), pOther->getHeight());
			if(getRigidBody()->isColliding)
				std::cout << 'f' << ": " << getRigidBody()->isColliding << " ";
		}
	}

	return getRigidBody()->isColliding;
}

float Ball::getDistance(Sprite* pOther)
{
	glm::vec2 myPos = getTransform()->position;
	glm::vec2 otherPos = pOther->getTransform()->position;

	// Use pythagorean to calculate distance c = sqrt(a^2 + b^2)
	float a = myPos.x - otherPos.x;
	float b = myPos.y - otherPos.y;
	return sqrt(a * a + b * b);
}
