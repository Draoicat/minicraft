//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

#include "PerlinNoise.hpp"
#include "Engine/Shader.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

struct ModelData
{
	Matrix mModel;
};

struct CameraData
{
	Matrix mView;
	Matrix mProjection;
};

// Global stuff
Shader* basicShader;

ComPtr<ID3D11Buffer> vertexBuffer;
ComPtr<ID3D11Buffer> indexBuffer;
ComPtr<ID3D11InputLayout> inputLayout;
ComPtr<ID3D11Buffer> modelBuffer;
ComPtr<ID3D11Buffer> cameraBuffer;

Matrix mProjection;

// Game
Game::Game() noexcept(false) {
	m_deviceResources = std::make_unique<DeviceResources>(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT, 2);
	m_deviceResources->RegisterDeviceNotify(this);
}

Game::~Game() {
	delete basicShader;
	g_inputLayouts.clear();
}

void Game::Initialize(HWND window, int width, int height) {
	// Create input devices
	m_gamePad = std::make_unique<GamePad>();
	m_keyboard = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(window);

	// Initialize the Direct3D resources
	m_deviceResources->SetWindow(window, width, height);
	m_deviceResources->CreateDeviceResources();
	m_deviceResources->CreateWindowSizeDependentResources();

	basicShader = new Shader(L"Basic");
	basicShader->Create(m_deviceResources.get());

	mProjection = Matrix::CreatePerspectiveFieldOfView(60 * XM_PI / 180.0f, (float)width / (float)height, 0.01f, 100.0f);

	auto device = m_deviceResources->GetD3DDevice();

	const std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementDescs = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	device->CreateInputLayout(
		InputElementDescs.data(), InputElementDescs.size(),
		basicShader->vsBytecode.data(), basicShader->vsBytecode.size(),
		inputLayout.ReleaseAndGetAddressOf()
	);

	// MODEL AND CAMERA
	CD3D11_BUFFER_DESC modelDesc(
		sizeof(ModelData),
		D3D11_BIND_CONSTANT_BUFFER
	);

	CD3D11_BUFFER_DESC cameraDesc(
		sizeof(CameraData),
		D3D11_BIND_CONSTANT_BUFFER
	);

	device->CreateBuffer(
		&modelDesc,nullptr, modelBuffer.ReleaseAndGetAddressOf()
	);

	device->CreateBuffer(
		&cameraDesc, nullptr, cameraBuffer.ReleaseAndGetAddressOf()
	);

	//// RECTANGLE
	//std::vector<float> data = {
	//	-0.5f, 0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 
	//	0.5f, 0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 
	//	0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 
	//	-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 1.0f 
	//};

	//CUBE

	std::vector<float> cube = {
		-3.0f, 3.0f, -3.0f,
		3.0f, 3.0f, -3.0f,
		-3.0f, -3.0f, -3.0,
		3.0f, -3.0f, -3.0f,
		- 3.0f, 3.0f, 3.0f,
		3.0f, 3.0f, 3.0f,
		-3.0f, -3.0f, 3.0f,
		3.0f, -3.0f, 3.0f
	};

	CD3D11_BUFFER_DESC desc(
		sizeof(float) * cube.size(),
		D3D11_BIND_VERTEX_BUFFER
	);

	D3D11_SUBRESOURCE_DATA subResData = {};
	subResData.pSysMem = cube.data();

	device->CreateBuffer(
		&desc, &subResData, vertexBuffer.ReleaseAndGetAddressOf()
	);

	//std::vector<uint32_t> indexes = { 0, 1, 2, 2, 3, 0 };

	std::vector<uint32_t> indexesCube = {
		3,1,2,2,1,0,
		2,0,6,6,0,4,
		4,5,6,6,5,7,
		5,1,7,7,1,3,
		1,5,0,0,5,4,
		6,7,2,2,7,3,
	};

	CD3D11_BUFFER_DESC descIndex(
		sizeof(float) * indexesCube.size(),
		D3D11_BIND_INDEX_BUFFER
	);

	D3D11_SUBRESOURCE_DATA subResDataIndexes = {};
	subResDataIndexes.pSysMem = indexesCube.data();

	device->CreateBuffer(
		&descIndex, &subResDataIndexes, indexBuffer.ReleaseAndGetAddressOf()
	);
}

void Game::Tick() {
	// DX::StepTimer will compute the elapsed time and call Update() for us
	// We pass Update as a callback to Tick() because StepTimer can be set to a "fixed time" step mode, allowing us to call Update multiple time in a row if the framerate is too low (useful for physics stuffs)
	m_timer.Tick([&]() { Update(m_timer); });

	Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer) {
	auto const kb = m_keyboard->GetState();
	auto const ms = m_mouse->GetState();
	
	// add kb/mouse interact here
	
	if (kb.Escape)
		ExitGame();

	auto const pad = m_gamePad->GetState(0);
}

// Draws the scene.
void Game::Render() {
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
		return;

	auto context = m_deviceResources->GetD3DDeviceContext();
	auto renderTarget = m_deviceResources->GetRenderTargetView();
	auto depthStencil = m_deviceResources->GetDepthStencilView();
	auto const viewport = m_deviceResources->GetScreenViewport();

	context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->RSSetViewports(1, &viewport);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);
	
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(inputLayout.Get());

	basicShader->Apply(m_deviceResources.get());

	ID3D11Buffer* cbs[] = { modelBuffer.Get(), cameraBuffer.Get() };
	context->VSSetConstantBuffers(
		0, 2, cbs
	);

	CameraData cameraData = {};
	ModelData modelData = {};

	modelData.mModel = Matrix::Identity;
	cameraData.mView = Matrix::CreateLookAt(
		Vector3::Backward * 10,
		Vector3::Zero,
		Vector3::Up
	).Transpose();
	cameraData.mProjection = mProjection.Transpose();

	context->UpdateSubresource(
		cameraBuffer.Get(),
		0,
		nullptr,
		&cameraData,
		0,
		0
	);

	for (int i = 0; i < 10; ++i)
	{
		Matrix model = Matrix::Identity;
		model *= Matrix::CreateScale(0.3f);
		model *= Matrix::CreateRotationZ(m_timer.GetTotalSeconds() * XM_PI / 180.0f * 45);
		model *= Matrix::CreateRotationX(m_timer.GetTotalSeconds() * XM_PI / 180.0f * 60);
		model *= Matrix::CreateRotationY(m_timer.GetTotalSeconds() * XM_PI / 180.0f * 90);

		model *= Matrix::CreateTranslation(
			tan(m_timer.GetTotalSeconds() * XM_PI / 180.0f * 45 + i * 10),
			tan(m_timer.GetTotalSeconds() * XM_PI / 180.0f * 45 + i * 10),
			0
		);

		modelData.mModel = model.Transpose();

		;context->UpdateSubresource(
			modelBuffer.Get(),
			0,
			nullptr,
			&modelData,
			0,
			0
		);


		// TP: Tracer votre vertex buffer ici
		ID3D11Buffer* vbs[] = { vertexBuffer.Get() };
		UINT strides[] = { sizeof(float) * 3 };
		UINT offsets[] = { 0 };

		context->IASetVertexBuffers(0, 1, vbs, strides, offsets);
		context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(36, 0, 0);
		//context->Draw(36, 0);
	}

	// envoie nos commandes au GPU pour etre afficher � l'�cran
	m_deviceResources->Present();
}

#pragma region Message Handlers
void Game::OnActivated() {}

void Game::OnDeactivated() {}

void Game::OnSuspending() {}

void Game::OnResuming() {
	m_timer.ResetElapsedTime();
}

void Game::OnWindowMoved() {
	auto const r = m_deviceResources->GetOutputSize();
	m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnDisplayChange() {
	m_deviceResources->UpdateColorSpace();
}

void Game::OnWindowSizeChanged(int width, int height) {

	if (!m_deviceResources->WindowSizeChanged(width, height))
		return;
	mProjection = Matrix::CreatePerspectiveFieldOfView(60 * XM_PI / 180.0f, (float)width / (float)height, 0.01f, 100.0f);

	// The windows size has changed:
	// We can realloc here any resources that depends on the target resolution (post processing etc)
}

void Game::OnDeviceLost() {
	// We have lost the graphics card, we should reset resources [TODO]
}

void Game::OnDeviceRestored() {
	// We have a new graphics card context, we should realloc resources [TODO]
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept {
	width = 800;
	height = 600;
}

#pragma endregion
