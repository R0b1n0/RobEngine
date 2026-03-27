#include "../include/main.h"

float pi = 3.14159f;
sf::Color wireFrameColor = sf::Color::Cyan;
sf::Color clippedColor = sf::Color::Magenta;

short renderMode;
enum ERenderMode
{
	shaded = 1 << 0,
	wireFrame = 1 << 1,
	debug = 1 << 2,
	clipped = 1 << 3
};

void SetRenderMode(int mode, bool state)
{
	if (state)
		renderMode |= mode;
	else
		renderMode &= ~mode;
}

void ToogleRenderMode(ERenderMode mode)
{
	SetRenderMode(mode, !(renderMode & mode));
}

int main()
{
	sf::RenderWindow window(sf::VideoMode({ winWidth,  winHeight }), "TerrainGen");

	//Rendering Data
	float fNear = 0.1f;
	float fFar = 1000.0f;
	float fov = 50.0f;
	float fovRad = 1.0f / tanf(fov * 0.5f / 180.0f * pi);
	float aspectRatio = (float)winHeight / (float)winWidth;

	//Camera param
	Vec3 cameraPos{};
	Vec3 lookDirection{ 0,0,1 };
	Vec3 cameraRight{ 1,0,0 };
	Vec3 cameraUp{ 0,1,0 };
	Vec3 dirLight{ 0,0.5f,-1 };
	float yawn = 0; //Lacet
	float pitch = 0; //Tang
	float roll = 0; //roulis 
	float speed = 10.0f;
	sf::Vector2i mouseMovement;

	//RenderMode 
	SetRenderMode(ERenderMode::shaded + ERenderMode::wireFrame, true);

	//Set The Matrice
	Mat4x4 matProj = Mat4x4::MakeProjectionMatrix(aspectRatio, fovRad, fNear, fFar);

	//MeshData
	Mesh mesh;
	mesh.LoadFromObjectFile("assets/models/mountains.obj");

	Vec3 up{ 0.0f, 1.0f,0.0f };
	Vec3 right{ 1.0f, 0.0f,0.0f };
	Vec3 forward{ 0.0f, 0.0f, 1.0f };

	float fTheta = 0;

	sf::Clock clock;

	window.setMouseCursorGrabbed(true);
	window.setMouseCursorVisible(false);

	while (window.isOpen())
	{
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
		//fTheta += 0.2f * dt;
		pitch += dt * mouseMovement.y / 3;
		yawn -= dt * mouseMovement.x / 3;

		mouseMovement = sf::Vector2i{ 0,0 };
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

		Mat4x4 matRotZ, matRotX;
		Mat4x4 matTranslation = Mat4x4::MakeTranslationMatrix(0.0f, 0.0f, 15.0f);

		matRotZ = Mat4x4::MakeRotationMatriZ(fTheta);
		matRotX = Mat4x4::MakeRotationMatrixX(-fTheta);

		Mat4x4 world = Mat4x4::MakeIdentity();
		world = matRotZ * matRotX;
		world = world * matTranslation;

		Vec3 up = { 0,1,0 };
		Vec3 target = { 0,0,1 }; //Process target from world origin
		Vec3 rightTarget{ 1,0,0 };
		Mat4x4 targetRotation = Mat4x4::MakeRotationMatrixX(pitch) * Mat4x4::MakeRotationMatrixY(yawn);
		lookDirection = targetRotation.Multiply(target); //Process lookDirection
		cameraRight = targetRotation.Multiply(rightTarget);
		cameraUp = targetRotation.Multiply(up);
		target = cameraPos + lookDirection;

		Mat4x4 matCamera = Mat4x4::MakePointAt(cameraPos, target, up);
		Mat4x4 matView = Mat4x4::MatrixQuickInverse(matCamera);

		std::vector<Triangle> toRender;

		//Process MeshData
		for (auto& tri : mesh.triangles)
		{
			Triangle projected, transformed, viewed;

			//Aply Object Transform
			transformed.p[0] = world.Multiply(tri.p[0]);
			transformed.p[1] = world.Multiply(tri.p[1]);
			transformed.p[2] = world.Multiply(tri.p[2]);

			//Before rendering, check normals
			Vec3 A = transformed.p[1] - transformed.p[0];
			Vec3 B = transformed.p[2] - transformed.p[0];
			Vec3 normal = Vec3::CrossProduct(A, B).Normalize();

			Vec3 cam2Vert = (transformed.p[0] - cameraPos);

			//Check if the triangle is facing the camera 
			if (Vec3::DotProduct(cam2Vert, normal) < 0.0f)
			{
				//Put it in view space 
				viewed.p[0] = matView.Multiply(transformed.p[0]);
				viewed.p[1] = matView.Multiply(transformed.p[1]);
				viewed.p[2] = matView.Multiply(transformed.p[2]);

				//Clip with the near plane 
				int nClippedTriangles = 0;
				Triangle clipped[2];
				nClippedTriangles = ClipTriangleAgainstSpace({ 0.0f, 0.0f, fNear }, { 0.0f, 0.0f, 1.0f }, viewed, clipped[0], clipped[1]);

				//Apply the rest of the algorithm for each new tri
				for (int i = 0; i < nClippedTriangles; i++)
				{
					//Apply projection matrix
					projected.p[0] = matProj.Multiply(clipped[i].p[0]);
					projected.p[1] = matProj.Multiply(clipped[i].p[1]);
					projected.p[2] = matProj.Multiply(clipped[i].p[2]);

					//Manual Perspectiv divide 
					for (int i = 0; i < 3; i++)
					{
						if (projected.p[i].w == 0)
							continue;
						projected.p[i] /= projected.p[i].w;
					}

					//Process light 
					float lightF = Max(0.1f, Min(Vec3::DotProduct(dirLight, normal), 0.9f));

					//Image space 2 ScreenCoordinate
					Vec3 center{ 1,1,0 };
					Vec3 normalize{ 0.5f * (float)winWidth , 0.5f * (float)winHeight, 1.0f };
					for (int i = 0; i < 3; i++)
					{
						projected.p[i] += center;
						projected.p[i] *= normalize;
					}

					projected.col = sf::Color(clipped[i].col.r * lightF, clipped[i].col.g * lightF, clipped[i].col.b * lightF, 255);

					toRender.push_back(projected);
				}
			}
		}

		//Sort triangles 
		sort(toRender.begin(), toRender.end(), [](Triangle& t1, Triangle& t2)
			{
				float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
				float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;

				return z1 > z2;
			});

		window.clear();

		//Render mesh
		for (const Triangle& tri : toRender)
		{
			//clip against image box 
			Triangle clipped[2];
			std::list<Triangle> listTriangles;

			// Add initial triangle
			listTriangles.push_back(tri);
			int nNewTriangles = 1;

			//Genrate window clipped triangles
			for (int p = 0; p < 4; p++)
			{
				int nTrisToAdd = 0;
				while (nNewTriangles > 0)
				{
					Triangle test = listTriangles.front();
					listTriangles.pop_front();
					nNewTriangles--;

					switch (p)
					{
					case 0:
						nTrisToAdd = ClipTriangleAgainstSpace({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 1:
						nTrisToAdd = ClipTriangleAgainstSpace({ 0.0f, (float)winHeight - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 2:
						nTrisToAdd = ClipTriangleAgainstSpace({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 3:
						nTrisToAdd = ClipTriangleAgainstSpace({ (float)winWidth - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					}

					for (int w = 0; w < nTrisToAdd; w++)
						listTriangles.push_back(clipped[w]);
				}
				nNewTriangles = listTriangles.size();
			}

			for (auto& t : listTriangles)
			{
				if (renderMode & ERenderMode::clipped)
				{
					//WireFrame
					sf::VertexArray clipped(sf::PrimitiveType::LineStrip, 4);
					for (int i = 0; i < 4; i++)
					{
						clipped[i].position = sf::Vector2(t.p[i % 3].x, winHeight - t.p[i % 3].y);
						clipped[i].color = clippedColor;
					}
					window.draw(clipped);
				}
				if (renderMode & ERenderMode::shaded)
				{
					//Draw shaded
					sf::VertexArray tri(sf::PrimitiveType::Triangles, 3);
					for (int i = 0; i < 3; i++) {
						tri[i].position = sf::Vector2(t.p[i].x, winHeight - t.p[i].y);
						tri[i].color = t.col;
					}
					window.draw(tri);
				}
			}

			if (renderMode & ERenderMode::wireFrame)
			{
				//WireFrame
				sf::VertexArray wireFrame(sf::PrimitiveType::LineStrip, 4);
				wireFrame[0].position = sf::Vector2(tri.p[0].x, winHeight - tri.p[0].y);
				wireFrame[0].color = wireFrameColor;
				wireFrame[1].position = sf::Vector2(tri.p[1].x, winHeight - tri.p[1].y);
				wireFrame[1].color = wireFrameColor;
				wireFrame[2].position = sf::Vector2(tri.p[2].x, winHeight - tri.p[2].y);
				wireFrame[2].color = wireFrameColor;
				wireFrame[3].position = sf::Vector2(tri.p[0].x, winHeight - tri.p[0].y);
				wireFrame[3].color = wireFrameColor;
				window.draw(wireFrame);
			}
		}

		window.display();
	}
}