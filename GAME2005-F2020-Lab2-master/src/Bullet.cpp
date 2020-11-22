#include "Bullet.h"
#include "TextureManager.h"

Bullet::Bullet() {
	TextureManager::Instance()->load("../Assets/textures/raindrop.png", "enemy");

	auto size = TextureManager::Instance()->getTextureSize("enemy");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(700.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(ENEMY);
}

Bullet::~Bullet()
= default;

void Bullet::reset()
{
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 9.8f * 2.0f);
	getRigidBody()->isColliding = false;
	std::cout << "DEBUG: BULLET DROPPING" << std::endl;
}

void Bullet::draw()
{
	if(!active)
		return;

	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->draw("enemy", x, y, 0, 255, true);
}

void Bullet::update()
{
	if(!active)
		return;
	
	float deltaTime = 1.0f / 60.0f;
	getRigidBody()->velocity += getRigidBody()->acceleration * deltaTime;
	getTransform()->position += getRigidBody()->velocity * deltaTime;
	if(getTransform()->position.y >= 650 || getRigidBody()->isColliding)
	{
		//std::cout << "DEBUG: BULLET COLLIDING" << std::endl;
		active = false;
	}

	if(getRigidBody()->isColliding)
	{
		std::cout << "DEBUG: BULLET COLLIDING" << std::endl;

	}
}

void Bullet::clean()
{
}

bool Bullet::isColliding(Sprite* pOther)
{
	// Works for square sprites only
	float myRadius = getWidth() * 0.5f;
	float otherRadius = pOther->getWidth() * 0.5f;

	return (getDistance(pOther) <= myRadius + otherRadius);

}

float Bullet::getDistance(Sprite* pOther)
{
	glm::vec2 myPos = getTransform()->position;
	glm::vec2 otherPos = pOther->getTransform()->position;

	// Use pythagorean to calculate distance c = sqrt(a^2 + b^2)
	float a = myPos.x - otherPos.x;
	float b = myPos.y - otherPos.y;
	return sqrt(a * a + b * b);
}