#include "main.h"

int main()
{
	GameState::SetWindowParameters(WindowParameters{ winWidth, winHeight, "RobEngine :D" });
	
	Mesh mesh = Mesh();
	mesh.LoadFromObjectFile("assets/Models/mountains.obj");
	Renderer::RegisterMesh(&mesh);

	//Camera param
	Vec3 cameraPos{};
	float yawn = 0; //Lacet
	float pitch = 0; //Tang
	float roll = 0; //Roulis
	float speed = 10.0f;
	
	sf::Clock clock;
	
	FirstPersonCtrl fpsCtrl;

	InputManager::RegisterMethod(InputManager::KeyCode::E, [&](InputManager::InputState i) 
		{
			if (i == InputManager::InputState::Pressed)	
				std::cout << "touch started \n";  
			else if (i == InputManager::InputState::Held)
				std::cout << "touch ongoing \n";
			else if (i == InputManager::InputState::Released)
				std::cout << "touch end \n";
		}
	);

	GameState::Run();

	//TODO remove everything below me :O
	Renderer renderer(winWidth, winHeight, "ignore me TT ");

	while (renderer.window->isOpen())
	{
		sf::Vector2i mouseMovement;
		while (const std::optional event = renderer.window->pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				renderer.window->close();

			if (event->is<sf::Event::FocusLost>())
			{
				renderer.window->setMouseCursorGrabbed(false);
				renderer.window->setMouseCursorVisible(true);
			}

			if (event->is<sf::Event::FocusGained>())
			{
				renderer.window->setMouseCursorGrabbed(true);
				renderer.window->setMouseCursorVisible(false);
			}

			if (const auto* mouseMovedRaw = event->getIf<sf::Event::MouseMovedRaw>())
			{
				mouseMovement = mouseMovedRaw->delta;
			}
		}

		//Handle DeltaTime
		sf::Time deltaTime = clock.restart();
		float dt = deltaTime.asSeconds();
		pitch += dt * mouseMovement.y / 3;
		yawn -= dt * mouseMovement.x / 3;

		InputManager::ProcessInputs();

#pragma region Process inputs
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
		{
			cameraPos += renderer.camera->CamForward() * dt * speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			cameraPos -= renderer.camera->CamForward() * dt * speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		{
			cameraPos += renderer.camera->CamUp() * speed * dt;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
		{
			cameraPos -= renderer.camera->CamUp() * speed * dt;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			cameraPos += renderer.camera->CamRight() * dt * speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
		{
			cameraPos -= renderer.camera->CamRight() * dt * speed;
		}
#pragma endregion

		renderer.camera->SetRotation(yawn, pitch, roll);
		renderer.camera->SetPos(cameraPos);

	}
}