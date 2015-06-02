#include <input/d3d11/touch_input_manager_d3d11.h>
#include <abertay_framework.h>
#include <system/d3d11/platform_d3d11.h>
#include <system/win32/window_win32.h>

namespace abfw
{
#define INVALID_TOUCH_ID		(-1)


	TouchInputManagerD3D11::TouchInputManagerD3D11(const PlatformD3D11* platform) :
		direct_input_(NULL),
		mouse_(NULL),
		platform_(platform)
	{
		HRESULT hresult = S_OK;
		// Setup DirectInput
		hresult = DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION, IID_IDirectInput8, ( VOID** )&direct_input_, NULL );

		if(SUCCEEDED(hresult))
			hresult = direct_input_->CreateDevice(GUID_SysMouse, &mouse_, NULL);

		if(SUCCEEDED(hresult))
			hresult = mouse_->SetDataFormat(&c_dfDIMouse);

		if(SUCCEEDED(hresult))
			hresult = mouse_->SetCooperativeLevel(platform_->window()->hwnd(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);


		if(SUCCEEDED(hresult))
		{
			DIPROPDWORD dipdw;
			dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
			dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			dipdw.diph.dwObj        = 0;
			dipdw.diph.dwHow        = DIPH_DEVICE;
			dipdw.dwData            = 16;

			hresult = mouse_->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
		}

		if(FAILED(hresult))
			CleanUp();

		for(Int32 panel_index=0;panel_index<1;++panel_index)
		{
			panel_enabled_[panel_index] = false;

			for(Int32 touch_index=0;touch_index<2;++touch_index)
			{
				Touch* touch = &touches_[panel_index][touch_index];
				touch->type = TT_NONE;
				touch->id = INVALID_TOUCH_ID;
				is_button_down_[touch_index] = false;
			}
		}
	}

	TouchInputManagerD3D11::~TouchInputManagerD3D11()
	{
		CleanUp();
	}

	void TouchInputManagerD3D11::CleanUp()
	{
		ReleaseNull(mouse_);
		ReleaseNull(direct_input_);
	}

	void TouchInputManagerD3D11::Update()
	{
		HRESULT hresult = S_OK;
		mouse_->Acquire();

		DIDEVICEOBJECTDATA od;
		DWORD dwElements = 1;


		while(dwElements != 0)
		{
			dwElements = 1;
			hresult = mouse_->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &od, &dwElements, 0);
			if(FAILED(hresult))
				break;

			switch(od.dwOfs)
			{
			case DIMOFS_BUTTON0:
				if(od.dwData & 0x80)
					is_button_down_[0] = true;
				else
					is_button_down_[0] = false;
				break;
			case DIMOFS_BUTTON1:
				if(od.dwData & 0x80)
					is_button_down_[1] = true;
				else
					is_button_down_[1] = false;
				break;
			}
		}

		POINT mouse_pos;
		GetCursorPos(&mouse_pos);
		ScreenToClient(platform_->window()->hwnd(), &mouse_pos);

		for(Int32 panel_index=0;panel_index<1;++panel_index)
		{
			if(panel_enabled_[panel_index])
			{
				// get the touch data for this panel
				Touch* touches = &touches_[panel_index][0];
				// check to see if we have any data to update existing touches
				for(Int32 touch_index=0;touch_index<2;++touch_index)
				{
					Touch* touch = &touches[touch_index];

					if(is_button_down_[touch_index])
					{
						touch->id = touch_index;
						if(touch->type == TT_NONE)
							touch->type = TT_NEW;
						else if(touch->type == TT_NEW)
							touch->type = TT_ACTIVE;
					}
					else
					{
						if(touch->type == TT_NEW || touch->type == TT_ACTIVE)
							touch->type = TT_RELEASED;
						else
							touch->type = TT_NONE;
					}

					if(touch->type != TT_NONE)
					{
						touch->position = Vector2(mouse_pos.x, mouse_pos.y);
					}
				}
			}
		}
	}

	void TouchInputManagerD3D11::EnablePanel(const Int32 panel_index)
	{
		panel_enabled_[panel_index] = true;
	}

	void TouchInputManagerD3D11::DisablePanel(const Int32 panel_index)
	{
		panel_enabled_[panel_index] = false;
	}
}

