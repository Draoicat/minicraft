#ifndef WORLD_H
#define WORLD_H

#include "Cube.h"
#include "Block.h"

class World
{
public:
	int static constexpr CHUNK_SIZE{ 16 * 384 * 16 };

	World() = default;

	void Generate(DeviceResources* deviceRes);
	void Draw(DeviceResources* device);

	BlockId* GetCube(int globalX, int globalY, int globalZ);

private:
	struct ModelData
	{
		Matrix mModel;
		Vector4 time;
	};

	ConstantBuffer<ModelData> cbModel;

	BlockId data[CHUNK_SIZE];
	std::vector<Cube> cubes;
};

#endif
