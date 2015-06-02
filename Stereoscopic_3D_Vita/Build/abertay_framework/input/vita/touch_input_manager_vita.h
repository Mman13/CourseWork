#ifndef _ABFW_TOUCH_INPUT_MANAGER_VITA_H
#define _ABFW_TOUCH_INPUT_MANAGER_VITA_H

#include <input/touch_input_manager.h>
#include <touch.h>
#include <abertay_framework.h>

namespace abfw
{



class TouchInputManagerVita : public TouchInputManager
{
public:
	TouchInputManagerVita();
	~TouchInputManagerVita();

	void Update();

	void EnablePanel(const Int32 panel_index);
	void DisablePanel(const Int32 panel_index);

	const Touch& GetTouch(const Int32 panel_index, const Int32 touch_index) { return touches_[panel_index][touch_index]; }
	const Int32 max_num_touches() const { return kMaxNumTouches; }
	const Int32 max_num_panels() const { return kMaxNumPanels; }
	const bool panel_enabled(const Int32 panel_index) const { return panel_enabled_[panel_index]; }
private:
	Vector2 GetCursorPosition(const Int32 panel_index, const SceTouchReport& report);

	SceTouchPanelInfo panel_info_[SCE_TOUCH_PORT_MAX_NUM];
	bool panel_enabled_[SCE_TOUCH_PORT_MAX_NUM];

	Touch touches_[SCE_TOUCH_PORT_MAX_NUM][SCE_TOUCH_MAX_REPORT];
	static const Int32 kMaxNumTouches;
	static const Int32 kMaxNumPanels;
};

}

#endif // _ABFW_TOUCH_INPUT_MANAGER_VITA_H

