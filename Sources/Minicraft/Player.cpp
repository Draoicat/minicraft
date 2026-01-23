#include "pch.h"

#include "Player.h"

#include "Keyboard.h"
#include "Mouse.h"
#include "SimpleMath.h"

Player::Player(Camera* camera) : 
	camera(camera)
{
	
}

void Player::update(DirectX::Keyboard::State const& kb, DirectX::Mouse::State const& ms, double dt)
{
	DirectX::SimpleMath::Vector3 delta = DirectX::SimpleMath::Vector3::Zero;
	if (kb.Z) delta += camera->Forward();
	if (kb.S) delta -= camera->Forward();
	if (kb.Q) delta -= camera->Right();
	if (kb.D) delta += camera->Right();
	if (kb.Space) delta += camera->Up();
	if (kb.LeftShift) delta -= camera->Up();
	//delta = Vector3::TransformNormal(delta, camera.GetInverseViewMatrix());
	camera->SetPosition(camera->GetPosition() + delta * 10.0f * dt);

	DirectX::SimpleMath::Quaternion rot = camera->GetRotation();
	rot *= DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(camera->Right(), -ms.y * dt * 0.2f);
	rot *= DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, -ms.x * dt * 0.2f);
	camera->SetRotation(rot);
}

void Player::applyGravity()
{

}
