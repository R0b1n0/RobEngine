#include "Utils/FirstPersonCtrl.h"


FirstPersonCtrl::FirstPersonCtrl() : yawn(0), pitch(0)
{
	InputManager::RegisterMethod(InputManager::KeyCode::Z, [&](InputManager::InputState i)
		{this->HandleDirectionInput(Vec3{ 0,0,1 }); });
	InputManager::RegisterMethod(InputManager::KeyCode::S, [&](InputManager::InputState i)
		{this->HandleDirectionInput(Vec3{ 0,0,-1 }); });
	InputManager::RegisterMethod(InputManager::KeyCode::Q, [&](InputManager::InputState i)
		{this->HandleDirectionInput(Vec3{ -1,0,0 }); });
	InputManager::RegisterMethod(InputManager::KeyCode::D, [&](InputManager::InputState i)
		{this->HandleDirectionInput(Vec3{ 1,0,0 }); });
	InputManager::RegisterMethod(InputManager::KeyCode::Space, [&](InputManager::InputState i)
		{this->HandleDirectionInput(Vec3{ 0,1,0 }); });
	InputManager::RegisterMethod(InputManager::KeyCode::LShift, [&](InputManager::InputState i)
		{this->HandleDirectionInput(Vec3{ 0,-1,0 }); });
}

void FirstPersonCtrl::Update(float dt)
{
	cameraPos += Camera::main->CamRight() * inputDir.x * dt * speed;
	cameraPos += Camera::main->CamUp() * inputDir.y * dt * speed;
	cameraPos += Camera::main->CamForward() * inputDir.z * dt * speed;

	inputDir = Vec3();
	pitch += dt * InputManager::mouseInput.delta.y / 3;
	yawn -= dt * InputManager::mouseInput.delta.x / 3;

	Camera::main->SetRotation(yawn, pitch, 0);
	Camera::main->SetPos(cameraPos);
}

void FirstPersonCtrl::HandleDirectionInput(Vec3 directionInput)
{
	inputDir += directionInput;
}