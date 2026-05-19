#include "core/GameState.h"
#include <iostream>
#include "core/InputManager.h"
#include "renderer/Renderer.h"
#include <SFML/Graphics.hpp>

bool GameState::running = false;
WindowParameters GameState::winParam = {1920, 1080, "RobEngine :D"};
std::vector <std::function<void(float)>> GameState::tickers = {};
std::vector<std::function<void(const std::optional<sf::Event>)>> GameState::windowEventSensitiv;

void GameState::Run()
{
	//Load stuff 
	running = true;
	
	Renderer gameWindow{ winParam.height, winParam.width, winParam.name };
	//TODO handle the clock without SFML
	sf::Clock clock;

	//Update stuff 
	while (running)
	{
		//ProcessWindow events
		while (const std::optional event = gameWindow.window->pollEvent())
		{
			InputManager::ProcessWindowEvents(event);
			for (auto& winEvent : windowEventSensitiv)
			{
				winEvent(event);
			}
		}
		InputManager::ProcessInputs();

		float dt = clock.restart().asSeconds();
		for (auto& tick : tickers) 
		{
			//TODO Don't destroy while updating, wait for flush 
			tick(dt);
		}

		//TODO Find a clean way to empty delta value
		InputManager::mouseInput.delta = Vec2();

		gameWindow.Render();
	}

	//Unload stuff
	gameWindow.window->close();
}

void GameState::SetWindowParameters(WindowParameters win)
{
	winParam = win;
}

void GameState::RegisterTickFunction(std::function<void(float)> update)
{
	tickers.push_back(update);
}

void GameState::RegisterWindowEventFunction(std::function<void(const std::optional<sf::Event>)> windowEvent)
{
	windowEventSensitiv.push_back(windowEvent);
}

void GameState::CloseGame()
{
	running = false;
	//TODO Vider la liste de tickers 
}
