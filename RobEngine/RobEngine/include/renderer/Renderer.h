#pragma once

#include "renderer/Camera.h"
#include "mathB/Matrice.h"
#include "mathB/Mesh.h"
#include "renderer/Color.h"

namespace sf { class RenderWindow; }

class Renderer
{
private :
	short renderMode = 0;

	Mat4x4 matProj;

	Color wireFrameColor {0,255,255};
	Color clippedColor {255,0,255};

	Vec3 dirLight{ 0,0.5f,1 };
	static std::vector<Mesh*> visibleMesh;

public : 
	enum ERenderMode
	{
		shaded = 1 << 0,
		wireFrame = 1 << 1,
		debug = 1 << 2,
		clipped = 1 << 3
	};

	sf::RenderWindow* window = nullptr;
	//TODO Make it a smart pointer 
	Camera* camera;

public : 
	Renderer(unsigned int height, unsigned int width, std::string name);
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
	static void RegisterMesh(Mesh* mesh);

private : 
	void UpdateMatrices();
	//No Copy >:(
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
};

