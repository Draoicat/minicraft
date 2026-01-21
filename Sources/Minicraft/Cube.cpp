#include "pch.h"
#include "Cube.h"

Cube::Cube(float x, float y, float z, BlockId data) : blockData{BlockData::Get(data)}
{
	position = Vector3( x,y,z );
}

void Cube::Generate(DeviceResources* deviceRes)
{
	PushFace(Vector3::Zero, Vector3::Up, Vector3::Right, Vector3::Backward, blockData.texIdSide);
	PushFace(Vector3::Right, Vector3::Up, Vector3::Forward, Vector3::Right, blockData.texIdSide);
	PushFace(Vector3::Forward + Vector3::Right, Vector3::Up, Vector3::Left, Vector3::Forward, blockData.texIdSide);
	PushFace(Vector3::Forward, Vector3::Up, Vector3::Backward, Vector3::Left, blockData.texIdSide);
	PushFace(Vector3::Up, Vector3::Forward, Vector3::Right, Vector3::Up, blockData.texIdTop);
	PushFace(Vector3::Zero, Vector3::Right, Vector3::Forward, Vector3::Down, blockData.texIdBottom);

	vertexBuffer.Create(deviceRes);
	indexBuffer.Create(deviceRes);
}
 
void Cube::PushFace(Vector3 pos, Vector3 up, Vector3 right, Vector3 norm, int texId)
{
	Vector2 uv(texId % 16, texId / 16);
	pos -= {0.5, 0.5, -0.5};
	int vertexLeftBottom = vertexBuffer.PushVertex({ pos,norm ,(uv + Vector2::UnitY) / 16 });
	int vertexLeftTop = vertexBuffer.PushVertex({ {pos + up},norm ,( uv + Vector2::Zero ) / 16} );
	int vertexRightTop = vertexBuffer.PushVertex({ {pos + up + right},norm ,(uv + Vector2::UnitX) / 16 });
	int vertexRightBottom = vertexBuffer.PushVertex({{pos + right},norm,  (uv + Vector2::One) / 16});
	indexBuffer.PushTriangle(vertexLeftBottom, vertexLeftTop, vertexRightTop);
	indexBuffer.PushTriangle(vertexRightTop, vertexRightBottom, vertexLeftBottom);
}

void Cube::Draw(DeviceResources* deviceRes)
{
	vertexBuffer.Apply(deviceRes);
	indexBuffer.Apply(deviceRes);
	deviceRes->GetD3DDeviceContext()->DrawIndexed(36, 0, 0);
}

DirectX::SimpleMath::Matrix Cube::GetModelMatrix()
{
	return Matrix::CreateTranslation(position);
}

