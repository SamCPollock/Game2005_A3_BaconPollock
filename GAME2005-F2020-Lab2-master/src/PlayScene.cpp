#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
//#include "CollisionManager.h"
#include "SoundManager.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("background", 400, 300, 0, 255, true);
	drawDisplayList();
}

void PlayScene::update()
{
	updateDisplayList();

	// Handle bullet collisions and despawning
	for(auto bullet : m_pBulletPool->active)
	{
		if(bullet->isColliding(m_pPlayer))
		{
			bullet->active = false;
			SoundManager::Instance().playSound("melting", 0, 1);
		}
		else if(bullet->getTransform()->position.y > 650)
		{
			bullet->active = false;
		}
	}
	m_pBulletPool->Clean();

	// Spawn new bullets
	if(SDL_GetTicks() - bulletSpawnTimerStart >= bulletSpawnDuration)
		SpawnBullet();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
	{
		m_pPlayer->moveLeft();
	}
	else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
	{
		m_pPlayer->moveRight();
	}
	else
	{
		m_pPlayer->stopMovingX();
	}


	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W)) {
		m_pPlayer->moveUp();
	}
	else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S)) {
		m_pPlayer->moveDown();
	}
	else
	{
		m_pPlayer->stopMovingY();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}
}

void PlayScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/Forest.jpg", "background");
	SoundManager::Instance().load("../Assets/audio/I'm melting.wav", "melting", SoundType::SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/cubedcanada+forest.mp3", "Forest", SoundType::SOUND_MUSIC);
	SoundManager::Instance().playMusic("Forest");
	Mix_VolumeMusic(MIX_MAX_VOLUME / 8);

	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);

	m_pBulletPool = new BulletPool(10);
	for(auto bullet : m_pBulletPool->inactive)
	{
		addChild(bullet);
	}

	const SDL_Color blue = { 0, 0, 255, 255 };
}

void PlayScene::SpawnBullet()
{
	Bullet* bullet = m_pBulletPool->Spawn();
	if(bullet)
	{
		bullet->getTransform()->position = glm::vec2(50 + rand() % 700, 0);
	}
	bulletSpawnTimerStart = SDL_GetTicks();
}
