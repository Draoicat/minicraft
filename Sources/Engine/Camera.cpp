#include "pch.h"
#include "Camera.h"

Camera::Camera(float fov, float aspectRatio) : fov{fov}
{
	UpdateAspectRatio(aspectRatio);
	UpdateViewMatrix();
}

void Camera::Create(DeviceResources* deviceRes)
{
	cbCamera.Create(deviceRes);
}

void Camera::UpdateAspectRatio(float aspectRatio)
{
	proj = Matrix::CreatePerspectiveFieldOfView(
		fov * DirectX::XM_PI / 180.0f,
		aspectRatio,
		nearPlane,
		farPlane
	);
}

void Camera::setPosition(Vector3 pos)
{
	position = pos;
	UpdateViewMatrix();
}

void Camera::setRotation(Quaternion rot)
{
	rotation = rot;
	UpdateViewMatrix();
}

DirectX::SimpleMath::Matrix Camera::GetInverseViewMatrix()
{
	return cbCamera.data.mView.Invert();
}

void Camera::ApplyCamera(DeviceResources* deviceRes)
{
	cbCamera.ApplyToVS(deviceRes, 1);
	
	cbCamera.data.mProj = proj.Transpose();
	cbCamera.data.mView = view.Transpose();

	cbCamera.UpdateBuffer(deviceRes);
}

Vector3 Camera::Forward()
{
	return Vector3::Forward;
}

void Camera::Right()
{
}

void Camera::Up()
{
}

void Camera::UpdateViewMatrix()
{
	Vector3 forward = Vector3::Transform(Vector3::Forward, rotation);
	Vector3 up = Vector3::Transform(Vector3::Up, rotation);

	view = Matrix::CreateLookAt(
		position,
		position + forward,
		up
	);
}
