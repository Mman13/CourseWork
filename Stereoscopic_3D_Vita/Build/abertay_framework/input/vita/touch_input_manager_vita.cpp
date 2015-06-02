#include <input/vita/touch_input_manager_vita.h>
#include <system/vita/platform_vita.h>
#include <abertay_framework.h>
#include <libdbg.h>
#include <iostream>

namespace abfw
{

#define NUM_TOUCH_DATA_SETS		(64)
#define INVALID_TOUCH_ID		(128)

const Int32 TouchInputManagerVita::kMaxNumTouches = SCE_TOUCH_MAX_REPORT;
const Int32 TouchInputManagerVita::kMaxNumPanels = SCE_TOUCH_PORT_MAX_NUM;

TouchInputManagerVita::TouchInputManagerVita(void)
{
	for(Int32 panel_index=0;panel_index<SCE_TOUCH_PORT_MAX_NUM;++panel_index)
	{
		Int32 return_value = SCE_OK;
		panel_enabled_[panel_index] = false;

		// Store the panel info
		return_value = sceTouchGetPanelInfo(panel_index, &panel_info_[panel_index]);
		SCE_DBG_ASSERT(return_value == SCE_OK);


		for(Int32 touch_index=0;touch_index<SCE_TOUCH_MAX_REPORT;++touch_index)
		{
			Touch* touch = &touches_[panel_index][touch_index];
			touch->type = TT_NONE;
			touch->id = INVALID_TOUCH_ID;
		}

		std::cout << "Panel " << panel_index
			<< " Active X(" << panel_info_[panel_index].minAaX << "->" << panel_info_[panel_index].maxAaX
			<< ") Y(" << panel_info_[panel_index].minAaY << "->" << panel_info_[panel_index].maxAaY
			<< ") Disp X(" << panel_info_[panel_index].minDispX << "->" << panel_info_[panel_index].maxDispX
			<< ") Y(" << panel_info_[panel_index].minDispY << "->" << panel_info_[panel_index].maxDispY
			<<")" << std::endl;
	}
}


TouchInputManagerVita::~TouchInputManagerVita(void)
{
}

void TouchInputManagerVita::Update()
{
	for(Int32 panel_index=0;panel_index<SCE_TOUCH_PORT_MAX_NUM;++panel_index)
	{
		// get the touch data for this panel
		Touch* touches = &touches_[panel_index][0];
		bool new_touch[SCE_TOUCH_MAX_REPORT];

		// initialise touch data for this panel
		for(Int32 touch_index=0;touch_index<SCE_TOUCH_MAX_REPORT;++touch_index)
		{
			// initialise touch data
			new_touch[touch_index] = true;
			Touch* touch = &touches[touch_index];

			// invalid the touch id of any touches that were released last update
			if(touch->type == TT_RELEASED)
				touch->id = INVALID_TOUCH_ID;

			touch->type = TT_NONE;
		}

		if(panel_enabled_[panel_index])
		{
			SceTouchData touch_data[NUM_TOUCH_DATA_SETS];
			Int32 num_touch_datasets = sceTouchRead(panel_index, touch_data, NUM_TOUCH_DATA_SETS);
			// under normal circumstances we will just have one set of touch data
			// but touch data can be buffered if time between read calls is too long
			// the newest dataset is at the end of the buffer
			if(num_touch_datasets)
			{
				// get the newest data from the buffer
				SceTouchData* data_read = &touch_data[num_touch_datasets-1];

				// check to see if we have any data to update existing touches
				for(Int32 touch_index=0;touch_index<SCE_TOUCH_MAX_REPORT;++touch_index)
				{
					Touch* touch = &touches[touch_index];

					for (Int32 touch_report_index = 0; touch_report_index < data_read->reportNum; ++touch_report_index)
					{
						SceTouchReport* touch_report = &data_read->report[touch_report_index];
						if(touch->id == touch_report->id)
						{
							new_touch[touch_report_index] = false;
							touch->position = GetCursorPosition(panel_index, *touch_report);
							touch->type = TT_ACTIVE;
						}
					}
				}

				// add any new touches
				for (Int32 touch_report_index = 0; touch_report_index < data_read->reportNum; ++touch_report_index)
				{
					SceTouchReport* touch_report = &data_read->report[touch_report_index];
					if(new_touch[touch_report_index])
					{
						// check to see if we have any data to update existing touches
						for(Int32 touch_index=0;touch_index<SCE_TOUCH_MAX_REPORT;++touch_index)
						{
							Touch* touch = &touches[touch_index];

							if(touch->type == TT_NONE)
							{
								touch->type = TT_NEW;
								touch->position = GetCursorPosition(panel_index, *touch_report);
								touch->id = touch_report->id;
								break;
							}
						}
					}
				}
			}
		}

		// go through all the touches and see if any have just been released
		// if the still have a valid touch ID but their type is TT_NONE i.e. not been updated by the read call
		// then this touch has just been released
		for(Int32 touch_index=0;touch_index<SCE_TOUCH_MAX_REPORT;++touch_index)
		{
			Touch* touch = &touches[touch_index];

			if(touch->type == TT_NONE && touch->id != INVALID_TOUCH_ID)
				touch->type = TT_RELEASED;
		}
	}
}

void TouchInputManagerVita::EnablePanel(const Int32 panel_index)
{
		Int32 return_value = SCE_OK;
		return_value = sceTouchSetSamplingState(panel_index, SCE_TOUCH_SAMPLING_STATE_START);
		SCE_DBG_ASSERT(return_value == SCE_OK);
		panel_enabled_[panel_index] = true;
}

void TouchInputManagerVita::DisablePanel(const Int32 panel_index)
{
		Int32 return_value = SCE_OK;
		return_value = sceTouchSetSamplingState(panel_index, SCE_TOUCH_SAMPLING_STATE_STOP);
		SCE_DBG_ASSERT(return_value == SCE_OK);
		panel_enabled_[panel_index] = false;
}

Vector2 TouchInputManagerVita::GetCursorPosition(const Int32 panel_index, const SceTouchReport& report)
{
	Vector2 position;

	// Convert to screen coordinates
	float ratioX = (report.x - panel_info_[panel_index].minAaX) / (float)(panel_info_[panel_index].maxAaX  - panel_info_[panel_index].minAaX + 1);
	float ratioY = (report.y - panel_info_[panel_index].minAaY) / (float)(panel_info_[panel_index].maxAaY  - panel_info_[panel_index].minAaY + 1);
	position.x = ratioX * (float)DISPLAY_WIDTH;
	position.y = ratioY * (float)DISPLAY_HEIGHT;
//	position.x = position.x - DISPLAY_WIDTH*0.5f;
//	position.y = DISPLAY_HEIGHT *0.5f - position.y;

	return position;
}

}