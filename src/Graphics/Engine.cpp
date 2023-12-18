#include "Engine.h"
#include <World/Map.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Vector<IO::GrhData> Graphics::Engine::GrhList;
Vector<IO::BodyData> Graphics::Engine::BodyList;
Vector<IO::HeadData> Graphics::Engine::HeadList;
Vector<IO::HeadData> Graphics::Engine::HelmetList;

namespace Graphics
{
	Engine::Engine(HWND hwnd, Int32 width, Int32 height, Bool vsync)
	{
		assert(hwnd != 0);
		assert(width > 0);
		assert(height > 0);

		mWindowHandle = hwnd;
		mWidth = width;
		mHeight = height;
		mVsync = vsync;

		RECT rect;
		GetClientRect(mWindowHandle, &rect);

		UInt32 clientWidth = rect.right - rect.left;
		UInt32 clientHeight = rect.bottom - rect.top;

		UInt32 createDeviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;

#if defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> context;

		D3D_FEATURE_LEVEL fl;

		HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			createDeviceFlags, featureLevels, _countof(featureLevels),
			D3D11_SDK_VERSION, device.GetAddressOf(), &fl, context.GetAddressOf());

		if (hr == E_INVALIDARG)
		{
			hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
				createDeviceFlags, &featureLevels[1], _countof(featureLevels) - 1,
				D3D11_SDK_VERSION, device.GetAddressOf(), &fl, context.GetAddressOf());
		}

		if (FAILED(hr))
		{
			//do smth?
			return;
		}

		ComPtr<ID3D11Device5> device5;
		ComPtr<ID3D11DeviceContext4> context4;

		ThrowIfFailed(device.As(&device5));
		ThrowIfFailed(context.As(&context4));

		ThrowIfFailed(device.As(&mDevice));
		ThrowIfFailed(context.As(&mContext));

		ComPtr<IDXGIFactory4> factory;
		ComPtr<IDXGISwapChain1> swapChain;

		UInt32 createFactoryFlags = 0;

#if defined(_DEBUG)
		createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

		ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&factory)));

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };

		swapChainDesc.BufferCount = 2;
		swapChainDesc.Width = clientWidth;
		swapChainDesc.Height = clientHeight;
		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainDesc.Stereo = false;
		swapChainDesc.SampleDesc = { 1, 0 };
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.Flags = 0; //todo: check for tearing support


		ThrowIfFailed(factory->CreateSwapChainForHwnd(
			mDevice.Get(),
			mWindowHandle,
			&swapChainDesc,
			nullptr,
			nullptr,
			swapChain.GetAddressOf()
		));

		ThrowIfFailed(factory->MakeWindowAssociation(mWindowHandle, DXGI_MWA_NO_ALT_ENTER));

		ThrowIfFailed(swapChain.As(&mSwapChain));

		ComPtr<ID3D11Texture2D> backBuffer;
		ThrowIfFailed(mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));

		ThrowIfFailed(mDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, mRenderTargetView.GetAddressOf()));

		D3D11_TEXTURE2D_DESC depthStencilBufferDesc = { 0 };

		depthStencilBufferDesc.ArraySize = 1;
		depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilBufferDesc.CPUAccessFlags = 0; // No CPU access required.
		depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilBufferDesc.Width = clientWidth;
		depthStencilBufferDesc.Height = clientHeight;
		depthStencilBufferDesc.MipLevels = 1;
		depthStencilBufferDesc.SampleDesc.Count = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
		depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		ThrowIfFailed(mDevice->CreateTexture2D(&depthStencilBufferDesc, nullptr, mDepthStencilBuffer.GetAddressOf()));

		ThrowIfFailed(mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, mDepthStencilView.GetAddressOf()));

		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());;

		depthStencilStateDesc.DepthEnable = true;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilStateDesc.StencilEnable = false;
		depthStencilStateDesc.StencilReadMask = 255;
		depthStencilStateDesc.StencilWriteMask = 255;
		depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

		ThrowIfFailed(mDevice->CreateDepthStencilState(&depthStencilStateDesc, mDepthStencilState.GetAddressOf()));
		depthStencilStateDesc.DepthEnable = false;
		ThrowIfFailed(mDevice->CreateDepthStencilState(&depthStencilStateDesc, mDepthStencilStateDisabled.GetAddressOf()));

		D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());
		//rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
		ThrowIfFailed(mDevice->CreateRasterizerState(&rasterDesc, mRasterizerState.GetAddressOf()));

		mViewport.Width = static_cast<float>(clientWidth);
		mViewport.Height = static_cast<float>(clientHeight);
		mViewport.TopLeftX = 0.0f;
		mViewport.TopLeftY = 0.0f;
		mViewport.MinDepth = 0.0f;
		mViewport.MaxDepth = 1.0f;

		{
			D3D11_BUFFER_DESC constantBufferDesc = { 0 };

			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDesc.CPUAccessFlags = 0;
			constantBufferDesc.MiscFlags = 0;
			constantBufferDesc.ByteWidth = sizeof(Matrix4F);
			constantBufferDesc.StructureByteStride = 0;
			constantBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;

			Matrix4F projection = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, width, height, 0.0f, 1.0f, -1.0f);

			D3D11_SUBRESOURCE_DATA constantBufferData = { 0 };

			constantBufferData.pSysMem = &projection;
			constantBufferData.SysMemPitch = 0;
			constantBufferData.SysMemSlicePitch = 0;

			device->CreateBuffer(&constantBufferDesc, &constantBufferData, mConstantBuffers[ConstantBuffers::Application].GetAddressOf());

			D3D11_BUFFER_DESC vertexBufferDesc = { 0 };

			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.ByteWidth = MAX_VERTEXBUFFER_SIZE;
			vertexBufferDesc.StructureByteStride = 0;
			vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;

			mDevice->CreateBuffer(&vertexBufferDesc, nullptr, mVertexBuffer.GetAddressOf());

			UInt16* indices = new UInt16[MAX_SPRITES * 6];

			for (Int32 i = 0, j = 0; i < MAX_SPRITES * 6; j += 4)
			{
				indices[i++] = j;
				indices[i++] = j + 1;
				indices[i++] = j + 3;
				indices[i++] = j + 1;
				indices[i++] = j + 2;
				indices[i++] = j + 3;
			}

			D3D11_BUFFER_DESC indexBufferDesc = { 0 };

			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			indexBufferDesc.ByteWidth = MAX_SPRITES * 6 * sizeof(UInt16);
			indexBufferDesc.StructureByteStride = 0;
			indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;

			D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
			indexBufferData.pSysMem = indices;
			indexBufferData.SysMemPitch = 0;
			indexBufferData.SysMemSlicePitch = 0;


			mDevice->CreateBuffer(&indexBufferDesc, &indexBufferData, mIndexBuffer.GetAddressOf());

			delete[] indices;
		}

		{
			D3D11_BLEND_DESC blendDesc = { 0 };

			/*        desc.AlphaToCoverageEnable = FALSE;
        desc.RenderTarget[0].BlendEnable = TRUE;
        desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;****
        desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;*/

			blendDesc.AlphaToCoverageEnable = true;
			//blendDesc.IndependentBlendEnable = false;
			blendDesc.RenderTarget[0].BlendEnable = true;
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			mDevice->CreateBlendState(&blendDesc, mBlendState.GetAddressOf());
		}

		CreatePipeline();

		mContext->VSSetConstantBuffers(0, 1, mConstantBuffers[ConstantBuffers::Application].GetAddressOf());
		mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
		mContext->OMSetBlendState(mBlendState.Get(), NULL, D3D11_DEFAULT_SAMPLE_MASK);

		GrhList = IO::GrhData::LoadGrhData();
		BodyList = IO::BodyData::LoadBodyData(GrhList);
		HeadList = IO::HeadData::LoadHeadData(GrhList);
		HelmetList = IO::HeadData::LoadHeadData(GrhList, true);
	}

	Bool Engine::CreatePipeline()
	{
		ID3DBlob* pVS = nullptr;
		ID3DBlob* pPS = nullptr;
		ID3DBlob* pErr = nullptr;

		assert(mDevice);
		assert(mContext);

		HRESULT hr;

		hr = D3DCompileFromFile(L"Resources/Shaders/shader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VShader", "vs_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL1, 0, &pVS, &pErr);

		if (FAILED(hr))
		{
			//TODO: log
			return false;
		}

		hr = D3DCompileFromFile(L"Resources/Shaders/shader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PShader", "ps_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL1, 0, &pPS, &pErr);

		if (FAILED(hr))
		{
			//TODO: log
			return false;
		}

		mDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &mVertexShader);
		mDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &mPixelShader);

		D3D11_INPUT_ELEMENT_DESC inputElements[] = {
			{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR",		0, DXGI_FORMAT_R8G8B8A8_UNORM,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		hr = mDevice->CreateInputLayout(
			inputElements, 3,
			pVS->GetBufferPointer(), pVS->GetBufferSize(), mInputLayout.GetAddressOf());

		if (FAILED(hr))
		{
			pVS->Release();
			pPS->Release();
			pErr->Release();

			//TODO: log
			return false;
		}

		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mContext->IASetInputLayout(mInputLayout.Get());

		pVS->Release();
		pPS->Release();
		//pErr->Release();

		return true;
	}

	Engine::~Engine()
	{
		for (auto& item : mShaderResourceList)
		{
			item.second.ResourceView.Reset();
		}

		mShaderResourceList.clear();
	}

	void Engine::Clear(const Float color[4], Float depth, UInt8 stencil)
	{
		mContext->ClearRenderTargetView(mRenderTargetView.Get(), color);
		mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
	}

	void Engine::ResetStates()
	{
		mContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
		mContext->VSSetConstantBuffers(0, 1, mConstantBuffers[ConstantBuffers::Application].GetAddressOf());

		mContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
		mContext->IASetInputLayout(mInputLayout.Get());
		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mContext->RSSetViewports(1, &mViewport);
		mContext->RSSetState(mRasterizerState.Get());

		mContext->OMSetDepthStencilState(mDepthStencilState.Get(), 0);

		mContext->OMSetBlendState(mBlendState.Get(), nullptr, D3D11_DEFAULT_SAMPLE_MASK);

		mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
	}

	void Engine::SetBuffers()
	{
		const UInt32 offset = 0;
		const UInt32 stride = sizeof(VertexPositionColorTexture);

		mContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
		mContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	}
	void Engine::Present()
	{
		mSwapChain->Present(mVsync ? 1 : 0, 0);
	}
	void Engine::Draw(Grh grh, Int32 x, Int32 y, Float z, Bool center = false, Bool animate = false)
	{
		//Todo: grh se está copiando?

		Int32 textureWidth = 0;
		Int32 textureHeight = 0;

		if (animate)
		{
			//xd
		}

		auto pGrh = &GrhList[grh.GrhIndex];
		auto pTex = GetShaderResource(pGrh->FileNum, textureWidth, textureHeight);

		UInt32 currentGrhIndex = pGrh->Frames[grh.FrameCounter - 1];

		if (center)
		{
			if (pGrh->TileWidth != 1)
				x -= (pGrh->TileWidth * 32 / 2) - 16;

			if (pGrh->TileHeight != 1)
			y -= (pGrh->TileHeight * 32) - 32;
		}

		Draw(pGrh->FileNum, -1, x, y, z, pGrh->PixelWidth, pGrh->PixelHeight, pGrh->SX / (float)textureWidth, pGrh->SY / (float)textureHeight, (pGrh->SX + pGrh->PixelWidth) / (float)textureWidth, (pGrh->SY + pGrh->PixelHeight) / (float)textureHeight);
	}
	void Engine::Draw(UInt32 grhIndex, Int32 x, Int32 y, Float z, Bool center)
	{
		Int32 textureWidth = 0;
		Int32 textureHeight = 0;

		auto pGrh = &GrhList[grhIndex];
		auto pTex = GetShaderResource(pGrh->FileNum, textureWidth, textureHeight);//&mShaderResourceList[pGrh->FileNum];

		if (center)
		{
			//xd
		}

		Draw(pGrh->FileNum, -1, x, y, z, pGrh->PixelWidth, pGrh->PixelHeight, pGrh->SX / (float)textureWidth, pGrh->SY / (float)textureHeight, (pGrh->SX + pGrh->PixelWidth) / (float)textureWidth, (pGrh->SY + pGrh->PixelHeight) / (float)textureHeight);
	}
	void Engine::Draw(UInt32 fileNum, UInt32 color, Float x, Float y, Float z, UInt32 width, UInt32 height, Float tx1, Float ty1, Float tx2, Float ty2)
	{
		if (mSpriteCount >= MAX_SPRITES)
			Flush();

		auto item = &mDrawables[mSpriteCount];

		item->FileNum = fileNum;
		item->Color = color;
		item->X = x;
		item->Y = y;
		item->Depth = z;
		item->Width = width;
		item->Height = height;
		item->Tx1 = tx1;
		item->Ty1 = ty1;
		item->Tx2 = tx2;
		item->Ty2 = ty2;

		++mSpriteCount;
	}

	void Engine::Render(World::Map* map)
	{
		
		
		Int32 spX = 0;
		Int32 spY = 0;

	
		for (auto y = 19; y < 44; ++y)
		{
			spX = 0;
			for (auto x = 20; x < 64; ++x)
			{
				World::Map::Tile tile = map->GetTile(x, y);
				//x 45
				//y 19
				Draw(tile.Graphic[0], (spX - 1) * 32, (spY - 1)* 32, CalculateDepth(1, spX, spY, 1.0f));

				if (tile.Graphic[1].GrhIndex > 0)
					Draw(tile.Graphic[1], (spX - 1) * 32, (spY - 1) * 32, CalculateDepth(2, spX, spY, 1.0f), true);

				if (tile.Graphic[2].GrhIndex > 0)
					Draw(tile.Graphic[2], (spX - 1) * 32, (spY - 1) * 32, CalculateDepth(3, spX, spY, 1.0f), true); //CENTRALO FACU CENTRALO

				if (tile.Graphic[3].GrhIndex > 0)
					Draw(tile.Graphic[3], (spX - 1) * 32, (spY - 1) * 32, CalculateDepth(4, spX, spY, 1.0f), true);

				++spX;
			}
			++spY;
		}

	}

	void Engine::Flush()
	{
		if (mSpriteCount == 0) return;

		
		std::sort(mDrawables, mDrawables + mSpriteCount, [](const Drawable& lhs, const Drawable& rhs)
		{
			return lhs.FileNum < rhs.FileNum;
		});

		UInt32 lastTexture = 0;
		auto tempDraw = &mDrawables[0];
		auto tempVert = &mVertices[0];

		Int32 position = 0;

		for (auto i = 0; i < mSpriteCount; ++i)
		{

			if (lastTexture == 0 && tempDraw[i].FileNum != 0)
			{
				lastTexture = tempDraw[i].FileNum;
				mShaderResourceView = GetShaderResource(lastTexture);
			}
			else if (lastTexture != tempDraw[i].FileNum && tempDraw[i].FileNum != 0)
			{
				Write(tempVert, position);
				lastTexture = tempDraw[i].FileNum;
				mShaderResourceView = GetShaderResource(lastTexture);
			}

			//normalizar el tema de los colores
			tempVert[position].Position = DirectX::XMFLOAT3{tempDraw[i].X, tempDraw[i].Y, tempDraw[i].Depth};
			tempVert[position].Color = tempDraw[i].Color;
			tempVert[position].Texture = DirectX::XMFLOAT2{ tempDraw[i].Tx1, tempDraw[i].Ty1 };

			++position;
			//++tempVert;

			tempVert[position].Position = DirectX::XMFLOAT3{ tempDraw[i].X + tempDraw[i].Width, tempDraw[i].Y, tempDraw[i].Depth };
			tempVert[position].Color = tempDraw[i].Color;
			tempVert[position].Texture = DirectX::XMFLOAT2{ tempDraw[i].Tx2, tempDraw[i].Ty1 };

			++position;
			//++tempVert;

			tempVert[position].Position = DirectX::XMFLOAT3{ tempDraw[i].X + tempDraw[i].Width, tempDraw[i].Y + tempDraw[i].Height, tempDraw[i].Depth };
			tempVert[position].Color = tempDraw[i].Color;
			tempVert[position].Texture = DirectX::XMFLOAT2{ tempDraw[i].Tx2, tempDraw[i].Ty2 };

			++position;
			//++tempVert;

			tempVert[position].Position = DirectX::XMFLOAT3{ tempDraw[i].X, tempDraw[i].Y + tempDraw[i].Height, tempDraw[i].Depth };
			tempVert[position].Color = tempDraw[i].Color;
			tempVert[position].Texture = DirectX::XMFLOAT2{ tempDraw[i].Tx1, tempDraw[i].Ty2 };
			
			++position;
			//++tempVert;
		}
		
		Write(tempVert, position);
		mSpriteCount = 0;
	}

	void Engine::Write(VertexPositionColorTexture* vertices, Int32& position)
	{
		const UInt32 offset = mVBOffset + (position / 4) > MAX_SPRITES ? 0 : mVBOffset;
		
		D3D11_MAPPED_SUBRESOURCE subResource;
		mContext->Map(mVertexBuffer.Get(), 0, (offset == 0 ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE), 0, &subResource);

		memcpy((UInt8*)subResource.pData + (offset * 4 * sizeof(VertexPositionColorTexture)), vertices, position * sizeof(VertexPositionColorTexture));

		mContext->Unmap(mVertexBuffer.Get(), 0);

		mContext->PSSetShaderResources(0, 1, mShaderResourceView.GetAddressOf());
		mContext->DrawIndexed((position / 4) * 6, offset * 6, 0);

		mVBOffset = offset + (position / 4);
		position = 0;
	}
	
	Float Engine::CalculateDepth(Float layer, Float x, Float y, Float z)
	{
		//Dr. Wolftein
		return -1.0f + (layer * 0.1f) + ((y - 1) * 0.001f) + ((x - 1) * 0.00001f) + ((z - 1) * 0.000001f);
	}

	ComPtr<ID3D11Texture2D> Engine::CreateTexture(UInt32 file, Int32& width, Int32& height)
	{
		String path = String("D:/Downloads/ArgentumOnline0.13.3-Cliente-Servidor/client/Graficos/") + std::to_string(file) + String(".png");

		//TODO: check for path to exists
		UInt8* imageData = stbi_load(path.data(), &width, &height, nullptr, STBI_rgb_alpha);
		Int32 imagePitch = width * 4;

		if (imageData == NULL)
		{
#if defined(_DEBUG)
			//Load a fallback, just for testing purposes
			imageData = stbi_load("D:/Downloads/ArgentumOnline0.13.3-Cliente-Servidor/client/Graficos/1.png", &width, &height, nullptr, STBI_rgb_alpha);
			imagePitch = width * 4;
#else
			return NULL; //TODO: some error handling
#endif
		}

		D3D11_TEXTURE2D_DESC textureDesc = { 0 };
		
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.ArraySize = 1;
		textureDesc.MipLevels = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;

		D3D11_SUBRESOURCE_DATA textureData = { 0 };
		textureData.pSysMem = imageData;
		textureData.SysMemPitch = imagePitch;

		ComPtr<ID3D11Texture2D> texture;

		HRESULT hr = mDevice->CreateTexture2D(&textureDesc, &textureData, texture.GetAddressOf());

		//todo hr

		free(imageData);

		return texture;
	}

	ComPtr<ID3D11ShaderResourceView> Engine::CreateShaderResource(UInt32 file, Int32& width, Int32& height)
	{
		ComPtr<ID3D11Texture2D> texture = CreateTexture(file, width, height);

		if (!texture)
			return nullptr;

		ShaderResource sr = { 0 };
		sr.Width = width;
		sr.Height = height;

		HRESULT hr = mDevice->CreateShaderResourceView(texture.Get(), nullptr, sr.ResourceView.GetAddressOf());

		mShaderResourceList.insert({ file, sr });

		//todo: check if texture is being released(decremented?)
		return sr.ResourceView;
	}

	ComPtr<ID3D11ShaderResourceView> Engine::GetShaderResource(UInt32 file, Int32& width, Int32& height)
	{
		UnorderedMap<UInt32, ShaderResource>::const_iterator it = mShaderResourceList.find(file);

		if (it == mShaderResourceList.end())
		{
			return CreateShaderResource(file, width, height);
		}
		else
		{
			auto sr = mShaderResourceList[file];
			width = sr.Width;
			height = sr.Height;
			return sr.ResourceView;
		}
	}

	ComPtr<ID3D11ShaderResourceView> Engine::GetShaderResource(UInt32 file)
	{
		Int32 width, height = 0;

		UnorderedMap<UInt32, ShaderResource>::const_iterator it = mShaderResourceList.find(file);

		if (it == mShaderResourceList.end())
		{
			return CreateShaderResource(file, width, height);
		}
		else
		{
			auto sr = mShaderResourceList[file];
			return sr.ResourceView;
		}
	}

	ComPtr<ID3D11Device5> Engine::GetDevice()
	{
		return mDevice;
	}

	ComPtr<ID3D11DeviceContext> Engine::GetDeviceContext()
	{
		return mContext;
	}

	void Engine::DepthStateEnabled()
	{
		mContext->OMSetDepthStencilState(mDepthStencilState.Get(), 1);
	}

	void Engine::DepthStateDisabled()
	{
		mContext->OMSetDepthStencilState(mDepthStencilStateDisabled.Get(), 1);
	}
}