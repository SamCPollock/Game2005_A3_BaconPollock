#include "BulletPool.h"

BulletPool::BulletPool(unsigned int size) :
	m_uSize(size)
{
	for(unsigned int i = 0; i < size; i++)
	{
		inactive.push_back(new Bullet);
	}
}

Bullet* BulletPool::Spawn()
{
	Bullet* bullet = nullptr;

	if(inactive.size() > 0)
	{
		bullet = inactive.back();
		bullet->active = true;
		bullet->reset();
		inactive.pop_back();
		active.push_back(bullet);
	}
	else
	{
		printf("error");
	}

	return bullet;
}

void BulletPool::Despawn(Bullet* bullet)
{
	bullet->active = false;
	inactive.push_back(bullet);
	std::remove(active.begin(), active.end(), bullet);
	active.erase(active.end() - 1);
}

void BulletPool::Clean()
{
	for(Bullet* bullet : active)
		if(!bullet->active)
			inactive.push_back(bullet);

	active.erase(std::remove_if(active.begin(), active.end(),
		[](const Bullet* bullet)
		{ return bullet->active == false; }), active.end());
}
