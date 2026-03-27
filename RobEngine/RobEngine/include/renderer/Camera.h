#pragma once
#include "../mathB/Utils.h"
#include "../mathB/Vector.h"
#include "../mathB/Matrice.h"

class Camera
{
public : 
	float fNear, fFar, fov, fovRad, aspectRatio = 0;
	Vec3 cameraPos{};
	Vec3 lookDirection{ 0,0,1 };
	Vec3 cameraRight{ 1,0,0 };
	Vec3 cameraUp{ 0,1,0 };
	float yawn = 0; //Lacet
	float pitch = 0; //Tang
	float roll = 0; //roulis 
	float speed = 10.0f;

	Mat4x4 transform;
	Mat4x4 matView;

public : 
	Camera(float FNear = 0.1f, float FFar = 1000.0f, float Fov = 50.0f, float AspectRatio = 0) : fNear(FNear), fFar(FFar), fov(Fov), fovRad(1.0f / tanf(fov * 0.5f / 180.0f * pi)), aspectRatio(AspectRatio){}
};