#ifndef _ABFW_PLATFORM_D3D11_H
#define _ABFW_PLATFORM_D3D11_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <d3d11.h>

#include <system/platform.h>
//#include "colour.h"

namespace abfw
{
	class WindowWin32;
	class Mesh;

	class PlatformD3D11 : public Platform
	{
	public:
		PlatformD3D11(HINSTANCE hinstance, UInt32 width, UInt32 height, bool fullscreen, bool vsync_enabled);
		~PlatformD3D11();
//		void Resize(UInt32 width, UInt32 height);
		class Renderer3D* CreateRenderer3D() const;
		class SpriteRenderer* CreateSpriteRenderer() const;
		Mesh* CreateMesh() const;
		class Texture* CreateTexture(const ImageData& image_data) const;
		SonyControllerInputManager* CreateSonyControllerInputManager() const;

//		class Texture* CreateCheckerTexture(UInt32 size, UInt32 num_checkers) ;
		Matrix44 PerspectiveProjectionFov(const float fov, const float aspect_ratio, const float near_distance, const float far_distance) const;
		Matrix44 PerspectiveProjectionFrustum(const float left, const float right, const float top, const float bottom, const float near_distance, const float far_distance) const;
//		void OrthographicFrustumLH(Matrix44& projection_matrix, const float left, const float right, const float top, const float bottom, const float near, const float far);
		void Clear(const bool clear_render_target, const bool clear_depth_buffer, const bool clear_stencil_buffer) const;


		void BeginScene() const;
		void EndScene() const;


		void PreRender();
		void PostRender();
		void Clear() const;

		bool Update();
		float GetFrameTime();
		std::string FormatFilename(const std::string& filename) const;
		std::string FormatFilename(const char* filename) const;
		File* CreateFile() const;
		AudioManager* CreateAudioManager() const;
		TouchInputManager* CreateTouchInputManager() const;

		inline ID3D11Device* device() const { return device_; } 
		inline ID3D11DeviceContext* device_context() const { return device_context_; }
		inline WindowWin32* window() const { return window_; }
	private:
		RenderTarget* CreateRenderTarget(const Int32 width, const Int32 height) const;
		ID3D11RenderTargetView* GetRenderTargetView() const;

		void Release();
		bool InitDeviceAndSwapChain(bool fullscreen);
		void ReleaseDeviceAndSwapChain();

		WindowWin32* window_;
		UInt64		clock_last_frame_;
		D3D_DRIVER_TYPE driver_type_;
		ID3D11Device* device_;
		ID3D11DeviceContext* device_context_;
		IDXGISwapChain* swap_chain_;
		ID3D11Texture2D* back_buffer_;
		ID3D11RenderTargetView* render_target_view_;
		ID3D11Texture2D* depth_stencil_;
		ID3D11DepthStencilView* depth_stencil_view_;
		bool vsync_enabled_;
	};
}
#endif // _ABFW_PLATFORM_D3D11_H
