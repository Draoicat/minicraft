#pragma once

#include "Block.h"
#include "Cube.h"
#include "Chunk.h"
#include <array>

class World {
	constexpr static int WORLD_SIZE = 64;
	constexpr static int WORLD_HEIGHT = 20;
	std::array<Chunk, WORLD_SIZE * WORLD_HEIGHT* WORLD_SIZE> chunks;

	struct CubeData {
		Matrix mModel;
	};
	ConstantBuffer<CubeData> cbModel;
public:
	void Generate(DeviceResources* res);
	void Draw(DeviceResources* res, ShaderPass pass);

	BlockId* GetCube(int gx, int gy, int gz);
	void SetCube(int gx, int gy, int gz, BlockId id);

	void ShowImGui(DeviceResources* res);

	std::vector<std::array<int, 3>> Raycast(Vector3 pos, Vector3 dir, float maxDist);
};