#pragma once
#ifndef __BULLET_POOL__
#define __BULLET_POOL__
#include "Bullet.h"
#include <vector>

class BulletPool
{
public:
	BulletPool(unsigned int size);
	Bullet* Spawn();
	void Despawn(Bullet* bullet);
	void Clean();
	void Resize(size_t new_size);
	std::vector<Bullet*> active;
	std::vector<Bullet*> inactive;
private:
	const unsigned int m_uSize;
};

#endif /* defined (__BULLET_POOL__) */