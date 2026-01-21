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
				cubes.emplace_back(i, j, k).Generate(deviceRes);
				
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
	return nullptr;
}

