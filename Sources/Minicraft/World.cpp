#include "pch.h"
#include "World.h"

void World::Generate(DeviceResources* deviceRes)
{
	
	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 1; ++j)
		{
			for (int k = 0; k < 16; ++k)
			{
				chunks.emplace_back(i * Chunk::CHUNK_SIZE, j * Chunk::CHINK_HEIGHT, k * Chunk::CHUNK_SIZE).Generate(deviceRes);
			}
		}
	}
	cbModel.Create(deviceRes);
}

void World::Draw(DeviceResources* device)
{
	cbModel.ApplyToVS(device, 0);
	for (Chunk& chunk : chunks)
	{
		Matrix model = chunk.GetModelMatrix();
		cbModel.data.mModel = model.Transpose();
		cbModel.UpdateBuffer(device);
		chunk.Draw(device);
	}
}
