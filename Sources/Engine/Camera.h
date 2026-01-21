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

	void setPosition(Vector3 pos);
	void setRotation(Quaternion rot);
	DirectX::SimpleMath::Matrix GetInverseViewMatrix();

	void ApplyCamera(DeviceResources* deviceRes);
	Vector3 Forward();
	void Right();
	void Up();

private:
	float fov{ 60.0f };
	float nearPlane = 0.01f;
	float farPlane = 100.0f;
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
