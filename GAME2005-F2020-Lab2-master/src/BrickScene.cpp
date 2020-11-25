#include "BrickScene.h"
#include "Game.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "Util.h"
#include "imgui.h"
#include "imgui_sdl.h"

BrickScene::BrickScene()
{
	BrickScene::start();
}

BrickScene::~BrickScene()
= default;

void BrickScene::draw()
{
	TextureManager::Instance()->draw("background", 400, 300, 0, 255, true);
	drawDisplayList();

	if(EventManager::Instance().isIMGUIActive())
		GUI_Function();
}

void BrickScene::update()
{
	updateDisplayList();

	// Handle player bounds
	if(m_pPlayer->getTransform()->position.x < 0.0f)
	{
		m_pPlayer->getTransform()->position.x = 0.0f;
		m_pPlayer->getRigidBody()->velocity.x = 0.0f;
	}
	else if(m_pPlayer->getTransform()->position.x > 800.0f)
	{
		m_pPlayer->getTransform()->position.x = 800.0f;
		m_pPlayer->getRigidBody()->velocity.x = 0.0f;
	}
	if(m_pPlayer->getTransform()->position.y < 0.0f)
	{
		m_pPlayer->getTransform()->position.y = 0.0f;
		m_pPlayer->getRigidBody()->velocity.y = 0.0f;
	}
	else if(m_pPlayer->getTransform()->position.y > 600.0f)
	{
		m_pPlayer->getTransform()->position.y = 600.0f;
		m_pPlayer->getRigidBody()->velocity.y = 0.0f;
	}

	// Handle bullet collisions and despawning
	for(auto ball : m_pBallVec)
	{
		if(ball->active)
		{
			auto position = ball->getTransform()->position;
			auto halfWidth = ball->getWidth() * 0.5f;
			auto halfHeight = ball->getHeight() * 0.5f;
			if(ball->isColliding(m_pPlayer))
			{
				SoundManager::Instance().playSound("hit", 0, 1);
			}
			if(position.y + halfHeight > m_screenHeight)
			{
				SoundManager::Instance().playSound("hit", 0, 1);
				ball->collisionResponse(m_screenHeight, glm::vec2(1.0f, -1.0f), 0, 0, m_wallCollisionEnergyLossFactor);
			}
			else if(position.y - halfHeight < 0.0f)
			{
				SoundManager::Instance().playSound("hit", 0, 1);
				ball->collisionResponse(0.0f, glm::vec2(-1.0f, 1.0f), 0, 0, m_wallCollisionEnergyLossFactor);
			}
			if(position.x + halfWidth > m_screenWidth)
			{
				SoundManager::Instance().playSound("hit", 0, 1);
				ball->collisionResponse(m_screenWidth, glm::vec2(-1.0f, 1.0f), 0, 0, m_wallCollisionEnergyLossFactor);
			}
			else if(position.x - halfWidth < 0.0f)
			{
				SoundManager::Instance().playSound("hit", 0, 1);
				ball->collisionResponse(0.0f, glm::vec2(-1.0f, 1.0f), 0, 0, m_wallCollisionEnergyLossFactor);
			}
		}
	}
}

void BrickScene::clean()
{
	removeAllChildren();
}

void BrickScene::handleEvents()
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

void BrickScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/alex-miteva-flying-broom-stick2.png", "broom");
	TextureManager::Instance()->load("../Assets/textures/Forest.jpg", "background");
	SoundManager::Instance().load("../Assets/audio/rainhit.wav", "hit", SoundType::SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/cubedcanada+forest.mp3", "Forest", SoundType::SOUND_MUSIC);
	SoundManager::Instance().playMusic("Forest");
	Mix_VolumeMusic(MIX_MAX_VOLUME / 8);

	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);

	SpawnBall();
}

void BrickScene::SpawnBall()
{
	m_pBallVec.push_back(new Ball);
	addChild(m_pBallVec.back());
	m_pBallVec.back()->getTransform()->position = glm::vec2(rand() % m_screenWidth, rand() % m_screenHeight);
	m_pBallVec.back()->getRigidBody()->velocity = glm::vec2((rand() % 100) - 50, (rand() % 100) - 50);
}

void BrickScene::DespawnBall()
{
	removeChild(m_pBallVec.back());
	m_pBallVec.pop_back();
}

void BrickScene::GUI_Function()
{
	ImGui::NewFrame();
	ImGui::Begin("Options", NULL, ImGuiWindowFlags_MenuBar);
	ImGui::SetWindowSize({ 340, 0 });

	if(ImGui::SliderInt("Number of Balls", &m_numberOfBalls, 1, 100))
	{
		while(m_pBallVec.size() < m_numberOfBalls)
			SpawnBall();
		while(m_pBallVec.size() > m_numberOfBalls)
			DespawnBall();
	}

	ImGui::End();
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}