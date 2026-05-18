#pragma once
#include "core/GameState.h"
#include <iostream>

class Ticker
{
public : 

	Ticker()
	{
		GameState::RegisterTickFunction([this](float dt){this->Update(dt);});
	}

	//TODO Est ce que avoir Update en private peut quand même permettre de l'appeller
	//dps la gameloop?
	void virtual Update(float deltaTime) {}; 

	//TODO mettre un wrapper qui unsub automatiquement des events
	//Petite class wrapper qui se sub, unsub, et se gère tt seule
};
