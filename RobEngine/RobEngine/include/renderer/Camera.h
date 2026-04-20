#pragma once
#include "../mathB/Utils.h"
#include "../mathB/Vector.h"
#include "../mathB/Matrice.h"

class Camera
{
public : 
	Mat4x4 matView;

private :
	float fNear, fFar, fov, fovRad, aspectRatio = 0;
	Vec3 cameraPos{};
	Vec3 lookDirection{ 0,0,1 };
	Vec3 cameraRight{ 1,0,0 };
	Vec3 cameraUp{ 0,1,0 };

public : 
	Camera(float FNear = 0.1f, float FFar = 1000.0f, float Fov = 50.0f, float AspectRatio = 0) : fNear(FNear), fFar(FFar), fov(Fov), fovRad(1.0f / tanf(fov * 0.5f / 180.0f * pi)), aspectRatio(AspectRatio){}
	
	void SetRotation(Vec3 rotation);
	void SetRotation(float yawn, float pitch, float roll);

#pragma region Getter setter	
	void SetPos(Vec3 pos);
	Vec3 GetPos() const;
	float FNear() { return fNear; }
	float FFar() { return fFar; }
	float FOV() { return fov; }
	float FovRad() { return fovRad; }
	float AspectRatio() { return aspectRatio; }
	Vec3 CamForward() { return lookDirection; }
	Vec3 CamRight() { return cameraRight; }
	Vec3 CamUp() { return cameraUp; }
#pragma endregion

};