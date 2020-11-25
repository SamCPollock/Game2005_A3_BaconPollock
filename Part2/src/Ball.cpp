#include "Ball.h"
#include "TextureManager.h"

Ball::Ball() {
	TextureManager::Instance()->load("../Assets/textures/bullet.png", "ball");

	auto size = TextureManager::Instance()->getTextureSize("ball");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 200.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(BALL);
}

Ball::~Ball()
= default;

void Ball::draw() {
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->draw("ball", x, y, 0, 255, true);
}

void Ball::update() {


	float scale = 3.0f; // TODO: Make this a constant. Also make gravity a constant. 
	float deltaTime = 1.0f / 60;
	getRigidBody()->acceleration = glm::vec2(5, 9.8 * scale);

	getRigidBody()->velocity = getRigidBody()->velocity + (getRigidBody()->acceleration * deltaTime);
	getTransform()->position = getTransform()->position + getRigidBody()->velocity * deltaTime;

	//glm::vec2 gravity = glm::vec2(0, 9.8);
	//float metersPerPixel = 30;
	//getRigidBody()->velocity += (getRigidBody()->acceleration + gravity) * deltaTime;
	//getTransform()->position += (getRigidBody()->velocity * metersPerPixel) * deltaTime; // actually move it

	// gravity
	//getRigidBody()->acceleration = glm::vec2(0, 9.8);

}

void Ball::clean() {

}


bool Ball::isColliding(Sprite* pOther)
{
	// Works for square sprites only
	float myRadius = getWidth() * 0.5f;
	float otherRadius = pOther->getWidth() * 0.5f;

	return (getDistance(pOther) <= myRadius + otherRadius);

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