#ifndef _ABFW_PLATFORM_H
#define _ABFW_PLATFORM_H

#include <abertay_framework.h>
#include <graphics/colour.h>
#include <string>

namespace abfw
{
	// forward declarations
	class File;
	class SpriteRenderer;
	class AudioManager;
	class TouchInputManager;
	class Texture;
	class ImageData;
	class Mesh;
	class SonyControllerInputManager;
	class Renderer3D;
	class RenderTarget;

	class Platform
	{
	public:
		Platform();
		virtual ~Platform();

		virtual bool Update() = 0;
		virtual float GetFrameTime() = 0;
		virtual void PreRender() = 0;
		virtual void PostRender() = 0;
		virtual void Clear() const= 0;

		std::string FormatFilename(const std::string& filename) const;
		virtual std::string FormatFilename(const char* filename) const = 0;
		virtual SpriteRenderer* CreateSpriteRenderer() const = 0;
		virtual File* CreateFile() const = 0;
		virtual AudioManager* CreateAudioManager() const = 0;
		virtual TouchInputManager* CreateTouchInputManager() const = 0;
		virtual Texture* CreateTexture(const ImageData& image_data) const = 0;
		virtual Mesh* CreateMesh() const = 0;
		virtual SonyControllerInputManager* CreateSonyControllerInputManager() const = 0;
		virtual Renderer3D* CreateRenderer3D() const = 0;

		/// Can only create render targets that are the same size as the screen at the moment
		/// so we can use the same depth buffer for all render targets
		RenderTarget* CreateRenderTarget() const;

		virtual Matrix44 PerspectiveProjectionFov(const float fov, const float aspect_ratio, const float near_distance, const float far_distance) const = 0;
		virtual Matrix44 PerspectiveProjectionFrustum(const float left, const float right, const float top, const float bottom, const float near_distance, const float far_distance) const = 0;

		inline Int32 width() const { return width_; }
		inline Int32 height() const { return height_; }
		inline const Colour& render_target_clear_colour() const { return render_target_clear_colour_; }
		inline void set_render_target_clear_colour(const Colour& colour) { render_target_clear_colour_ = colour; }
		inline float depth_clear_value() const { return depth_clear_value_; }
		inline void set_depth_clear_value(const float value) { depth_clear_value_ = value; }
		inline UInt32 stencil_clear_value() const { return stencil_clear_value_; }
		inline void set_stencil_clear_value(const UInt32 value) { stencil_clear_value_ = value; }
		inline void set_render_target(RenderTarget* render_target) { render_target_ = render_target; }
		inline RenderTarget* render_target() const { return render_target_; }

	protected:
		inline void set_width(const Int32 width) { width_ = width; }
		inline void set_height(const Int32 height) { height_ = height; }

		virtual RenderTarget* CreateRenderTarget(const Int32 width, const Int32 height) const = 0;

		Int32 width_;
		Int32 height_;
		Colour render_target_clear_colour_;
		float depth_clear_value_;
		UInt32 stencil_clear_value_;
		RenderTarget* render_target_;
	};
}

#endif // _ABFW_PLATFORM_H