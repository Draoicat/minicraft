#include "pch.h"

#include "Cube.h"

Cube::Cube(Vector3 pos) : mModel(Matrix::CreateTranslation(pos)) {
}

void Cube::Generate(DeviceResources* deviceRes) {
	PushFace(Vector3::Zero, Vector3::Up, Vector3::Right);
	PushFace(Vector3::Right, Vector3::Up, Vector3::Forward);
	PushFace(Vector3::Right + Vector3::Forward, Vector3::Up, Vector3::Left);
	PushFace(Vector3::Forward, Vector3::Up, Vector3::Backward);
	PushFace(Vector3::Up, Vector3::Forward, Vector3::Right);
	PushFace(Vector3::Right + Vector3::Forward, Vector3::Left, Vector3::Backward);
	vBuffer.Create(deviceRes);
	iBuffer.Create(deviceRes);
}

void Cube::Draw(DeviceResources* deviceRes) {
	vBuffer.Apply(deviceRes);
	iBuffer.Apply(deviceRes);
	deviceRes->GetD3DDeviceContext()->DrawIndexed(iBuffer.Size(), 0, 0);
}

void Cube::PushFace(Vector3 pos, Vector3 up, Vector3 right) {
	uint32_t bottomLeft = vBuffer.PushVertex(VertexLayout_PositionUV(pos, { 0, 0 }));
	uint32_t bottomRight = vBuffer.PushVertex(VertexLayout_PositionUV(pos + right, { 1, 0 }));
	uint32_t upLeft = vBuffer.PushVertex(VertexLayout_PositionUV(pos + up, { 0, 1 }));
	uint32_t upRight = vBuffer.PushVertex(VertexLayout_PositionUV(pos + up + right, { 1, 1 }));
	iBuffer.PushTriangle(bottomLeft, upLeft, upRight);
	iBuffer.PushTriangle(bottomLeft, upRight, bottomRight);
}
