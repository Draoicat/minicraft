#ifndef BUFFER_H
#define BUFFER_H

template<typename TVertex>
class VertexBuffer
{
public:
	uint32_t PushVertex(const TVertex& v)
	{
		data.push_back(v);
		return data.size() - 1;
	}

	void Create(DeviceResources* deviceRes)
	{
		if (data.empty()) return;
		CD3D11_BUFFER_DESC desc(
			sizeof(TVertex) * data.size(),
			D3D11_BIND_VERTEX_BUFFER
		);

		D3D11_SUBRESOURCE_DATA subResData = {};
		subResData.pSysMem = data.data();

		deviceRes->GetD3DDevice()->CreateBuffer(
			&desc, &subResData, vertexBuffer.ReleaseAndGetAddressOf()
		);
	}

	void Apply(DeviceResources* deviceRes, int slot = 0)
	{
		ID3D11Buffer* vbs[] = { vertexBuffer.Get() };
		UINT strides[] = { sizeof(TVertex) };
		UINT offsets[] = { 0 };

		deviceRes->GetD3DDeviceContext()->IASetVertexBuffers(slot, 1, vbs, strides, offsets);
	}

private:
	std::vector<TVertex> data;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
};

class IndexBuffer
{
public:
	void PushTriangle(uint32_t a, uint32_t b, uint32_t c)
	{
		data.push_back(a);
		data.push_back(b);
		data.push_back(c);
	}

	uint32_t Size()
	{
		return data.size();
	}

	void Create(DeviceResources* deviceRes)
	{
		if (data.empty()) return;
		CD3D11_BUFFER_DESC desc(
			sizeof(uint32_t) * data.size(),
			D3D11_BIND_INDEX_BUFFER
		);

		D3D11_SUBRESOURCE_DATA subResData = {};
		subResData.pSysMem = data.data();

		deviceRes->GetD3DDevice()->CreateBuffer(
			&desc, &subResData, indexBuffer.ReleaseAndGetAddressOf()
		);
	}

	void Apply(DeviceResources* deviceRes)
	{
		deviceRes->GetD3DDeviceContext()->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

private:
	std::vector<uint32_t> data;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
};

template<typename TData>
class ConstantBuffer
{
public:
	TData data;

	void Create(DeviceResources* deviceRes)
	{
		CD3D11_BUFFER_DESC desc(
			sizeof(TData),
			D3D11_BIND_CONSTANT_BUFFER
		);

		deviceRes->GetD3DDevice()->CreateBuffer(
			&desc, nullptr, constantbuffer.ReleaseAndGetAddressOf()
		);
	}

	void UpdateBuffer(DeviceResources* deviceRes)
	{
		deviceRes->GetD3DDeviceContext()->UpdateSubresource(
			constantbuffer.Get(),
			0,
			nullptr,
			&data,
			0,
			0
		);
	}

	void ApplyToVS(DeviceResources* deviceRes, int slot = 0)
	{

		ID3D11Buffer* cbs[] = { constantbuffer.Get() };
		deviceRes->GetD3DDeviceContext()->VSSetConstantBuffers(
			slot, 1, cbs
		);
	}

	void ApplyToPS(DeviceResources* deviceRes, int slot = 0)
	{

		ID3D11Buffer* cbs[] = { constantbuffer.Get() };
		deviceRes->GetD3DDeviceContext()->PSSetConstantBuffers(
			slot, 1, cbs
		);
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantbuffer;
};

#endif