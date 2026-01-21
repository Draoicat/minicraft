#ifndef CUBE_H
#define	CUBE_H

#include "pch.h"

#include "Block.h"
#include "Engine/Buffer.h"
#include "Engine/VertexLayout.h"

class Cube
{
public:
	Cube(float x, float y, float z, BlockId data);

	void Generate(DeviceResources* deviceRes);
	void Draw(DeviceResources* deviceRes);

	DirectX::SimpleMath::Matrix GetModelMatrix();

private:
	VertexBuffer<VertexLayout_PositionNormalUV> vertexBuffer;
	IndexBuffer indexBuffer;

	DirectX::SimpleMath::Vector3 position;

	BlockData blockData;
	
	void PushFace(Vector3 pos, Vector3 up, Vector3 right,Vector3 norm, int texId);
};

#endif