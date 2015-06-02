#ifndef _ABFW_TOUCH_INPUT_MANAGER_D3D11_H
#define _ABFW_TOUCH_INPUT_MANAGER_D3D11_H

#include <input/touch_input_manager.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace abfw
{
	class PlatformD3D11;

	class TouchInputManagerD3D11 : public TouchInputManager
	{
	public:
		TouchInputManagerD3D11(const PlatformD3D11* platform);
		~TouchInputManagerD3D11();

		void Update();

		void EnablePanel(const Int32 panel_index);
		void DisablePanel(const Int32 panel_index);
		const Touch& GetTouch(const Int32 panel_index, const Int32 touch_index) { return touches_[panel_index][touch_index]; }
		const Int32 max_num_touches() const { return 2; }
		const Int32 max_num_panels() const { return 1; }
		const bool panel_enabled(const Int32 panel_index) const { return panel_enabled_[panel_index]; }

	private:
		void CleanUp();
		LPDIRECTINPUT8			direct_input_;
		LPDIRECTINPUTDEVICE8	mouse_;
		const PlatformD3D11* platform_;
		Touch touches_[1][2];
		bool panel_enabled_[1];
		bool is_button_down_[2];
	};
}

#endif // _ABFW_TOUCH_INPUT_MANAGER_D3D11_H