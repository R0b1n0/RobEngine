#include "renderer/Camera.h"

Camera::Camera(float FNear, float FFar , float Fov , float AspectRatio ) 
	: fNear(FNear), fFar(FFar), fov(Fov), fovRad(1.0f / tanf(Fov * 0.5f / 180.0f * pi)), aspectRatio(AspectRatio) 
{
	SetRotation(Vec3{ 0,0,0 });
}

void Camera::SetRotation(Vec3 rotation)
{
	SetRotation(rotation.x, rotation.y, rotation.z);
}

void Camera::SetRotation(float yawn, float pitch, float roll)
{
	Vec3 up = { 0,1,0 };
	Vec3 target = { 0,0,1 }; //Process target from world origin
	Vec3 rightTarget{ 1,0,0 };
	Mat4x4 targetRotation = Mat4x4::MakeRotationMatrixX(pitch) * Mat4x4::MakeRotationMatrixY(yawn);
	lookDirection = targetRotation.Multiply(target);
	cameraRight = targetRotation.Multiply(rightTarget);
	cameraUp = targetRotation.Multiply(up);
	target = cameraPos + lookDirection;

	Mat4x4 matCamera = Mat4x4::MakePointAt(cameraPos, target, up);
	Mat4x4 viewMatrice = Mat4x4::MatrixQuickInverse(matCamera);

	matView = viewMatrice;
}

void Camera::SetPos(Vec3 pos)
{
	cameraPos = pos;
}
Vec3 Camera::GetPos() const
{
	return cameraPos;
}
