#include "../include/main.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode({ winWidth,  winHeight }), "RobEngine");

	Renderer renderer = Renderer(&window);

	//Camera param
	Vec3 cameraPos{};
	Vec3 lookDirection{ 0,0,1 };
	Vec3 cameraRight{ 1,0,0 };
	Vec3 cameraUp{ 0,1,0 };
	Vec3 dirLight{ 0,0.5f,-1 };
	float yawn = 0; //Lacet
	float pitch = 0; //Tang
	float roll = 0; //Roulis 
	float speed = 10.0f;
	
	Vec3 up{ 0.0f, 1.0f,0.0f };
	Vec3 right{ 1.0f, 0.0f,0.0f };
	Vec3 forward{ 0.0f, 0.0f, 1.0f };

	float fTheta = 0;

	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Vector2i mouseMovement;
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();

			if (event->is<sf::Event::FocusLost>())
			{
				window.setMouseCursorGrabbed(false);
				window.setMouseCursorVisible(true);
			}

			if (event->is<sf::Event::FocusGained>())
			{
				window.setMouseCursorGrabbed(true);
				window.setMouseCursorVisible(false);
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

#pragma region Process inputs
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
		{
			cameraPos += lookDirection * dt * speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			cameraPos -= lookDirection * dt * speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		{
			cameraPos += cameraUp * speed * dt;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
		{
			cameraPos -= cameraUp * speed * dt;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			cameraPos += cameraRight * dt * speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
		{
			cameraPos -= cameraRight * dt * speed;
		}
#pragma endregion

		Vec3 up = { 0,1,0 };
		Vec3 target = { 0,0,1 }; //Process target from world origin
		Vec3 rightTarget{ 1,0,0 };
		Mat4x4 targetRotation = Mat4x4::MakeRotationMatrixX(pitch) * Mat4x4::MakeRotationMatrixY(yawn);
		lookDirection = targetRotation.Multiply(target); 
		cameraRight = targetRotation.Multiply(rightTarget);
		cameraUp = targetRotation.Multiply(up);
		target = cameraPos + lookDirection;

		Mat4x4 matCamera = Mat4x4::MakePointAt(cameraPos, target, up);
		Mat4x4 matView = Mat4x4::MatrixQuickInverse(matCamera);

		renderer.camera.matView = matView;
		renderer.camera.cameraPos = cameraPos;

		renderer.Render();
	}
}