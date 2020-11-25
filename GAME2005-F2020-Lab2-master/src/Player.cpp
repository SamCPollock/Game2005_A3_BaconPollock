#include "Player.h"
#include "TextureManager.h"
#include "Util.h"

Player::Player()
{
	TextureManager::Instance()->load("../Assets/textures/witch.png", "circle");
	
	auto size = TextureManager::Instance()->getTextureSize("circle");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLAYER);
}

Player::~Player()
= default;

void Player::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->draw("circle", x, y, 0, 255, true);

	if(visibleForceField)
	{
		glm::vec2 offsetPosition;
		switch(shape)
		{
		case CollisionType::Circle:
			Util::DrawCircle(getTransform()->position, getHeight() * 0.5f, forceFieldColour);
			break;
		case CollisionType::Rectangle:
			offsetPosition = getTransform()->position;
			offsetPosition.x -= getWidth() * 0.5f;
			offsetPosition.y -= getHeight() * 0.5f;
			Util::DrawRect(offsetPosition, getWidth(), getHeight(), forceFieldColour);
			break;
		case CollisionType::Polygonal:
			throw("Error, the witch knows magic, not trigonometry!");
			break;
		default:
			break;
		}
	}
}

void Player::update()
{
	const float deltaTime = 1.0f / 60.f;

	// Normalize direction vector
	float dirMagnitude = Util::magnitude(m_direction);
	if (dirMagnitude > 0) // If a direction key is down
	{
		getRigidBody()->acceleration = Util::normalize(m_direction) * ACCELERATION;
	}
	else if (Util::magnitude(getRigidBody()->velocity) > 0)  // No direction key is down
	{
			getRigidBody()->acceleration = Util::normalize(getRigidBody()->velocity) * -ACCELERATION; // Reduces velocity by ACCELERATION value


			// Stopping the deceleration from overshooting 0.
			if (Util::magnitude(getRigidBody()->velocity) < ACCELERATION)
			{
				getRigidBody()->acceleration = glm::vec2(0.00f, 0.00f);
				getRigidBody()->velocity = glm::vec2(0.00f, 0.00f);
				//std::cout << "STOPPING MOVEMENT\n";
			}
	}

	if (getRigidBody()->isColliding)
	{
		std::cout << "DEBUG: PLAYER COLLIDING" << std::endl;
	}


	getRigidBody()->velocity += getRigidBody()->acceleration;

	glm::vec2 pos = getTransform()->position;
	pos.x += getRigidBody()->velocity.x * deltaTime;
	pos.y += getRigidBody()->velocity.y * deltaTime;


	getTransform()->position = pos;
}

void Player::clean()
{

}

void Player::moveLeft() {
	m_direction.x = -1;
}

void Player::moveRight() {
	m_direction.x = 1;

}

void Player::moveUp() {
	m_direction.y = -1;

}

void Player::moveDown() {
	m_direction.y = 1;

}

void Player::stopMovingX()
{
	m_direction.x = 0;


}

void Player::stopMovingY()
{
	m_direction.y = 0;
}

void Player::stopMoving() {
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
}

bool Player::isColliding(Sprite* pOther) {
	// Works for square sprites only
	float myRadius = getWidth() * 0.5f;
	float otherRadius = pOther->getWidth() * 0.5f;

	return (getDistance(pOther) <= myRadius + otherRadius);
}


float Player::getDistance(Sprite* pOther) {
	glm::vec2 myPos = getTransform()->position;
	glm::vec2 otherPos = pOther->getTransform()->position;

	// Use pythagorean to calculate distance c = sqrt(a^2 + b^2)
	float a = myPos.x - otherPos.x;
	float b = myPos.y - otherPos.y;
	return sqrt(a * a + b * b);
}

