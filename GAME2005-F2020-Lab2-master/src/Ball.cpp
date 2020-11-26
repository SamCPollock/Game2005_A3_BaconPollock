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
	if(!active)
		return;

	if(shape == CollisionType::Circle)
	{
		TextureManager::Instance()->draw("ball", getTransform()->position.x, getTransform()->position.y, 0, 255, true);
	}
	else if(shape == CollisionType::Rectangle)
	{
		auto position = getTransform()->position;
		position.x -= getWidth() * 0.5;
		position.y -= getHeight() * 0.5;
		Util::DrawRect(position, getWidth(), getHeight(), shapeColour);
	}
	else if(shape == CollisionType::Polygonal)
	{
		// Where are our lines?
		for(size_t i = 0; i < shapePoints.size() - 1; i++)
			Util::DrawLine(getTransform()->position + shapePoints.at(i), getTransform()->position + shapePoints.at(i + 1), shapeColour);
		Util::DrawLine(getTransform()->position + shapePoints.back(), getTransform()->position + shapePoints.front(), shapeColour);
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
	float radius = getHeight();
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
	float deltaTime = 1.0f / 60.0f;
	float difference;

	if(normal.y > 0.0f) // Collision with vertical bound
	{
		// Determine the horizontal difference between the right side of the ball and the left bound
		if(getRigidBody()->velocity.x > 0.0f)	// Ball is moving right
		{
			difference = getTransform()->position.x + (getWidth() * 0.5f) - contact_bound;
		}
		else									// Ball is moving left
		{
			difference = contact_bound + getTransform()->position.x - (getWidth() * 0.5f);
		}
		// Determine the amount along the vector of ball's velocity that is needed to move such that the difference becomes less than 0.0f;
		// To do this, obtain the proportion of the velocity vector's x component that the difference represents.
		// Then multiply the y component by that same proportion.
		glm::vec2 shiftVec = getRigidBody()->velocity * deltaTime;
		float proportion = (difference / shiftVec.x) * 1.1f;
		shiftVec.x *= proportion;
		shiftVec.y *= proportion;

		// Shift the player along the shift vector
		getTransform()->position -= shiftVec;
	}
	if(normal.x > 0.0f) // Collision with horizontal bound
	{
		// Determine the horizontal difference between the right side of the ball and the left bound
		if(getRigidBody()->velocity.y > 0.0f)	// Ball is moving down
		{
			difference = getTransform()->position.y + (getHeight() * 0.5f) - contact_bound;
		}
		else									// Ball is moving up
		{
			difference = contact_bound + getTransform()->position.y - (getHeight() * 0.5f);
		}
		// Determine the amount along the vector of ball's velocity that is needed to move such that the difference becomes less than 0.0f;
		// To do this, obtain the proportion of the velocity vector's y component that the difference represents.
		// Then multiply the x component by that same proportion.
		glm::vec2 shiftVec = getRigidBody()->velocity * deltaTime;
		float proportion = (difference / shiftVec.y) * 1.1f;
		shiftVec.x *= proportion;
		shiftVec.y *= proportion;

		// Shift the player along the shift vector
		getTransform()->position -= shiftVec;
	}

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
		getRigidBody()->isColliding = CollisionManager::AABBCheck(this, pOther, true);
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
