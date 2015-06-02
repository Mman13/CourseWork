#ifndef _ABFW_TOUCH_INPUT_MANAGER_H
#define _ABFW_TOUCH_INPUT_MANAGER_H

#include <abertay_framework.h>
#include <maths/vector2.h>

namespace abfw
{
	enum TouchType
	{
		TT_NONE = 0,
		TT_NEW,
		TT_ACTIVE,
		TT_RELEASED
	};

	struct Touch
	{
		Int32 id;
		TouchType type;
		Vector2 position;
	};

	class TouchInputManager
	{
	public:
		virtual ~TouchInputManager();

		virtual void Update() = 0;

		virtual void EnablePanel(const Int32 panel_index) = 0;
		virtual void DisablePanel(const Int32 panel_index) = 0;

		virtual const Touch& GetTouch(const Int32 panel_index, const Int32 touch_index)  = 0;
		virtual const Int32 max_num_touches() const  = 0;
		virtual const Int32 max_num_panels() const = 0;
		virtual const bool panel_enabled(const Int32 panel_index) const = 0;

	};
}

#endif // _ABFW_TOUCH_MANAGER_H