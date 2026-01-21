#include "pch.h"
#include "World.h"

void World::Generate(DeviceResources* deviceRes)
{
	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 16; ++j)
		{
			for (int k = 0; k < 16; ++k)
			{
				if (j < 10)cubes.emplace_back(i, j, k, STONE).Generate(deviceRes);
				else if (j < 15)cubes.emplace_back(i, j, k, DIRT).Generate(deviceRes);
				else cubes.emplace_back(i, j, k, GRASS).Generate(deviceRes);
			}
		}
	}
	for (int j = 16; j < 18; ++j)
	{
		for (int i = 5; i < 11; ++i)
		{
			cubes.emplace_back(i, j, 5, WOOD).Generate(deviceRes);
			cubes.emplace_back(i, j, 10, WOOD).Generate(deviceRes);

		}

		for (int i = 6; i < 10; ++i)
		{
			cubes.emplace_back(5, j, i, WOOD).Generate(deviceRes);
			cubes.emplace_back(10, j, i, WOOD).Generate(deviceRes);

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
	return nullptr;
}

