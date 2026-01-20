#include "pch.h"
#include "Cube.h"

Cube::Cube(float x, float y, float z)
{
	position = { x,y,z };
}

void Cube::Generate(DeviceResources* deviceRes)
{
	PushFace(Vector3::Zero, Vector3::Up, Vector3::Right);
	PushFace(Vector3::Right, Vector3::Up, Vector3::Forward);
	PushFace(Vector3::Forward + Vector3::Right, Vector3::Up, Vector3::Left);
	PushFace(Vector3::Forward, Vector3::Up, Vector3::Backward);
	PushFace(Vector3::Up, Vector3::Forward, Vector3::Right);
	PushFace(Vector3::Zero, Vector3::Right, Vector3::Forward);

	vertexBuffer.Create(deviceRes);
	indexBuffer.Create(deviceRes);
}

void Cube::PushFace(Vector3 pos, Vector3 up, Vector3 right)
{
	int vertexLeftBottom = vertexBuffer.PushVertex({ pos, {0.0f, 0.0f} });
	int vertexLeftTop = vertexBuffer.PushVertex({ {pos + up}, {0.0f, 1.0f} });
	int vertexRightTop = vertexBuffer.PushVertex({{pos + up + right}, {1.0f, 1.0f}});
	int vertexRightBottom = vertexBuffer.PushVertex({{pos + right}, {1.0f, 0.0f}});
	indexBuffer.PushTriangle(vertexLeftBottom, vertexLeftTop, vertexRightTop);
	indexBuffer.PushTriangle(vertexRightTop, vertexRightBottom, vertexLeftBottom);
}

void Cube::Draw(DeviceResources* deviceRes)
{
	vertexBuffer.Apply(deviceRes);
	indexBuffer.Apply(deviceRes);
	deviceRes->GetD3DDeviceContext()->DrawIndexed(36, 0, 0);
}

