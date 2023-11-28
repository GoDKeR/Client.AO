#pragma once

#include <d3d11_4.h>
#include <d3dcompiler.h>

#include <memory>
#include <cassert>

#include <Helpers.h>
#include <Defines.h>

#include <Graphics/Grh.h>
#include <IO/GrhData.h>
#include <IO/BodyData.h>
#include <IO/HeadData.h>

namespace World
{
	class Map;
}

namespace Graphics
{
	class Engine
	{
	private:
		struct VertexPositionColorTexture
		{
			DirectX::XMFLOAT3	Position;
			UInt32				Color;
			DirectX::XMFLOAT2	Texture;
		};

		struct Drawable
		{
			UInt32 FileNum = 0;
			UInt32 Color = 0;

			Float X = 0.0f;
			Float Y = 0.0f;
			Float Depth = 0.0f;
			UInt32 Width = 0;
			UInt32 Height = 0;

			Float Tx1 = 0.0f;
			Float Ty1 = 0.0f;
			Float Tx2 = 0.0f;
			Float Ty2 = 0.0f;
		};
		
		struct ShaderResource
		{
			ComPtr<ID3D11ShaderResourceView> ResourceView;
			Int32 Width		= 0;
			Int32 Height	= 0;
		};

		enum ConstantBuffers
		{
			Application,
			Frame,
			Object,
			NumConstantBuffers
		};

		ComPtr<ID3D11Device5>			mDevice;
		ComPtr<ID3D11DeviceContext>		mContext;

		ComPtr<IDXGISwapChain4>			mSwapChain;
		ComPtr<ID3D11RenderTargetView>	mRenderTargetView;
		ComPtr<ID3D11DepthStencilView>	mDepthStencilView;
		ComPtr<ID3D11DepthStencilState>	mDepthStencilState;
		ComPtr<ID3D11DepthStencilState>	mDepthStencilStateDisabled;
		ComPtr<ID3D11Texture2D>			mDepthStencilBuffer;

		ComPtr<ID3D11RasterizerState>	mRasterizerState;

		ComPtr<ID3D11InputLayout>		mInputLayout;
		ComPtr<ID3D11Buffer>			mVertexBuffer;
		ComPtr<ID3D11Buffer>			mIndexBuffer;
		ComPtr<ID3D11Buffer>			mConstantBuffers[NumConstantBuffers];
		
		ComPtr<ID3D11BlendState>		mBlendState;

		Int32							mVBOffset = 0;

		ComPtr<ID3D11VertexShader>		mVertexShader;
		ComPtr<ID3D11PixelShader>		mPixelShader;

		D3D11_VIEWPORT					mViewport = { 0 };

		Matrix4F						mWorldMatrix;
		Matrix4F						mViewMatrix;
		Matrix4F						mProjectionMatrix;

		Bool	mVsync;
		HWND	mWindowHandle;
		Int32	mWidth;
		Int32	mHeight;


		static constexpr Int32 MAX_SPRITES				= 2000;
		static constexpr Int32 MAX_VERTEXBUFFER_SIZE	= MAX_SPRITES * 4 * sizeof(VertexPositionColorTexture);

		Drawable					mDrawables[MAX_SPRITES];
		VertexPositionColorTexture	mVertices[MAX_SPRITES * 4];

		UInt32	mSpriteCount = 0;

		ComPtr<ID3D11ShaderResourceView> mShaderResourceView;

		UnorderedMap<UInt32, ShaderResource> mShaderResourceList = {};

		ComPtr<ID3D11ShaderResourceView> CreateShaderResource(UInt32 file, Int32& width, Int32& height);
		ComPtr<ID3D11Texture2D> CreateTexture(UInt32 file, Int32& width, Int32& height);
		ComPtr<ID3D11ShaderResourceView> GetShaderResource(UInt32 file, Int32& width, Int32& height);
		ComPtr<ID3D11ShaderResourceView> GetShaderResource(UInt32 file);

		void Draw(Graphics::Grh grh, Int32 x, Int32 y, Float z, Bool center, Bool animate);
		void Draw(UInt32 grhIndex, Int32 x, Int32 y, Float z, Bool center);
		void Draw(UInt32 fileNum, UInt32 color, Float x, Float y, Float z, UInt32 width, UInt32 height, Float tx1, Float ty1, Float tx2, Float ty2);

		Float CalculateDepth(FLOAT layer, FLOAT x, FLOAT y, FLOAT z);

		
		Bool CreatePipeline();
		
		void Write(VertexPositionColorTexture* vertices, Int32& position);

	public:
		Engine(HWND hwnd, Int32 width, Int32 height, Bool vsync);
		~Engine();

		void Render(World::Map* map);
		void Clear(const Float color[4], Float depth, UInt8 stencil);
		void ResetStates();
		void SetBuffers();
		void Flush();
		void Present();

		void DepthStateEnabled();
		void DepthStateDisabled();

		ComPtr<ID3D11Device5> GetDevice();
		ComPtr<ID3D11DeviceContext> GetDeviceContext();

		static Vector<IO::GrhData>	GrhList;
		static Vector<IO::BodyData> BodyList;
		static Vector<IO::HeadData> HeadList;
		static Vector<IO::HeadData> HelmetList;

	};
}