#ifndef _ABFW_PLATFORM_WIN32_NULL_RENDER_H
#define _ABFW_PLATFORM_WIN32_NULL_RENDER_H

#include <system/platform.h>

namespace abfw
{
	class PlatformWin32NullRenderer : public Platform
	{
	public:
		PlatformWin32NullRenderer();
		~PlatformWin32NullRenderer();

		class Renderer3D* CreateRenderer3D() const;
		class SpriteRenderer* CreateSpriteRenderer() const;
		Mesh* CreateMesh() const;
		class Texture* CreateTexture(const ImageData& image_data) const;
		SonyControllerInputManager* CreateSonyControllerInputManager() const;

		Matrix44 PerspectiveProjectionFov(const float fov, const float aspect_ratio, const float near_distance, const float far_distance) const;
		Matrix44 PerspectiveProjectionFrustum(const float left, const float right, const float top, const float bottom, const float near_distance, const float far_distance) const;

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

	private:
		RenderTarget* CreateRenderTarget(const Int32 width, const Int32 height) const;


	};

}

#endif // _ABFW_PLATFORM_WIN32_NULL_RENDER_H