#include "pch.h"
#include "Cube.h"

Cube::Cube(float x, float y, float z)
{
	position = Vector3( x,y,z );
}

void Cube::Generate(DeviceResources* deviceRes)
{
	PushFace(Vector3::Zero, Vector3::Up, Vector3::Right, {3.0, 0.0});
	PushFace(Vector3::Right, Vector3::Up, Vector3::Forward, {3.0, 0.0});
	PushFace(Vector3::Forward + Vector3::Right, Vector3::Up, Vector3::Left, {3.0, 0.0});
	PushFace(Vector3::Forward, Vector3::Up, Vector3::Backward, {3.0, 0.0});
	PushFace(Vector3::Up, Vector3::Forward, Vector3::Right, {0.0, 0.0});
	PushFace(Vector3::Zero, Vector3::Right, Vector3::Forward, {2.0, 0.0});

	vertexBuffer.Create(deviceRes);
	indexBuffer.Create(deviceRes);
}
 
void Cube::PushFace(Vector3 pos, Vector3 up, Vector3 right, Vector2 texPos)
{
	pos -= {0.5, 0.5, -0.5};
	int vertexLeftBottom = vertexBuffer.PushVertex({ pos, (texPos + Vector2(0.0f, 1.0f)) / 16 });
	int vertexLeftTop = vertexBuffer.PushVertex({ {pos + up}, {(texPos + Vector2(0.0f, 0.0f)) / 16} });
	int vertexRightTop = vertexBuffer.PushVertex({{pos + up + right}, {(texPos +Vector2(1.0f, 0.0f)) / 16}});
	int vertexRightBottom = vertexBuffer.PushVertex({{pos + right}, {(texPos + Vector2(1.0f, 1.0f)) / 16}});
	indexBuffer.PushTriangle(vertexLeftBottom, vertexLeftTop, vertexRightTop);
	indexBuffer.PushTriangle(vertexRightTop, vertexRightBottom, vertexLeftBottom);
}

void Cube::Draw(DeviceResources* deviceRes)
{
	vertexBuffer.Apply(deviceRes);
	indexBuffer.Apply(deviceRes);
	deviceRes->GetD3DDeviceContext()->DrawIndexed(36, 0, 0);
}

