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

	// Handle Wind Fluctuation
	wind += ((rand() % 1000) - 500.0f) * windFluctuation * 0.0001f;
	if(wind < -50.0f) wind = -50.0f;
	if(wind > 50.0f) wind = 50.0f;
	for(Ball* ball : m_pBallVec) ball->getRigidBody()->acceleration.x = wind;

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

	// Handle Ball Collisions
	for(auto ball : m_pBallVec)
	{
		if(ball->active)
		{
			auto position = ball->getTransform()->position;
			auto halfWidth = ball->getWidth() * 0.5f;
			auto halfHeight = ball->getHeight() * 0.5f;
			if(position.y + halfHeight > m_screenHeight)
			{
				SoundManager::Instance().playSound("hit", 0, 1);
				ball->collisionResponse(m_screenHeight, glm::vec2(1.0f, -1.0f), 0, 0, ball->bounciness);
			}
			else if(position.y - halfHeight < 0.0f)
			{
				SoundManager::Instance().playSound("hit", 0, 1);
				ball->collisionResponse(0.0f, glm::vec2(1.0f, -1.0f), 0, 0, ball->bounciness);
			}
			if(position.x + halfWidth > m_screenWidth)
			{
				SoundManager::Instance().playSound("hit", 0, 1);
				ball->collisionResponse(m_screenWidth, glm::vec2(-1.0f, 1.0f), 0, 0, ball->bounciness);
			}
			else if(position.x - halfWidth < 0.0f)
			{
				SoundManager::Instance().playSound("hit", 0, 1);
				ball->collisionResponse(0.0f, glm::vec2(-1.0f, 1.0f), 0, 0, ball->bounciness);
			}

			if(ball->isColliding(m_pPlayer, m_pPlayer->shape))
			{
				SoundManager::Instance().playSound("hit", 0, 1);
				float ballMass = ball->mass;
				float playerMass = m_pPlayer->mass;
				glm::vec2 ballVelocity = ball->getRigidBody()->velocity;
				glm::vec2 playerVelocity = m_pPlayer->getRigidBody()->velocity;
				
				// Account for tunnelling
				//float distance = ball->getDistance(m_pPlayer) - (ball->getHeight() * 0.5f) - (m_pPlayer->getHeight() * 0.5f);
				//ball->getTransform()->position.x

				//float deltaTime = 1.0f / 60.0f;
				//float difference = Util::magnitude(ball->getTransform()->position - m_pPlayer->getTransform()->position);
				//difference -= ball->getHeight() + m_pPlayer->getHeight();
				//glm::vec2 shiftVec = (ball->getRigidBody()->velocity + m_pPlayer->getRigidBody()->velocity) * deltaTime;
				//float xproportion = (difference / shiftVec.x);
				//float yproportion = (difference / shiftVec.y);
				//float proportion = Util::max(xproportion, yproportion) * 2.0f;
				//shiftVec.x *= proportion;
				//shiftVec.y *= proportion;

				// Momentum transfer
				ball->getRigidBody()->velocity = ballVelocity * ((ballMass - playerMass) / (ballMass + playerMass)) + (playerVelocity * ((2 * playerMass) / (ballMass + playerMass)));
				m_pPlayer->getRigidBody()->velocity = ((2 * ballMass) / (ballMass + playerMass)) * ballVelocity + ((playerMass - ballMass) / (ballMass + playerMass)) * playerVelocity;
				ball->getRigidBody()->velocity *= ball->bounciness;
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

	// Last Scene Button
	m_pBackButton = new Button("../Assets/textures/LastScene.png", "LastSceneButton", BACK_BUTTON, glm::vec2(650.0f, 550.0f), true);
	m_pBackButton->addEventListener(CLICK, [&]()-> void {
		m_pBackButton->setActive(false);
		TheGame::Instance()->changeSceneState(RAIN_SCENE); });
	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void { m_pBackButton->setAlpha(128); });
	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void { m_pBackButton->setAlpha(255); });
	addChild(m_pBackButton);

	// Next Scene Button
	m_pNextButton = new Button("../Assets/textures/NextScene.png", "NextSceneButton", NEXT_BUTTON, glm::vec2(750.0f, 550.0f), true);
	m_pNextButton->addEventListener(CLICK, [&]()-> void {
		m_pNextButton->setActive(false);
		TheGame::Instance()->changeSceneState(START_SCENE); });
	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void { m_pNextButton->setAlpha(128); });
	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void { m_pNextButton->setAlpha(255); });
	addChild(m_pNextButton);

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

	static bool chaosifyPosition = true;
	static bool chaosifyVelocity = true;
	static int witchShape = 1;
	static int ballShape = 0;
	static int ballSides = 3;
	static float ballMass = m_pBallVec.at(0)->mass;
	static float ballBounciness = m_pBallVec.at(0)->bounciness;
	static float ballHeightDefault = m_pBallVec.at(0)->getHeight();
	static float ballWidthDefault = m_pBallVec.at(0)->getWidth();
	static float ballHeight = ballHeightDefault;
	static float ballWidth = ballWidthDefault;
	static float ballAngle = 0.0f;

	if(ImGui::Button("Chaosify"))
	{
		for(auto ball : m_pBallVec)
		{
			ball->active = true;
			if(chaosifyPosition)
				ball->getTransform()->position = glm::vec2(rand() % m_screenWidth, rand() % m_screenHeight);
			if(chaosifyVelocity)
				ball->getRigidBody()->velocity = glm::vec2((rand() % 100) - 50, (rand() % 100) - 50);
		}
	}
	ImGui::SameLine(); if(ImGui::Checkbox("Position##ChaosityPosition", &chaosifyPosition));
	ImGui::SameLine(); if(ImGui::Checkbox("Velocity##ChaosityVelocity", &chaosifyVelocity));

	if(ImGui::SliderInt("Number of Balls", &m_numberOfBalls, 1, 100))
	{
		while(m_pBallVec.size() < m_numberOfBalls)
			SpawnBall();
		while(m_pBallVec.size() > m_numberOfBalls)
			DespawnBall();
	}

	if(ImGui::CollapsingHeader("Motion and Control Parameters"))
	{
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.25f);
		if(ImGui::SliderFloat("Witch Mass", &m_pPlayer->mass, 1.0f, 100.0f));
		ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.25f);
		if(ImGui::SliderFloat("Ball Mass", &ballMass, 1.0f, 100.0f))
			for(auto ball : m_pBallVec) ball->mass = ballMass;
		
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.25f);
		if(ImGui::SliderFloat("Ball Bounciness", &ballBounciness, 0.1f, 1.0f))
			for(auto ball : m_pBallVec) ball->bounciness = ballBounciness;

		ImGui::Checkbox("Mouse Move", &m_mouseMove);
		if(m_mouseMove)
		{
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.45f);
			ImGui::SliderFloat("Strength", &m_mouseMoveStrength, 0.1f, 10.0f);

			ImGui::TextWrapped("Please note that the witch will not significantly change her velocity in response to collision if Mouse Move is enabled.");
		}
	}

	if(ImGui::CollapsingHeader("Shape Parameters"))
	{
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.50f);
		if(ImGui::ColorEdit4("Witch's Shield Colour", &m_pPlayer->forceFieldColour[0], 0));

		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.50f);
		if(ImGui::ColorEdit4("Ball Shape Colour", &m_pBallVec.at(0)->shapeColour[0], 0));

		ImGui::Text("Witch's Shield Shape:");
		ImGui::SameLine();
		if(ImGui::RadioButton("Circle##WitchCircle", &witchShape, 0)) m_pPlayer->shape = CollisionType::Circle;
		ImGui::SameLine();
		if(ImGui::RadioButton("Rectangle##WitchRectangle", &witchShape, 1)) m_pPlayer->shape = CollisionType::Rectangle;

		ImGui::Text("Ball Shape:");
		ImGui::SameLine();
		if(ImGui::RadioButton("Circle##BallCircle", &ballShape, 0))
		{
			for(auto ball : m_pBallVec)
			{
				ball->setHeight(ballHeightDefault);
				ball->setWidth(ballWidthDefault);
				ball->shape = CollisionType::Circle;
			}
		}
		ImGui::SameLine();
		if(ImGui::RadioButton("Rectangle##BallRectangle", &ballShape, 1))
			for(auto ball : m_pBallVec) ball->shape = CollisionType::Rectangle;
		ImGui::SameLine();
		if(ImGui::RadioButton("Polygon##BallPolygon", &ballShape, 2))
		{
			for(auto ball : m_pBallVec)
			{
				ball->setHeight(ballHeight);
				ball->setWidth(ballWidth);
				ball->makePolygonal(ballAngle, ballSides);
			}
		}

		if(ballShape == 2)
		{
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.15f);
			if(ImGui::SliderInt("Sides##BallSides", &ballSides, 3, 16))
				for(auto ball : m_pBallVec) ball->makePolygonal(ballAngle, ballSides);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.2f);
			if(ImGui::SliderFloat("Size##BallSize", &ballHeight, 1.0f, 100.0f))
			{
				for(auto ball : m_pBallVec)
				{
					ball->setHeight(ballHeight);
					ball->setWidth(ballWidth);
					ball->makePolygonal(ballAngle, ballSides);
				}
			}
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.2f);
			if(ImGui::SliderFloat("Angle##BallAngle", &ballAngle, 0.0f, 359.9f))
				for(auto ball : m_pBallVec) ball->makePolygonal(ballAngle, ballSides);
		}
	}
	
	if(ImGui::CollapsingHeader("Scene Parameters"))
	{
		if(ImGui::SliderFloat("Gravity", &gravity, 1.0f, 100.0f))
			for(auto ball : m_pBallVec) ball->getRigidBody()->acceleration.y = gravity * gravityScale;
		if(ImGui::SliderFloat("Wind", &wind, -50.0f, 50.0f))
			for(auto ball : m_pBallVec) ball->getRigidBody()->acceleration.x = wind;
		if(ImGui::SliderFloat("Wind Fluctuation", &windFluctuation, 0.0f, 100.0f));
	}

	ImGui::End();
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}