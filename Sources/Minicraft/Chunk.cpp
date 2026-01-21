#include "pch.h"
#include "Chunk.h"

Chunk::Chunk(float x, float y, float z) : position{x, y, z}
{

}

void Chunk::Generate(DeviceResources* deviceRes)
{
	for (int z = 0; z < CHUNK_SIZE; ++z)
	{
		for (int x = 0; x < CHUNK_SIZE; ++x)
		{
			for (int y = 0; y < CHINK_HEIGHT - 5; ++y)
			{
				auto blockId = GetCubeLocal(x, y, z);
				*blockId = STONE;
			}
			for (int y = CHINK_HEIGHT - 5; y < CHINK_HEIGHT - 1; ++y)
			{
				auto blockId = GetCubeLocal(x, y, z);
				*blockId = DIRT;
			}
			for (int y = CHINK_HEIGHT - 1; y < CHINK_HEIGHT; ++y)
			{
				auto blockId = GetCubeLocal(x, y, z);
				*blockId = GRASS;
			}
		}
	}

	for (int i = 0; i < CHUNK_SIZE; ++i)
	{
		for (int j = 0; j < CHINK_HEIGHT; ++j)
		{
			for (int k = 0; k < CHUNK_SIZE; ++k)
			{
				PushCube(i, j, k);
			}
		}
	}

	vertexBuffer.Create(deviceRes);
	indexBuffer.Create(deviceRes);

}

void Chunk::Draw(DeviceResources* deviceRes)
{
	vertexBuffer.Apply(deviceRes);
	indexBuffer.Apply(deviceRes);
	deviceRes->GetD3DDeviceContext()->DrawIndexed(indexBuffer.Size(), 0, 0);
}

BlockId* Chunk::GetCubeLocal(int chunkX, int chunkY, int chunkZ)
{
	if (chunkX < 0) return nullptr;
	if (chunkY < 0) return nullptr;
	if (chunkZ < 0) return nullptr;
	if (chunkX >= CHUNK_SIZE) return nullptr;
	if (chunkY >= CHINK_HEIGHT) return nullptr;
	if (chunkZ >= CHUNK_SIZE) return nullptr;
	return &data[chunkX + chunkY * CHUNK_SIZE + chunkZ * CHUNK_SIZE * CHINK_HEIGHT];
}

void Chunk::PushFace(Vector3 pos, Vector3 up, Vector3 right, Vector3 norm, int texId)
{
	Vector2 uv(texId % 16, texId / 16);
	pos -= {0.5, 0.5, -0.5};
	int vertexLeftBottom = vertexBuffer.PushVertex({ pos,norm ,(uv + Vector2::UnitY) / 16 });
	int vertexLeftTop = vertexBuffer.PushVertex({ {pos + up},norm ,(uv + Vector2::Zero) / 16 });
	int vertexRightTop = vertexBuffer.PushVertex({ {pos + up + right},norm ,(uv + Vector2::UnitX) / 16 });
	int vertexRightBottom = vertexBuffer.PushVertex({ {pos + right},norm,  (uv + Vector2::One) / 16 });
	indexBuffer.PushTriangle(vertexLeftBottom, vertexLeftTop, vertexRightTop);
	indexBuffer.PushTriangle(vertexRightTop, vertexRightBottom, vertexLeftBottom);
}

void Chunk::PushCube(int x, int y, int z)
{
	Vector3 pos{ (float)x, (float)y, (float) z };
	BlockId* id = GetCubeLocal(x, y, z);
	if (*id == EMPTY) return;
	BlockData data = BlockData::Get(*id);
	PushFace(Vector3::Zero + pos, Vector3::Up, Vector3::Right, Vector3::Backward, data.texIdSide);
	PushFace(Vector3::Right + pos, Vector3::Up, Vector3::Forward, Vector3::Right, data.texIdSide);
	PushFace(Vector3::Forward + Vector3::Right + pos, Vector3::Up, Vector3::Left, Vector3::Forward, data.texIdSide);
	PushFace(Vector3::Forward + pos, Vector3::Up, Vector3::Backward, Vector3::Left, data.texIdSide);
	PushFace(Vector3::Up + pos, Vector3::Forward, Vector3::Right, Vector3::Up, data.texIdTop);
	PushFace(Vector3::Zero + pos, Vector3::Right, Vector3::Forward, Vector3::Down, data.texIdBottom);
}

DirectX::SimpleMath::Matrix Chunk::GetModelMatrix()
{
	return Matrix::CreateTranslation(position);
}

