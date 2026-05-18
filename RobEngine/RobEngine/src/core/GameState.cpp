#include "core/GameState.h"
#include <iostream>
#include "core/InputManager.h"
#include "renderer/Renderer.h"
#include <SFML/Graphics.hpp>

bool GameState::running = false;
WindowParameters GameState::winParam = {1920, 1080, "RobEngine :D"};
std::vector <std::function<void(float)>> GameState::tickers = {};

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
		float dt = clock.restart().asSeconds();
		for (auto& tick : tickers)
		{
			//TODO Don't destroy while updating, wait for flush 
			tick(dt);
		}

		InputManager::ProcessInputs();
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

void GameState::CloseGame()
{
	running = false;
	//TODO Vider la liste de tickers 
}
