#pragma once 
#include "core/Ticker.h"
#include "renderer/Camera.h"

class FirstPersonCtrl : Ticker
{
private : 
	Vec3 cameraPos;

public : 
	void Update(float dt) override;
};