#pragma once
#include <string>
#include <functional>
#include <optional>

struct WindowParameters
{
	unsigned int height;
	unsigned int width;
	std::string name;
};

namespace sf { class Event; }

class GameState
{
private : 
	static bool running;
	static WindowParameters winParam;
	static std::vector < std::function < void (float)>> tickers;
	static std::vector < std::function < void (const std::optional<sf::Event>)>> windowEventSensitiv;
	
public: 
	GameState() = delete;
	~GameState() = delete;

	static void Run();
	static void SetWindowParameters(WindowParameters win);
	static void RegisterTickFunction(std::function<void(float)> update);
	static void RegisterWindowEventFunction(std::function<void(const std::optional<sf::Event>) > windowEvent);

	static void CloseGame();
};
