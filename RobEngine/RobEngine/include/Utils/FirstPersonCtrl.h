#pragma once 
#include "core/Ticker.h"
#include "renderer/Camera.h"
#include "core/InputManager.h"
#include <functional>

class FirstPersonCtrl : Ticker
{
private : 
	Vec3 cameraPos;
	Vec3 inputDir;
	float yawn;
	float pitch;
	float speed = 10;

public : 
	FirstPersonCtrl();
	void Update(float dt) override;

private: 
	void HandleDirectionInput(Vec3 directionInput);
};