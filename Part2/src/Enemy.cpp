#include "Enemy.h"
#include "TextureManager.h"

Enemy::Enemy() {
	TextureManager::Instance()->load("../Assets/textures/bullet.png", "enemy");

	auto size = TextureManager::Instance()->getTextureSize("enemy");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(700.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(ENEMY);
}

Enemy::~Enemy()
= default;

void Enemy::draw() {
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->draw("enemy", x, y, 0, 255, true);
}

void Enemy::update() {


	float scale = 3.0f; // TODO: Make this a constant. Also make gravity a constant. 
	float deltaTime = 1.0f / 60;
	getRigidBody()->acceleration = glm::vec2(0, 9.8 * scale);

	getRigidBody()->velocity = getRigidBody()->velocity + (getRigidBody()->acceleration * deltaTime);
	getTransform()->position = getTransform()->position + getRigidBody()->velocity * deltaTime;

	//glm::vec2 gravity = glm::vec2(0, 9.8);
	//float metersPerPixel = 30;
	//getRigidBody()->velocity += (getRigidBody()->acceleration + gravity) * deltaTime;
	//getTransform()->position += (getRigidBody()->velocity * metersPerPixel) * deltaTime; // actually move it

	// gravity
	//getRigidBody()->acceleration = glm::vec2(0, 9.8);
	
}

void Enemy::clean() {

}