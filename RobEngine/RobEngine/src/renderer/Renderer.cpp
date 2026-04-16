#include "../../include/renderer/Renderer.h"
#include <SFML/Graphics.hpp>
#include <cmath>

Renderer::Renderer(sf::RenderWindow* window)
{
	this->window = window;
	camera = Camera(0.1f,1000.0f,50.0f, (float)window->getSize().y / (float)window->getSize().x);
	mesh = Mesh();
	mesh.LoadFromObjectFile("assets/Models/mountains.obj");
	window->setMouseCursorGrabbed(true);
	window->setMouseCursorVisible(false);
	SetRenderMode(ERenderMode::shaded + ERenderMode::wireFrame + ERenderMode::clipped);
	UpdateMatrices();
}

void Renderer::Render()
{
	Mat4x4 matRotZ, matRotX;
	Mat4x4 matTranslation = Mat4x4::MakeTranslationMatrix(0.0f, 0.0f, 15.0f);

	matRotZ = Mat4x4::MakeRotationMatriZ(0);
	matRotX = Mat4x4::MakeRotationMatrixX(-0);

	Mat4x4 world = Mat4x4::MakeIdentity();
	world = matRotZ * matRotX;
	world = world * matTranslation;

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

		Vec3 cam2Vert = (transformed.p[0] - camera.cameraPos);

		//Check if the triangle is facing the camera 
		if (Vec3::DotProduct(cam2Vert, normal) < 0.0f)
		{
			//Put it in view space 
			viewed.p[0] = camera.matView.Multiply(transformed.p[0]);
			viewed.p[1] = camera.matView.Multiply(transformed.p[1]);
			viewed.p[2] = camera.matView.Multiply(transformed.p[2]);

			//Clip with the near plane 
			int nClippedTriangles = 0;
			Triangle clipped[2];
			nClippedTriangles = ClipTriangleAgainstSpace({ 0.0f, 0.0f, camera.fNear }, { 0.0f, 0.0f, 1.0f }, viewed, clipped[0], clipped[1]);

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
				Vec3 normalize{ 0.5f * (float)window->getSize().x , 0.5f * (float)window->getSize().y, 1.0f };
				for (int i = 0; i < 3; i++)
				{
					projected.p[i] += center;
					projected.p[i] *= normalize;
				}

				projected.color = Color(clipped[i].color.r * lightF, clipped[i].color.g * lightF, clipped[i].color.b * lightF, 255);

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

	window->clear();

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
					nTrisToAdd = ClipTriangleAgainstSpace({ 0.0f, (float)window->getSize().y , 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 2:
					nTrisToAdd = ClipTriangleAgainstSpace({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 3:
					nTrisToAdd = ClipTriangleAgainstSpace({ (float)window->getSize().x , 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				}

				for (int w = 0; w < nTrisToAdd; w++)
					listTriangles.push_back(clipped[w]);
			}
			nNewTriangles = listTriangles.size();
		}

		for (auto& t : listTriangles)
		{
			if (renderMode & Renderer::ERenderMode::clipped)
			{
				//WireFrame
				sf::VertexArray clipped(sf::PrimitiveType::LineStrip, 4);
				for (int i = 0; i < 4; i++)
				{
					clipped[i].position = sf::Vector2(t.p[i % 3].x, window->getSize().y - t.p[i % 3].y);
					clipped[i].color = sf::Color(clippedColor.r, clippedColor.g, clippedColor.b);
				}
				window->draw(clipped);
			}
			if (renderMode & Renderer::ERenderMode::shaded)
			{
				//Draw shaded
				sf::VertexArray tri(sf::PrimitiveType::Triangles, 3);
				for (int i = 0; i < 3; i++) {
					tri[i].position = sf::Vector2(t.p[i].x, window->getSize().y - t.p[i].y);
					tri[i].color = sf::Color(t.color.r, t.color.g, t.color.b);
				}
				window->draw(tri);
			}
		}

		if (renderMode & Renderer::ERenderMode::wireFrame)
		{
			//WireFrame
			sf::VertexArray wireFrame(sf::PrimitiveType::LineStrip, 4);
			wireFrame[0].position = sf::Vector2(tri.p[0].x, window->getSize().y - tri.p[0].y);
			wireFrame[0].color = sf::Color(wireFrameColor.r, wireFrameColor.g, wireFrameColor.b);
			wireFrame[1].position = sf::Vector2(tri.p[1].x, window->getSize().y - tri.p[1].y);
			wireFrame[1].color = sf::Color(wireFrameColor.r, wireFrameColor.g, wireFrameColor.b);
			wireFrame[2].position = sf::Vector2(tri.p[2].x, window->getSize().y - tri.p[2].y);
			wireFrame[2].color = sf::Color(wireFrameColor.r, wireFrameColor.g, wireFrameColor.b);
			wireFrame[3].position = sf::Vector2(tri.p[0].x, window->getSize().y - tri.p[0].y);
			wireFrame[3].color = sf::Color(wireFrameColor.r, wireFrameColor.g, wireFrameColor.b);
			window->draw(wireFrame);
		}
	}
	window->display();
}

void Renderer::UpdateMatrices()
{
	matProj = Mat4x4::MakeProjectionMatrix(camera.aspectRatio, camera.fovRad, camera.fNear, camera.fFar);
}