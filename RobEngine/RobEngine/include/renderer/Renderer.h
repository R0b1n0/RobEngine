#pragma once

#include "Camera.h"
#include "../mathB/Matrice.h"
#include "../mathB/Mesh.h"
#include "Color.h"

namespace sf { class RenderWindow; }

class Renderer
{
private :
	short renderMode;

	Mat4x4 matProj;

	Color wireFrameColor {0,255,255};
	Color clippedColor {255,0,255};

	Vec3 dirLight{ 0,0.5f,1 };
	std::vector<Mesh*> visibleMesh;

public : 
	enum ERenderMode
	{
		shaded = 1 << 0,
		wireFrame = 1 << 1,
		debug = 1 << 2,
		clipped = 1 << 3
	};
	Camera camera;
	sf::RenderWindow* window;

public : 
	Renderer(unsigned int width, unsigned int height, std::string name);
	~Renderer();
	void SetRenderMode(int mode, bool state = true)
	{
		if (state)
			renderMode |= mode;
		else
			renderMode &= ~mode;
	}
	void ToogleRenderMode(Renderer::ERenderMode mode)
	{
		SetRenderMode(mode, !(renderMode & mode));
	}
	void Render();
	void RegisterMesh(Mesh* mesh);

private : 
	void UpdateMatrices();
};

