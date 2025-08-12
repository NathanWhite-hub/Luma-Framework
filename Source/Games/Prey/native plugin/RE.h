#pragma once

#include <stdint.h>
#include <d3d11_1.h>
#include <vector>
#include <wrl/client.h>

namespace RE
{
	class CShader;
	class CConstantBuffer;

	enum ETEX_Format : uint8_t
	{
		eTF_Unknown = 0x0,
		eTF_R8G8B8A8S = 0x1,
		eTF_R8G8B8A8 = 0x2,
		eTF_A8 = 0x4,
		eTF_R8 = 0x5,
		eTF_R8S = 0x6,
		eTF_R16 = 0x7,
		eTF_R16F = 0x8,
		eTF_R32F = 0x9,
		eTF_R8G8 = 0xA,
		eTF_R8G8S = 0xB,
		eTF_R16G16 = 0xC,
		eTF_R16G16S = 0xD,
		eTF_R16G16F = 0xE,
		eTF_R11G11B10F = 0xF,
		eTF_R10G10B10A2 = 0x10,
		eTF_R16G16B16A16 = 0x11,
		eTF_R16G16B16A16S = 0x12,
		eTF_R16G16B16A16F = 0x13,
		eTF_R32G32B32A32F = 0x14,
		eTF_CTX1 = 0x15,
		eTF_BC1 = 0x16,
		eTF_BC2 = 0x17,
		eTF_BC3 = 0x18,
		eTF_BC4U = 0x19,
		eTF_BC4S = 0x1A,
		eTF_BC5U = 0x1B,
		eTF_BC5S = 0x1C,
		eTF_BC6UH = 0x1D,
		eTF_BC6SH = 0x1E,
		eTF_BC7 = 0x1F,
		eTF_R9G9B9E5 = 0x20,
		eTF_D16 = 0x21,
		eTF_D24S8 = 0x22,
		eTF_D32F = 0x23,
		eTF_D32FS8 = 0x24,
		eTF_B5G6R5 = 0x25,
		eTF_B5G5R5 = 0x26,
		eTF_B4G4R4A4 = 0x27,
		eTF_EAC_R11 = 0x28,
		eTF_EAC_RG11 = 0x29,
		eTF_ETC2 = 0x2A,
		eTF_ETC2A = 0x2B,
		eTF_A8L8 = 0x2C,
		eTF_L8 = 0x2D,
		eTF_L8V8U8 = 0x2E,
		eTF_B8G8R8 = 0x2F,
		eTF_L8V8U8X8 = 0x30,
		eTF_B8G8R8X8 = 0x31,
		eTF_B8G8R8A8 = 0x32,
		eTF_PVRTC2 = 0x33,
		eTF_PVRTC4 = 0x34,
		eTF_ASTC_4x4 = 0x35,
		eTF_ASTC_5x4 = 0x36,
		eTF_ASTC_5x5 = 0x37,
		eTF_ASTC_6x5 = 0x38,
		eTF_ASTC_6x6 = 0x39,
		eTF_ASTC_8x5 = 0x3A,
		eTF_ASTC_8x6 = 0x3B,
		eTF_ASTC_8x8 = 0x3C,
		eTF_ASTC_10x5 = 0x3D,
		eTF_ASTC_10x6 = 0x3E,
		eTF_ASTC_10x8 = 0x3F,
		eTF_ASTC_10x10 = 0x40,
		eTF_ASTC_12x10 = 0x41,
		eTF_ASTC_12x12 = 0x42,
		eTF_R16U = 0x43,
		eTF_R16G16U = 0x44,
		eTF_R10G10B10A2UI = 0x45,
		eTF_MaxFormat = 0x46,
	};

	enum ETEX_Type : uint8_t
	{
		eTT_1D = 0,
		eTT_2D,
		eTT_3D,
		eTT_Cube,
		eTT_AutoCube,
		eTT_Auto2D,
		eTT_User,
		eTT_NearestCube,
		eTT_MaxTexType,
	};

	enum ETextureFlags : uint32_t
	{
		FT_NOMIPS = 0x1,
		FT_TEX_NORMAL_MAP = 0x2,
		FT_TEX_WAS_NOT_PRE_TILED = 0x4,
		FT_USAGE_DEPTHSTENCIL = 0x8,
		FT_USAGE_ALLOWREADSRGB = 0x10,
		FT_FILESINGLE = 0x20,
		FT_TEX_FONT = 0x40,
		FT_HAS_ATTACHED_ALPHA = 0x80,
		FT_USAGE_UNORDERED_ACCESS = 0x100,
		FT______________________ = 0x200,
		FT_USAGE_MSAA = 0x400,
		FT_FORCE_MIPS = 0x800,
		FT_USAGE_RENDERTARGET = 0x1000,
		FT_USAGE_DYNAMIC = 0x2000,
		FT_STAGE_READBACK = 0x4000,
		FT_STAGE_UPLOAD = 0x8000,
		FT_DONT_RELEASE = 0x10000,
		FT_ASYNC_PREPARE = 0x20000,
		FT_DONT_STREAM = 0x40000,
		FT_______________________ = 0x80000,
		FT_FAILED = 0x100000,
		FT_FROMIMAGE = 0x200000,
		FT_STATE_CLAMP = 0x400000,
		FT_USAGE_ATLAS = 0x800000,
		FT_ALPHA = 0x1000000,
		FT_REPLICATE_TO_ALL_SIDES = 0x2000000,
		FT____________________ = 0x4000000,
		FT_SPLITTED = 0x8000000,
		FT_STREAMED_PREPARE = 0x10000000,
		FT_NO_DEPTH_COMPRESSION = 0x20000000,
		FT_NO_COLOR_COMPRESSION = 0x40000000,
		FT_USAGE_UAV_RWTEXTURE = 0x80000000,
	};
	inline ETextureFlags operator|(ETextureFlags a, ETextureFlags b) { return static_cast<ETextureFlags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b)); }
	inline ETextureFlags operator&(ETextureFlags a, ETextureFlags b) { return static_cast<ETextureFlags>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b)); }
	inline ETextureFlags operator^(ETextureFlags a, ETextureFlags b) { return static_cast<ETextureFlags>(static_cast<uint32_t>(a) ^ static_cast<uint32_t>(b)); }
	inline ETextureFlags operator~(ETextureFlags a) { return static_cast<ETextureFlags>(~static_cast<uint32_t>(a)); }
	inline ETextureFlags& operator|=(ETextureFlags& a, ETextureFlags b) { return a = a | b; }
	inline ETextureFlags& operator&=(ETextureFlags& a, ETextureFlags b) { return a = a & b; }
	inline ETextureFlags& operator^=(ETextureFlags& a, ETextureFlags b) { return a = a ^ b; }

	class CTexture
	{
	public:
		uint8_t unk00[0x54];
		ETextureFlags m_nFlags;
	};
	static_assert(offsetof(CTexture, m_nFlags) == 0x54);

	struct Vec2
	{
		float x;
		float y;
	};

	struct Vec4
	{
		float x;
		float y;
		float z;
		float w;
	};

	struct Mat4x4
	{
		float m00;
		float m01;
		float m02;
		float m03;
		float m10;
		float m11;
		float m12;
		float m13;
		float m20;
		float m21;
		float m22;
		float m23;
		float m30;
		float m31;
		float m32;
		float m33;
	};

	struct DeviceInfo
	{
		IDXGIFactory1* m_pFactory;
		IDXGIAdapter* m_pAdapter;
		IDXGIOutput* m_pOutput;
		ID3D11Device* m_pDevice;
		ID3D11DeviceContext1* m_pContext;
		IDXGISwapChain* m_pSwapChain;
		std::vector<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>> m_pBackbufferRTVs; // Note: this was a "_smart_ptr", some CryEngine internal smart pointer, but it seems to be the same size as com ptr
		void*                 m_pBackbufferRTV; // D3DSurface* (ID3D11RenderTargetView likely)
		unsigned int          m_pCurrentBackBufferRTVIndex;
		DXGI_ADAPTER_DESC1    m_adapterDesc;
		DXGI_SWAP_CHAIN_DESC  m_swapChainDesc;
		DXGI_RATIONAL         m_refreshRate;
		DXGI_RATIONAL         m_desktopRefreshRate;
		D3D_DRIVER_TYPE       m_driverType;
		unsigned int          m_creationFlags;
		D3D_FEATURE_LEVEL     m_featureLevel;
		DXGI_FORMAT           m_autoDepthStencilFmt;
		unsigned int          m_outputIndex;
		unsigned int          m_syncInterval;
		unsigned int          m_presentFlags;
		bool                  m_activated;
		bool                  m_activatedMT;
	};

	class CD3D9Renderer
	{
	public:
		virtual void Unk00();
		virtual void Unk01();
		virtual void Unk02();
		virtual void Unk03();
		virtual void Unk04();
		virtual void Unk05();
		virtual void Unk06();
		virtual void Unk07();
		virtual void Unk08();
		virtual void Unk09();
		virtual void Unk0A();
		virtual void Unk0B();
		virtual void Unk0C();
		virtual void Unk0D();
		virtual void Unk0E();
		virtual void Unk0F();
		virtual void Unk10();
		virtual void Unk11();
		virtual void Unk12();
		virtual void Unk13();
		virtual void Unk14();
		virtual void Unk15();
		virtual void Unk16();
		virtual void Unk17();
		virtual void Unk18();
		virtual void Unk19();
		virtual void Unk1A();
		virtual void Unk1B();
		virtual void Unk1C();
		virtual void Unk1D();
		virtual void Unk1E();
		virtual void Unk1F();
		virtual void Unk20();
		virtual void Unk21();
		virtual void Unk22();
		virtual void Unk23();
		virtual void Unk24();
		virtual void Unk25();
		virtual void Unk26();
		virtual void Unk27();
		virtual void Unk28();
		virtual void Unk29();
		virtual void Unk2A();
		virtual void Unk2B();
		virtual void Unk2C();
		virtual void Unk2D();
		virtual void Unk2E();
		virtual void Unk2F();
		virtual void Unk30();
		virtual void Unk31();
		virtual void Unk32();
		virtual void Unk33();
		virtual void Unk34();
		virtual void Unk35();
		virtual void Unk36();
		virtual void Unk37();
		virtual void Unk38();
		virtual void Unk39();
		virtual void Unk3A();
		virtual void Unk3B();
		virtual void Unk3C();
		virtual void Unk3D();
		virtual void Unk3E();
		virtual void Unk3F();
		virtual void Unk40();
		virtual void Unk41();
		virtual void Unk42();
		virtual void Unk43();
		virtual void Unk44();
		virtual uint32_t GetHeight();
		virtual uint32_t GetWidth();
		virtual void Unk47();
		virtual void Unk48();
		virtual void Unk49();
		virtual void Unk4A();
		virtual void Unk4B();
		virtual void Unk4C();
		virtual void Unk4D();
		virtual void Unk4E();
		virtual void Unk4F();
		//...

		// members
		uint8_t unk0000[0xD68];
		Vec2 m_vProjMatrixSubPixoffset; // 0xD70
		uint8_t unkD78[0x8B20];
		volatile int32_t        m_nAsyncDeviceState;  //0x9898
		uint8_t unk98A0[0xD0];
		ID3D11DepthStencilView* m_pNativeZSurface;  // 0x9970
		uint8_t unk9978[0x1488];
		ID3D11RenderTargetView* m_pBackBuffer; // 0xAE00
		uint8_t unkAE08[0x130];
		ID3D11DeviceContext1* m_pDeviceContext; //0xAF38
		uint8_t unkAF40[0x40];
		DeviceInfo m_devInfo; // 0xAF80
	};
	static_assert(offsetof(CD3D9Renderer, m_vProjMatrixSubPixoffset) == 0xD70);
	static_assert(offsetof(CD3D9Renderer, m_nAsyncDeviceState) == 0x9898);
	static_assert(offsetof(CD3D9Renderer, m_pNativeZSurface) == 0x9970);
	static_assert(offsetof(CD3D9Renderer, m_pBackBuffer) == 0xAE00);
	static_assert(offsetof(CD3D9Renderer, m_pDeviceContext) == 0xAF38);
	static_assert(offsetof(CD3D9Renderer, m_devInfo) == 0xAF80);

	constexpr size_t offset = offsetof(CD3D9Renderer, m_devInfo);

	struct SD3DSurface;

	class CCryNameR
	{
	public:
		CCryNameR()
		{
			m_str = nullptr;
		}

		CCryNameR(const char* s);

	private:
		const char* m_str;
	};

	enum EHWShaderClass
	{
		eHWSC_Vertex = 0,
		eHWSC_Pixel = 1,
		eHWSC_Geometry = 2,
		eHWSC_Compute = 3,
		eHWSC_Domain = 4,
		eHWSC_Hull = 5,
		eHWSC_Num = 6
	};

	enum ECGParam
	{
		ECGP_Unknown = 0,

		// ...

		ECGP_Max = 0xFF
	};

	struct SParamDB
	{
		const char* szName = nullptr;
		const char* szAliasName = nullptr;
		ECGParam    eParamType = ECGParam::ECGP_Unknown;
		uint32_t    nFlags = 0;
		void*       ParserFunc = nullptr;
	};

	struct SCGBind
	{
		CCryNameR m_Name;
		uint32_t  m_Flags;
		int16_t   m_dwBind;
		int16_t   m_dwCBufSlot;
		int32_t   m_nParameters;
	};

	struct SCGParam : SCGBind
	{
		ECGParam  m_eCGParamType;
		void*     m_pData;
		uintptr_t m_nID;
	};

	struct HDRSetupParams
	{
		Vec4 HDRFilmCurve;
		Vec4 HDRBloomColor;
		Vec4 HDRColorBalance;  // Vec4(fHDRBloomAmount * 0.3f, fHDRBloomAmount * 0.3f, fHDRBloomAmount * 0.3f, fGrainAmount); before being sent to the shader, x, y, z are multiplied by 0.125f
		Vec4 HDREyeAdaptation;
		Vec4 HDREyeAdaptationLegacy;
	};
	
	class C3DEngine
	{
	public:
		virtual void Unk00();
		virtual void Unk01();
		virtual void Unk02();
		virtual void Unk03();
		virtual void Unk04();
		virtual void Unk05();
		virtual void Unk06();
		virtual void Unk07();
		virtual void Unk08();
		virtual void Unk09();
		virtual void Unk0A();
		virtual void Unk0B();
		virtual void Unk0C();
		virtual void Unk0D();
		virtual void Unk0E();
		virtual void Unk0F();
		virtual void Unk10();
		virtual void Unk11();
		virtual void Unk12();
		virtual void Unk13();
		virtual void Unk14();
		virtual void Unk15();
		virtual void Unk16();
		virtual void Unk17();
		virtual void Unk18();
		virtual void Unk19();
		virtual void Unk1A();
		virtual void Unk1B();
		virtual void Unk1C();
		virtual void Unk1D();
		virtual void Unk1E();
		virtual void Unk1F();
		virtual void Unk20();
		virtual void Unk21();
		virtual void Unk22();
		virtual void Unk23();
		virtual void Unk24();
		virtual void Unk25();
		virtual void Unk26();
		virtual void Unk27();
		virtual void Unk28();
		virtual void Unk29();
		virtual void Unk2A();
		virtual void Unk2B();
		virtual void GetHDRSetupParams(RE::HDRSetupParams& a_pParams);
		//...
	};

	struct PostAAConstants
	{
		Mat4x4 matReprojection;
		Vec4 params;
		Vec4 screenSize;
		Vec4 worldViewPos;
		Vec4 fxaaParams;
	};
	static_assert(offsetof(PostAAConstants, fxaaParams) == 0x70);
}
