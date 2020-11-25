#include "Ball.h"
#include "TextureManager.h"
#include "Util.h"

Ball::Ball()
{
	TextureManager::Instance()->load("../Assets/textures/raindrop.png", "enemy");
	auto size = TextureManager::Instance()->getTextureSize("enemy");
	setWidth(size.x);
	setHeight(size.y);
	getRigidBody()->isColliding = false;
	setType(ENEMY);
	active = true;
}

Ball::~Ball() = default;

void Ball::draw()
{
	TextureManager::Instance()->draw("enemy", getTransform()->position.x, getTransform()->position.y, 0, 255, true);
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

void Ball::collisionResponse(float contact_bound, glm::vec2 normal, float velocity, float mass, float energyLossFactor)
{
	getRigidBody()->velocity *= energyLossFactor;
	getRigidBody()->velocity *= normal;
}

bool Ball::isColliding(Sprite*)
{
	return false;
}

float Ball::getDistance(Sprite*)
{
	return 0.0f;
}
