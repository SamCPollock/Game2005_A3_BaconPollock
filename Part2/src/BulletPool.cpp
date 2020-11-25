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
		printf("Error: Unable to spawn new bullet!\n");
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

void BulletPool::Resize(size_t new_size)
{
	if(new_size < active.size() + inactive.size())
	{
		if(new_size < active.size())
		{
			active.resize(new_size);
			inactive.resize(0);
		}
		else
		{
			new_size -= active.size();
			inactive.resize(new_size);
		}
	}
	else
	{
		while(new_size > active.size() + inactive.size())
		{
			inactive.push_back(new Bullet);
		}
	}
}
