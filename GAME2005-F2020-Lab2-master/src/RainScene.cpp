#include "RainScene.h"
#include "Game.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "Util.h"
#include "imgui.h"
#include "imgui_sdl.h"

RainScene::RainScene()
{
	RainScene::start();
}

RainScene::~RainScene()
= default;

void RainScene::draw()
{
	TextureManager::Instance()->draw("background", 400, 300, 0, 255, true);
	for(int hp = 0; hp < current_hp; hp++)
	{
		TextureManager::Instance()->draw("broom", (hp * 20) - 10, 550, 90);
		
		hp_flashTimer = SDL_GetTicks() - hp_flashTimerStart;
		if((current_hp == 1 || hp_flashTimer < hp_flashTimerDuration) && hp_flashTimer % hp_flashTimerPeriod < 500)
			TextureManager::Instance()->setColour("broom", 0, 0, 0);
		else
			TextureManager::Instance()->setColour("broom", 255, 255, 255);
	}
	drawDisplayList();

	if(EventManager::Instance().isIMGUIActive())
		GUI_Function();
}

void RainScene::update()
{
	int wx, wy, mx, my;
	SDL_GetWindowPosition(Game::Instance()->getWindow(), &wx, &wy);
	SDL_GetGlobalMouseState(&mx, &my);
	mx -= wx; my -= wy;

	m_mousePosition = glm::vec2(mx, my);

	// Mouse Control
	if(m_mouseMove)
	{
		float distance = Util::distance(m_pPlayer->getTransform()->position, m_mousePosition);
		float velocityFactor = distance * m_mouseMoveStrength;
		glm::vec2 angle = glm::vec2((m_mousePosition.x - m_pPlayer->getTransform()->position.x), (m_mousePosition.y - m_pPlayer->getTransform()->position.y));
		angle = Util::normalize(angle);
		m_pPlayer->getRigidBody()->velocity = angle * velocityFactor;
	}

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

	// Handle Wind Fluctuation
	wind += ((rand() % 1000) - 500.0f) * windFluctuation * 0.0001f;
	if(wind < -50.0f) wind = -50.0f;
	if(wind > 50.0f) wind = 50.0f;
	for(Bullet* bullet : m_pBulletPool->active) bullet->getRigidBody()->acceleration.x = wind;

	// Handle bullet collisions and despawning
	for(auto bullet : m_pBulletPool->active)
	{
		if(bullet->isColliding(m_pPlayer))
		{
			bullet->active = false;
			--current_hp;
			hp_flashTimerStart = SDL_GetTicks();
			SoundManager::Instance().playSound("hit", 0, 1);
			if(current_hp < 1)
				TheGame::Instance()->changeSceneState(START_SCENE);
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

void RainScene::clean()
{
	removeAllChildren();
}

void RainScene::handleEvents()
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

void RainScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/alex-miteva-flying-broom-stick2.png", "broom");
	TextureManager::Instance()->load("../Assets/textures/Forest.jpg", "background");
	SoundManager::Instance().load("../Assets/audio/rainhit.wav", "hit", SoundType::SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/cubedcanada+forest.mp3", "Forest", SoundType::SOUND_MUSIC);
	SoundManager::Instance().playMusic("Forest");
	Mix_VolumeMusic(MIX_MAX_VOLUME / 8);

	// Last Scene Button
	m_pBackButton = new Button("../Assets/textures/LastScene.png", "LastSceneButton", BACK_BUTTON, glm::vec2(650.0f, 550.0f), true);
	m_pBackButton->addEventListener(CLICK, [&]()-> void {
		m_pBackButton->setActive(false);
		TheGame::Instance()->changeSceneState(START_SCENE); });
	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void { m_pBackButton->setAlpha(128); });
	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void { m_pBackButton->setAlpha(255); });
	addChild(m_pBackButton);

	// Next Scene Button
	m_pNextButton = new Button("../Assets/textures/NextScene.png", "NextSceneButton", NEXT_BUTTON, glm::vec2(750.0f, 550.0f), true);
	m_pNextButton->addEventListener(CLICK, [&]()-> void {
		m_pNextButton->setActive(false);
		TheGame::Instance()->changeSceneState(BRICK_SCENE); });
	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void { m_pNextButton->setAlpha(128); });
	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void { m_pNextButton->setAlpha(255); });
	addChild(m_pNextButton);

	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);

	m_pBulletPool = new BulletPool(bulletPoolSize);
	for(auto bullet : m_pBulletPool->inactive)
	{
		addChild(bullet);
	}
}


void RainScene::SpawnBullet()
{
	Bullet* bullet = m_pBulletPool->Spawn();
	if(bullet)
	{
		bullet->getTransform()->position = glm::vec2(50 + rand() % 700, 0);
		bullet->getRigidBody()->acceleration = glm::vec2(wind, gravity * gravityScale);
	}
	bulletSpawnTimerStart = SDL_GetTicks();
}

void RainScene::GUI_Function()
{
	ImGui::NewFrame();
	ImGui::Begin("Options", NULL, ImGuiWindowFlags_MenuBar);
	ImGui::SetWindowSize({ 340, 0 });

	if(ImGui::SliderInt("Brooms", &current_hp, 1, 40));
	if(ImGui::SliderInt("Raindrops", &bulletPoolSize, 1, 200))
	{
		for(Bullet* bullet : m_pBulletPool->active) removeChild(bullet, false);
		for(Bullet* bullet : m_pBulletPool->inactive) removeChild(bullet, false);
		m_pBulletPool->Resize(bulletPoolSize);
		for(Bullet* bullet : m_pBulletPool->active) addChild(bullet);
		for(Bullet* bullet : m_pBulletPool->inactive) addChild(bullet);
	}
	if(ImGui::SliderFloat("Rain Period", &bulletSpawnDuration, 5000.0f, 100.0f));
	
	ImGui::Checkbox("Mouse Move", &m_mouseMove);
	if(m_mouseMove)
	{
		ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.35f);
		ImGui::SliderFloat("Strength", &m_mouseMoveStrength, 0.1f, 10.0f);
	}

	if(ImGui::SliderFloat("Gravity", &gravity, 1.0f, 100.0f))
	{
		for(Bullet* bullet : m_pBulletPool->active) bullet->getRigidBody()->acceleration.y = gravity * gravityScale;
	}
	if(ImGui::SliderFloat("Wind", &wind, -50.0f, 50.0f))
	{
		for(Bullet* bullet : m_pBulletPool->active) bullet->getRigidBody()->acceleration.x = wind;
	}
	if(ImGui::SliderFloat("Wind Fluctuation", &windFluctuation, 0.0f, 100.0f));

	ImGui::End();
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}