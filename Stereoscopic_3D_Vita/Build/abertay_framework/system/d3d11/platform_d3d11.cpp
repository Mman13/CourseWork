#include <system/d3d11/platform_d3d11.h>
#include <system/win32/window_win32.h>
#include <system/win32/file_win32.h>
#include <graphics/d3d11/texture_d3d11.h>
#include <graphics/d3d11/sprite_renderer_d3d11.h>
#include <input/d3d11/touch_input_manager_d3d11.h>
#include <input/d3d11/sony_controller_input_manager_d3d11.h>
#include <graphics/d3d11/mesh_d3d11.h>
#include <graphics/d3d11/renderer_3d_d3d11.h>
#include <graphics/d3d11/render_target_d3d11.h>

namespace abfw
{
	PlatformD3D11::PlatformD3D11(HINSTANCE hinstance, UInt32 width, UInt32 height, bool fullscreen, bool vsync_enabled) :
		window_(NULL),
		clock_last_frame_(0),
		device_(NULL),
		device_context_(NULL),
		swap_chain_(NULL),
		back_buffer_(NULL),
		render_target_view_(NULL),
		depth_stencil_(NULL),
		depth_stencil_view_(NULL),
		vsync_enabled_(vsync_enabled)
	{
		HRESULT hresult = S_OK;

		set_render_target_clear_colour(Colour(0.4f, 0.525f, 0.7f, 1.0f)); 
		set_depth_clear_value(1.0f);
		set_stencil_clear_value(0.0f);

		// create window
		window_ = new WindowWin32(hinstance, width, height, fullscreen);
		set_width(window_->width());
		set_height(window_->height());



		// Store the vsync setting.
		vsync_enabled_ = vsync_enabled;



		bool success = true;
		success = InitDeviceAndSwapChain(fullscreen);

		if(success)
		{
			// Create a render target view
			hresult = swap_chain_->GetBuffer(0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&back_buffer_);

			if(SUCCEEDED(hresult))
			{
				hresult = device_->CreateRenderTargetView( back_buffer_, NULL, &render_target_view_ );
			}

			// Create DepthStencil Buffer
			if(SUCCEEDED(hresult))
			{
				D3D11_TEXTURE2D_DESC desc_depth;
				desc_depth.Width = window_->width();
				desc_depth.Height = window_->height();
				desc_depth.MipLevels = 1;
				desc_depth.ArraySize = 1;
				desc_depth.Format = DXGI_FORMAT_D32_FLOAT;
				desc_depth.SampleDesc.Count = 1;
				desc_depth.SampleDesc.Quality = 0;
				desc_depth.Usage = D3D11_USAGE_DEFAULT;
				desc_depth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				desc_depth.CPUAccessFlags = 0;
				desc_depth.MiscFlags = 0;
				hresult = device_->CreateTexture2D( &desc_depth, NULL, &depth_stencil_ );

				if(SUCCEEDED(hresult))
				{
					D3D11_DEPTH_STENCIL_VIEW_DESC desc_DSV;
					ZeroMemory(&desc_DSV, sizeof(desc_DSV));
					desc_DSV.Format = desc_depth.Format;
					desc_DSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
					desc_DSV.Texture2D.MipSlice = 0;
					hresult = device_->CreateDepthStencilView(depth_stencil_, &desc_DSV, &this->depth_stencil_view_ );
				}
			}
		}

		if(SUCCEEDED(hresult))
		{
			// bind the render and depth targets to the rendering pipeline 
			device_context_->OMSetRenderTargets( 1, &render_target_view_, depth_stencil_view_ );

			// Setup the viewport
			D3D11_VIEWPORT vp;
			vp.Width = static_cast<FLOAT>(window_->width());
			vp.Height = static_cast<FLOAT>(window_->height());
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			device_context_->RSSetViewports( 1, &vp );
		}
		else
		{
			Release();
		}
	}

	PlatformD3D11::~PlatformD3D11()
	{
		Release();
	}

	void PlatformD3D11::Release()
	{
		ReleaseNull(depth_stencil_view_);
		ReleaseNull(depth_stencil_);
		ReleaseNull(render_target_view_);
		ReleaseNull(back_buffer_);
		ReleaseDeviceAndSwapChain();
		DeleteNull(window_);
	}

	bool PlatformD3D11::InitDeviceAndSwapChain(bool fullscreen)
	{
		IDXGIFactory* factory = NULL;
		IDXGIAdapter* adapter = NULL;
		IDXGIOutput* adapter_output = NULL;
		HRESULT hresult = S_OK;

		// Determine the resolution of the clients desktop screen.
		UInt32 screen_width  = GetSystemMetrics(SM_CXSCREEN);
		UInt32 screen_height = GetSystemMetrics(SM_CYSCREEN);

		UInt32 numerator = 0;
		UInt32 denominator = 1;

		// Create a DirectX graphics interface factory.
		hresult = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if(SUCCEEDED(hresult))
		{
			// Use the factory to create an adapter for the primary graphics interface (video card).
			hresult = factory->EnumAdapters(0, &adapter);
		}


		// Enumerate the primary adapter output (monitor).
		if(SUCCEEDED(hresult))
			hresult = adapter->EnumOutputs(0, &adapter_output);

		// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
		UInt32 num_modes = 0;
		if(SUCCEEDED(hresult))
			hresult = adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, NULL);

		if(SUCCEEDED(hresult))
		{
			// Create a list to hold all the possible display modes for this monitor/video card combination.
			DXGI_MODE_DESC* display_mode_list = new DXGI_MODE_DESC[num_modes];

			// Now fill the display mode list structures.
			hresult = adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, display_mode_list);
			if(SUCCEEDED(hresult))
			{	
				// Now go through all the display modes and find the one that matches the screen width and height.
				// When a match is found store the numerator and denominator of the refresh rate for that monitor.
				for(UInt32 i=0; i<num_modes; i++)
				{
					if(display_mode_list[i].Width == (unsigned int)screen_width)
					{
						if(display_mode_list[i].Height == (unsigned int)screen_height)
						{
							numerator = display_mode_list[i].RefreshRate.Numerator;
							denominator = display_mode_list[i].RefreshRate.Denominator;
						}
					}
				}
			}
			// Release the display mode list.
			delete [] display_mode_list;
			display_mode_list = 0;
		}

		// Release the adapter output.
		ReleaseNull(adapter_output);
		ReleaseNull(adapter);
		ReleaseNull(factory);

		UINT create_device_flags = 0;
	#ifdef _DEBUG
		// causes a crash on lab PCs
		//create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

		D3D_DRIVER_TYPE driver_types[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT num_driver_types = sizeof( driver_types ) / sizeof( driver_types[0] );

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory( &sd, sizeof( sd ) );
		sd.BufferCount = 1;
		sd.BufferDesc.Width = window_->width();
		sd.BufferDesc.Height = window_->height();
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		if(vsync_enabled_)
		{
			sd.BufferDesc.RefreshRate.Numerator = numerator;
			sd.BufferDesc.RefreshRate.Denominator = denominator;
		}
		else
		{
			sd.BufferDesc.RefreshRate.Numerator = 0;
			sd.BufferDesc.RefreshRate.Denominator = 1;
		}
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = window_->hwnd();
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = fullscreen ? FALSE : TRUE;

		// Set the feature level to DirectX 11.
		D3D_FEATURE_LEVEL feature_levels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT num_feature_levels = sizeof( feature_levels ) / sizeof( feature_levels[0] );
	
		for( UINT driver_type_index = 0; driver_type_index < num_driver_types; driver_type_index++ )
		{
			driver_type_ = driver_types[driver_type_index];
			hresult = D3D11CreateDeviceAndSwapChain( NULL, this->driver_type_, NULL, create_device_flags, feature_levels, num_feature_levels,
				D3D11_SDK_VERSION, &sd, &swap_chain_, &device_, NULL, &device_context_);
			if( SUCCEEDED( hresult ) )
				break;
		}
    
		if SUCCEEDED( hresult )
			return true;
		else
		{
			Release();
			return false;
		}
	}

	void PlatformD3D11::ReleaseDeviceAndSwapChain()
	{
		if( device_context_ )
			device_context_->ClearState();

		ReleaseNull(device_context_);

		// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
		if(swap_chain_)
			swap_chain_->SetFullscreenState(false, NULL);

		ReleaseNull(swap_chain_);
/*
		ID3D11Debug *debugDev = NULL;
		device_->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDev));
		debugDev->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
*/

		ReleaseNull(device_);
	}

	/*
	void PlatformD3D11::Run(class Application& application)
	{
		//
		// Windows Message Loop
		//

		// Initialize the message structure.
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		// Loop until there is a quit message from the window or the user.
		bool done = false;
		while(!done)
		{
			// Handle the windows messages.
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// If windows signals to end the application then exit out.
			if(msg.message == WM_QUIT)
				done = true;
			else
			{
				// calculate the time between updates
				UInt64 clock, clock_frequency;
				if(clock_last_frame_ == 0)
					QueryPerformanceCounter((LARGE_INTEGER*)&clock_last_frame_);

				QueryPerformanceCounter((LARGE_INTEGER*)&clock);
				QueryPerformanceFrequency((LARGE_INTEGER*)&clock_frequency);
				UInt32 cycles = (UInt32)(clock - clock_last_frame_);
				float ticks = (float)cycles / (float)clock_frequency;

				clock_last_frame_ = clock;

				// Otherwise do the frame processing.
				bool application_running = application.Update(ticks);
				PreRender();
				application.Render();
				PostRender();
				if(!application_running)
					done = true;
			}
		}
	}
	*/
	/*
	void PlatformD3D11::Resize(UInt32 width, UInt32 height)
	{
		Platform::Resize(width, height);
	}
	*/

	/*
	class Renderer3D* PlatformD3D11::CreateRenderer3D()
	{
		return new Renderer3DD3D(this);
	}
	*/
	class SpriteRenderer* PlatformD3D11::CreateSpriteRenderer() const
	{
		return new SpriteRendererD3D11(*this);
	}

	class Mesh* PlatformD3D11::CreateMesh() const
	{
		return new MeshD3D11();
	}

	class Texture* PlatformD3D11::CreateTexture(const ImageData& image_data) const
	{
		return new TextureD3D11(*this, image_data);
	//	return NULL;
	}

	/*
	class Texture* PlatformD3D11::CreateCheckerTexture(UInt32 size, UInt32 num_checkers)
	{
		return NULL;
	}
	*/
	/*
	void PlatformD3D11::PerspectiveFovRH(Matrix44& projection_matrix, const float fov, const float aspect_ratio, const float near_distance, const float far_distance)
	{
		projection_matrix.PerspectiveFovRHD3D(fov, aspect_ratio, near_distance, far_distance);
	}

	void PlatformD3D11::PerspectiveFrustumRH(Matrix44& projection_matrix, const float left, const float right, const float top, const float bottom, const float near_distance, const float far_distance)
	{
		projection_matrix.PerspectiveFrustumRHD3D(left, right, top, bottom, near_distance, far_distance);
	}

	void PlatformD3D11::OrthographicFrustumLH(Matrix44& projection_matrix, const float left, const float right, const float top, const float bottom, const float near_distance, const float far_distance)
	{
		projection_matrix.OrthographicFrustumLHD3D(left, right, top, bottom, near_distance, far_distance);
	}
	*/

	void PlatformD3D11::PreRender()
	{
	//    float clear_colour[4] = { 0.4f, 0.525f, 0.7f, 1.0f }; //red,green,blue,alpha
	//	device_context_->ClearRenderTargetView( render_target_view_, clear_colour );
	//	device_context_->ClearDepthStencilView( depth_stencil_view_, D3D11_CLEAR_DEPTH, 1.0f, 0 );
		Clear(true, true, true);
	}

	void PlatformD3D11::PostRender()
	{
		if(swap_chain_)
			swap_chain_->Present(vsync_enabled_ ? 1 : 0, 0);
	}

	void PlatformD3D11::Clear() const
	{
		Clear(true, true, true);
	}

	void PlatformD3D11::Clear(const bool clear_render_target_enabled, const bool clear_depth_buffer_enabled, const bool clear_stencil_buffer_enabled ) const
	{
		ID3D11RenderTargetView* render_target_view = GetRenderTargetView();

		if(clear_render_target_enabled)
			device_context_->ClearRenderTargetView( render_target_view, reinterpret_cast<const float*>(&render_target_clear_colour()) );

		UInt32 clear_flags = 0;
		if(clear_depth_buffer_enabled)
			clear_flags |= D3D11_CLEAR_DEPTH;

		if(clear_stencil_buffer_enabled)
			clear_flags |= D3D11_CLEAR_STENCIL;

		// if any of the flags have been set then
		// make the call to ClearRenderTargetView
		if(clear_flags)
			device_context_->ClearDepthStencilView( depth_stencil_view_, clear_flags, depth_clear_value(), stencil_clear_value() );
	}

	bool PlatformD3D11::Update()
	{
		// Initialize the message structure.
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return msg.message != WM_QUIT;
	}

	float PlatformD3D11::GetFrameTime()
	{
		// calculate the time between updates
		UInt64 clock, clock_frequency;
		if(clock_last_frame_ == 0)
			QueryPerformanceCounter((LARGE_INTEGER*)&clock_last_frame_);

		QueryPerformanceCounter((LARGE_INTEGER*)&clock);
		QueryPerformanceFrequency((LARGE_INTEGER*)&clock_frequency);
		UInt32 cycles = (UInt32)(clock - clock_last_frame_);
		clock_last_frame_ = clock;

		return (float)cycles / (float)clock_frequency;
	}

	std::string PlatformD3D11::FormatFilename(const std::string& filename) const
	{
		return filename;
	}

	std::string PlatformD3D11::FormatFilename(const char* filename) const
	{
		return std::string(filename);
	}

	File* PlatformD3D11::CreateFile() const
	{
		return new abfw::FileWin32();
	}

	AudioManager* PlatformD3D11::CreateAudioManager() const
	{
		return NULL;
	}

	TouchInputManager* PlatformD3D11::CreateTouchInputManager() const
	{
		return new TouchInputManagerD3D11(this);
	}

	SonyControllerInputManager* PlatformD3D11::CreateSonyControllerInputManager() const
	{
		return new SonyControllerInputManagerD3D11(*this);
	}

	Renderer3D* PlatformD3D11::CreateRenderer3D() const
	{
		return new Renderer3DD3D11(*this);
	}

	RenderTarget* PlatformD3D11::CreateRenderTarget(const Int32 width, const Int32 height) const
	{
		return new RenderTargetD3D11(*this, width, height);
	}

	Matrix44 PlatformD3D11::PerspectiveProjectionFov(const float fov, const float aspect_ratio, const float near_distance, const float far_distance) const
	{
		Matrix44 projection_matrix;
		projection_matrix.PerspectiveFovD3D(fov, aspect_ratio, near_distance, far_distance);
		return projection_matrix;
	}

	Matrix44 PlatformD3D11::PerspectiveProjectionFrustum(const float left, const float right, const float top, const float bottom, const float near_distance, const float far_distance) const
	{
		Matrix44 projection_matrix;
		projection_matrix.PerspectiveFrustumD3D(left, right, top, bottom, near_distance, far_distance);
		return projection_matrix;
	}


	void PlatformD3D11::BeginScene() const
	{
		ID3D11RenderTargetView* render_target_view = GetRenderTargetView();

		device_context_->OMSetRenderTargets( 1, &render_target_view, depth_stencil_view_ );
	}

	void PlatformD3D11::EndScene() const
	{
	}

	ID3D11RenderTargetView* PlatformD3D11::GetRenderTargetView() const
	{
		if(render_target())
		{
			RenderTargetD3D11* render_target_d3d11 = static_cast<RenderTargetD3D11*>(render_target());
			return render_target_d3d11->render_target_view();
		}
		else
		{
			return render_target_view_;
		}
	}

}