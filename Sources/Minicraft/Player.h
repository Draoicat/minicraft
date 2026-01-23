#ifndef PLAYER_H
#define PLAYER_H

#include "Engine/Camera.h"

class Player
{
	Camera* camera;

public:
	float constexpr static GRAVITY{ 1.0f };

	Player(Camera* camera);
	Vector3 position;

	void update(DirectX::Keyboard::State const& kb, DirectX::Mouse::State const& ms, double dt);
	void applyGravity();
};

#endif
