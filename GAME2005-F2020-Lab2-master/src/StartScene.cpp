#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
= default;

void StartScene::draw()
{
	TextureManager::Instance()->draw("background", 400, 300, 0, 255, true);

	drawDisplayList();
}

void StartScene::update()
{
	updateDisplayList();
}

void StartScene::clean()
{
	removeAllChildren();
}

void StartScene::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(RAIN_SCENE);
	}
}

void StartScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/Forest.jpg", "background");

	const SDL_Color blue = { 0, 0, 255, 255 };
	m_pStartLabel = new Label("ASSIGNMENT 3", "Consolas", 80, blue, glm::vec2(400.0f, 40.0f));
	m_pStartLabel->setParent(this);
	addChild(m_pStartLabel);

	m_pInstructionsLabel = new Label("Kiera Bacon, Sam Pollock", "Consolas", 40, blue, glm::vec2(400.0f, 120.0f));
	m_pInstructionsLabel->setParent(this);
	addChild(m_pInstructionsLabel);

	m_pCreditsLabel = new Label("Thanks to  GabiWolf at VectorStock.com for the forest background! Thanks to SpaceHydra at OpenGameArt.com for the fantasy button art! Thanks to symphony at OpenGameArt.com for the forest music!", "Consolas", 8, blue, glm::vec2(400.0f, 590.0f));
	m_pCreditsLabel->setParent(this);
	addChild(m_pCreditsLabel);

	m_pShip = new Ship();
	m_pShip->getTransform()->position = glm::vec2(400.0f, 300.0f); 
	addChild(m_pShip); 

	// Rain Scene Button
	m_pRainSceneButton = new Button("../Assets/textures/RainScene.png", "RainSceneButton", RAINSCENE_BUTTON, glm::vec2(300.0f, 400.0f), true);

	m_pRainSceneButton->addEventListener(CLICK, [&]()-> void
	{
			m_pRainSceneButton->setActive(false);
		TheGame::Instance()->changeSceneState(RAIN_SCENE);
	});
	
	m_pRainSceneButton->addEventListener(MOUSE_OVER, [&]()->void
	{
			m_pRainSceneButton->setAlpha(128);
	});

	m_pRainSceneButton->addEventListener(MOUSE_OUT, [&]()->void
	{
			m_pRainSceneButton->setAlpha(255);
	});
	addChild(m_pRainSceneButton);

	// Brick Scene Button
	m_pBrickSceneButton = new Button("../Assets/textures/BrickScene.png", "BrickSceneButton", BRICKSCENE_BUTTON, glm::vec2(500.0f, 400.0f), true);

	m_pBrickSceneButton->addEventListener(CLICK, [&]()-> void
		{
			m_pBrickSceneButton->setActive(false);
			TheGame::Instance()->changeSceneState(BRICK_SCENE);
		});

	m_pBrickSceneButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pBrickSceneButton->setAlpha(128);
		});

	m_pBrickSceneButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pBrickSceneButton->setAlpha(255);
		});
	addChild(m_pBrickSceneButton);
}

