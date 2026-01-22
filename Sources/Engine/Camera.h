#ifndef CAMERA_H
#define CAMERA_H

#include "Buffer.h"

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Quaternion;

class Camera
{
public:
	Camera(float fov, float aspectRatio);

	void Create(DeviceResources* deviceRes);
	void UpdateAspectRatio(float aspectRatio);

	Vector3 getPosition() { return position; }
	Quaternion getRotation() { return rotation; };
	void setPosition(Vector3 pos);
	void setRotation(Quaternion rot);
	DirectX::SimpleMath::Matrix GetInverseViewMatrix();

	void ApplyCamera(DeviceResources* deviceRes);
	Vector3 Camera::Forward() {return Vector3::TransformNormal(Vector3::Forward, view.Invert());}
	Vector3 Right() { return Vector3::TransformNormal(Vector3::Right, view.Invert()); }
	Vector3 Up() { return Vector3::TransformNormal(Vector3::Up, view.Invert()); }

private:
	float fov{ 60.0f };
	float nearPlane = 0.01f;
	float farPlane = 5000.0f;
	Quaternion rotation{ Quaternion::Identity };
	Vector3 position{ Vector3::Backward * 20 };

	Matrix view;
	Matrix proj;

	struct CameraData {
		DirectX::SimpleMath::Matrix mView;
		DirectX::SimpleMath::Matrix mProj;
	};
	ConstantBuffer<CameraData> cbCamera;

	void UpdateViewMatrix();
};

#endif
