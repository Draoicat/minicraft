#include "pch.h"

#include "Player.h"

#include "Keyboard.h"
#include "Mouse.h"
#include "SimpleMath.h"

Player::Player(Camera* camera, World* world) : 
	camera(camera), world{ world }, yaw {0}, pitch{ 0 }
{
	
}

void Player::update(DirectX::Keyboard::State const& kb, DirectX::Mouse::State const& ms, double dt)
{
	Vector3 delta =Vector3::Zero;
	if (kb.Z) delta += Vector3::Forward;
	if (kb.S) delta -= Vector3::Forward;
	if (kb.Q) delta -= Vector3::Right;
	if (kb.D) delta += Vector3::Right;
	delta.y = 0;
	delta.Normalize(); //incroyable
	delta += applyGravity();
	camera->SetPosition(camera->GetPosition() + delta * 10.0f * dt);

	pitch -= ms.x * dt * 0.2f;
	yaw -= ms.y * dt * 0.2f;
	yaw = std::clamp(yaw, -1.4f, 1.4f);
	Quaternion rot = Quaternion::CreateFromAxisAngle(Vector3::Right, yaw);
	rot *= Quaternion::CreateFromAxisAngle(Vector3::Up, pitch);
	camera->SetRotation(rot);
}

Vector3 Player::applyGravity()
{
	if (*world->GetCube(camera->GetPosition().x, camera->GetPosition().y - 1, camera->GetPosition().z) != EMPTY)
		return Vector3::Zero;
	return Vector3::Down / 2;
}
