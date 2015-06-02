#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <system/win32/platform_win32_null_renderer.h>
#include <system/win32/file_win32.h>
#include <maths/matrix44.h>

namespace abfw
{
	PlatformWin32NullRenderer::PlatformWin32NullRenderer()
	{
	}

	PlatformWin32NullRenderer::~PlatformWin32NullRenderer()
	{
	}

	class SpriteRenderer* PlatformWin32NullRenderer::CreateSpriteRenderer() const
	{
		return NULL;
	}

	class Mesh* PlatformWin32NullRenderer::CreateMesh() const
	{
		return NULL;
	}

	class Texture* PlatformWin32NullRenderer::CreateTexture(const ImageData& image_data) const
	{
		return NULL;
	}


	void PlatformWin32NullRenderer::PreRender()
	{
	}

	void PlatformWin32NullRenderer::PostRender()
	{
	}

	void PlatformWin32NullRenderer::Clear() const
	{
	}

	bool PlatformWin32NullRenderer::Update()
	{
		return true;
	}

	float PlatformWin32NullRenderer::GetFrameTime()
	{
		return 0.0f;
	}

	std::string PlatformWin32NullRenderer::FormatFilename(const std::string& filename) const
	{
		return filename;
	}

	std::string PlatformWin32NullRenderer::FormatFilename(const char* filename) const
	{
		return std::string(filename);
	}

	File* PlatformWin32NullRenderer::CreateFile() const
	{
		return new abfw::FileWin32();
	}

	AudioManager* PlatformWin32NullRenderer::CreateAudioManager() const
	{
		return NULL;
	}

	TouchInputManager* PlatformWin32NullRenderer::CreateTouchInputManager() const
	{
		return NULL;
	}

	SonyControllerInputManager* PlatformWin32NullRenderer::CreateSonyControllerInputManager() const
	{
		return NULL;
	}

	Renderer3D* PlatformWin32NullRenderer::CreateRenderer3D() const
	{
		return NULL;
	}

	RenderTarget* PlatformWin32NullRenderer::CreateRenderTarget(const Int32 width, const Int32 height) const
	{
		return NULL;
	}

	Matrix44 PlatformWin32NullRenderer::PerspectiveProjectionFov(const float fov, const float aspect_ratio, const float near_distance, const float far_distance) const
	{
		Matrix44 projection_matrix;
		projection_matrix.SetIdentity();
		return projection_matrix;
	}

	Matrix44 PlatformWin32NullRenderer::PerspectiveProjectionFrustum(const float left, const float right, const float top, const float bottom, const float near_distance, const float far_distance) const
	{
		Matrix44 projection_matrix;
		projection_matrix.SetIdentity();
		return projection_matrix;
	}


	void PlatformWin32NullRenderer::BeginScene() const
	{
	}

	void PlatformWin32NullRenderer::EndScene() const
	{
	}
}