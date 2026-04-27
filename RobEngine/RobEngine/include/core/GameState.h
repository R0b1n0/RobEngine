#pragma once
#include <string>
#include <functional>

struct WindowParameters
{
	unsigned int height;
	unsigned int width;
	std::string name;
};

class GameState
{
private : 
	static bool running;
	static WindowParameters winParam;
	static std::vector <std::function<void(float)>> tickers;

public: 
	GameState() = delete;
	~GameState() = delete;

	static void Run();
	static void SetWindowParameters(WindowParameters win);
	static void RegisterTickFunction(std::function<void(float)> update);

	static void CloseGame();
};
