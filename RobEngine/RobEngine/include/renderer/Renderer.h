#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "../mathB/Matrice.h"
#include "../mathB/Mesh.h"

class Renderer
{
private :
	short renderMode;
	sf::RenderWindow* window;

	Mat4x4 matProj;
	Mesh mesh;

	sf::Color wireFrameColor = sf::Color::Cyan;
	sf::Color clippedColor = sf::Color::Magenta;

	Vec3 dirLight{ 0,0.5f,-1 };

public : 
	enum ERenderMode
	{
		shaded = 1 << 0,
		wireFrame = 1 << 1,
		debug = 1 << 2,
		clipped = 1 << 3
	};
	Camera camera;

public : 
	Renderer(sf::RenderWindow* window);
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

private : 
	void UpdateMatrices();
};

