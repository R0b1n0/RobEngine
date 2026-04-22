#include "../../include/core/InputManager.h"
#include <iostream>
#include <windows.h>
#include <string>

InputManager::InputManager()
{
	mouseInput = {};
}

void InputManager::RegisterMethod(KeyCode keycode, std::function<void(InputState)> method)
{
	inputDependentMetthods[keycode].push_back(method);
}

void InputManager::ProcessInputs()
{

	bool keyDown;
	for (auto& subs : inputDependentMetthods)
	{
		keyDown = heldKeys.find(subs.first) != heldKeys.end();

		if (GetAsyncKeyState(subs.first) & 0x8000)
		{
			if (!keyDown)
			{
				heldKeys.insert(subs.first);

				for (std::function<void(InputState)>& method : subs.second)
				{
					method(InputState::Started);
				}
			}
			else
			{
				for (std::function<void(InputState)>& method : subs.second)
				{
					method(InputState::Ongoing);
				}
			}
		}
		else if (keyDown)
		{
			heldKeys.erase(subs.first);
			for (std::function<void(InputState)>& method : subs.second)
			{
				method(InputState::End);
			}
		}
	}
}