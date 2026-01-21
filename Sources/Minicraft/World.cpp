#include "pch.h"
#include "World.h"

void World::Generate(DeviceResources* deviceRes)
{
	for (int z = 0; z < 16; ++z)
	{
		for (int x = 0; x < 16; ++x)
		{
			for (int y = 0; y < 3; ++y)
			{
				auto blockId = GetCube(x, y, z);
				*blockId = STONE;
			}
			for (int y = 3; y < 6; ++y)
			{
				auto blockId = GetCube(x, y, z);
				*blockId = DIRT;
			}
			for (int y = 6; y < 7; ++y)
			{
				auto blockId = GetCube(x, y, z);
				*blockId = GRASS;
			}
		}
	}
	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 16; ++j)
		{
			for (int k = 0; k < 16; ++k)
			{
				auto blockId = GetCube(i, j, k);
				if (blockId == nullptr || *blockId == EMPTY) continue;
				cubes.emplace_back(i, j, k, *blockId).Generate(deviceRes);
			}
		}
	}


	cbModel.Create(deviceRes);
}

void World::Draw(DeviceResources* device)
{
	cbModel.ApplyToVS(device, 0);
	for (Cube& cube : cubes)
	{
		Matrix model = cube.GetModelMatrix();
		cbModel.data.mModel = model.Transpose();
		cbModel.UpdateBuffer(device);
		cube.Draw(device);
	}
}

BlockId* World::GetCube(int globalX, int globalY, int globalZ)
{
	if (globalX < 0) return nullptr;
	if (globalY < 0) return nullptr;
	if (globalZ < 0) return nullptr;
	if (globalY >= 16) return nullptr;
	if (globalX >= 16) return nullptr;
	if (globalZ >= 16) return nullptr;
	return &data[globalX + globalY * 16 + globalZ * 16 * 16];
}

