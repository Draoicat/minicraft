#ifndef CHUNK_H
#define CHUNK_H

#include <array>

#include "Block.h"
#include "Engine/Buffer.h"
#include "Engine/VertexLayout.h"

class Chunk
{
public:
	int static constexpr CHUNK_SIZE{ 16 };
	int static constexpr CHINK_HEIGHT{ 16 };

	Chunk(float x, float y, float z);

	void Generate(DeviceResources* deviceRes);
	void Draw(DeviceResources* deviceRes);
	
	BlockId* GetCubeLocal(int chunkX, int chunkY, int chunkZ);
	DirectX::SimpleMath::Matrix GetModelMatrix();

private:
	DirectX::SimpleMath::Vector3 position;

	std::array<BlockId, CHUNK_SIZE * CHINK_HEIGHT * CHUNK_SIZE> data;

	VertexBuffer<VertexLayout_PositionNormalUV> vertexBuffer;
	IndexBuffer indexBuffer;

	void PushFace(Vector3 pos, Vector3 up, Vector3 right, Vector3 norm, int texId);
	void PushCube(int x, int y, int z);
	bool ShouldRenderFace(int x, int y, int z, int dx, int dy, int dz);


};

#endif
