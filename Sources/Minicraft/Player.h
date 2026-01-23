#ifndef PLAYER_H
#define PLAYER_H

#include "World.h"
#include "Engine/Camera.h"

class Player
{
	Camera* camera;
	World* world;

	Vector3 position;
	Vector3 velocity;

	float yaw;
	float pitch;

public:
	float constexpr static GRAVITY{ 1.0f };

	Player(Camera* camera, World* world);
	Quaternion rotation;


	void update(DirectX::Keyboard::State const& kb, DirectX::Mouse::State const& ms, double dt);
	Vector3 applyGravity();


};

#endif
