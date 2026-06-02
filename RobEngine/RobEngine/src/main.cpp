#include "main.h"

int main()
{
	GameState::SetWindowParameters(WindowParameters{ winWidth, winHeight, "RobEngine :D" });
	
	Mesh mesh = Mesh();
	mesh.LoadFromObjectFile("assets/Models/mountains.obj");
	Renderer::RegisterMesh(&mesh);

	FirstPersonCtrl fpsCtrl;

	GameState::Run();
}