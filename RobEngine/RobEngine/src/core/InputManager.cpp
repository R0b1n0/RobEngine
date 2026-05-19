#include "core/InputManager.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <SFML/Graphics.hpp>

std::unordered_set<InputManager::KeyCode> InputManager::heldKeys = {};

std::unordered_map<
	InputManager::KeyCode,
	std::vector<
	std::function<void(InputManager::InputState)>>> 
	InputManager::inputDependentMethods = {};

WinMouseInput InputManager::mouseInput = { };

void InputManager::RegisterMethod(KeyCode keycode, std::function<void(InputState)> method)
{
	inputDependentMethods[keycode].push_back(method);
}

void InputManager::ProcessInputs()
{
	bool keyDown;
	//Process Keys
	for (auto& subs : inputDependentMethods)
	{
		keyDown = heldKeys.find(subs.first) != heldKeys.end();

		//is key down?
		if (GetAsyncKeyState(subs.first) & 0x8000)
		{
			if (!keyDown)
			{
				heldKeys.insert(subs.first);

				for (std::function<void(InputState)>& method : subs.second)
				{
					method(InputState::Pressed);
				}
			}
			else
			{
				for (std::function<void(InputState)>& method : subs.second)
				{
					method(InputState::Held);
				}
			}
		}
		else if (keyDown)
		{
			heldKeys.erase(subs.first);
			for (std::function<void(InputState)>& method : subs.second)
			{
				method(InputState::Released);
			}
		}
	}

}

void InputManager::ProcessWindowEvents(const std::optional<sf::Event> event)
{
	if (const auto* mouseMovedRaw = event->getIf<sf::Event::MouseMovedRaw>())
	{
		mouseInput.delta.x = mouseMovedRaw->delta.x;
		mouseInput.delta.y = mouseMovedRaw->delta.y;
	}
}
