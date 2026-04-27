#pragma once
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include "core/WinMouseInput.h"

class InputManager
{
public:
	//Enum values match key values 
	//https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	enum KeyCode
	{
		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,
		Num0 = 0x30,
		Num1 = 0x31,
		Num2 = 0x32,
		Num3 = 0x33,
		Num4 = 0x34,
		Num5 = 0x35,
		Num6 = 0x36,
		Num7 = 0x37,
		Num8 = 0x38,
		Num9 = 0x39,
		Escape = 0x1B,
		LControl = 0xA2,
		LShift = 0xA0,
		LAlt = 0xA4,
		RControl = 0xA3,
		RShift = 0xA1,
		RAlt = 0xA5,
		Space = 0x20,
		Left = 0x25,
		Right = 0x26,
		Up = 0x27,
		Down = 0x28,
		Numpad0 = 0x60,
		Numpad1 = 0x61,
		Numpad2 = 0x62,
		Numpad3 = 0x63,
		Numpad4 = 0x64,
		Numpad5 = 0x65,
		Numpad6 = 0x66,
		Numpad7 = 0x67,
		Numpad8 = 0x68,
		Numpad9 = 0x69,
		F1 = 0x70,
		F2 = 0x71,
		F3 = 0x72,
		F4 = 0x73,
		F5 = 0x74,
		F6 = 0x75,
		F7 = 0x76,
		F8 = 0x77,
		F9 = 0x78,
		F10 = 0x79,
		F11 = 0x7A,
		F12 = 0x7B,
		F13 = 0x7C,
		F14 = 0x7D,
		F15 = 0x7E,
		Pause = 0x13,
		Mouse0 = 0x01,
		Mouse1 = 0x02,
	};
	enum InputState {
		Pressed,
		Held,
		Released
	};

private:
	static std::unordered_map<
		KeyCode,
		std::vector<
		std::function<void(InputState)>>>
		inputDependentMetthods;

	static std::unordered_set<KeyCode> heldKeys;
	static WinMouseInput mouseInput;

public:
	InputManager() = delete;
	~InputManager() = delete;
	static void RegisterMethod(KeyCode keycode, std::function<void(InputState)> method);
	static void ProcessInputs();
};