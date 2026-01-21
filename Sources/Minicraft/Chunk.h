#ifndef CHUNK_H
#define CHUNK_H

class Chunk
{
public:
	int const CHUNK_SIZE{ 16 };
	int const CHINK_HEIGHT{ 384 };

	Chunk() = default;

	void generate();

private:
	std::vector<DirectX::SimpleMath::Vector3> cubes;

};

#endif
