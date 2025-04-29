/**
 * @file eve_lvgl.c
 * @brief lvgl demo updated for LVGL 9.2.2
 *
 * @author Bridgetek
 *
 * @date 2024
 * 
 * MIT License
 *
 * Copyright (c) [2024] [Bridgetek Pte Ltd (BRTChip)]
 */

#include "eve_lvgl.h"
#include "lvgl.h"
#include "lv_demos.h"
#include "lv_examples.h"

static EVE_HalContext s_halContext;
EVE_HalContext *s_pHalContext;

#define HSIZE 1920
#define VSIZE 1200

void eve_display_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p)
{
	EVE_CoCmd_dl(s_pHalContext, DISPLAY());
	EVE_CoCmd_swap(s_pHalContext);
	EVE_Cmd_waitFlush(s_pHalContext);

	// Restart a new display list
	EVE_CoCmd_dlStart(s_pHalContext);
	EVE_CoDl_vertexFormat(s_pHalContext, 0);

	lv_display_flush_ready(disp);
}

static void eve_touch_read(lv_indev_t *drv, lv_indev_data_t *data)
{
	uint16_t touchX = 0, touchY = 0, x, y;
	uint16_t isTouch = EVE_Hal_rd32(s_pHalContext, REG_TOUCH_RAW_XY) != 0xFFFFFFFF;

	uint32_t touch_buff = EVE_Hal_rd32(s_pHalContext, REG_TOUCH_SCREEN_XY);
	x = touch_buff >> 16;
	y = touch_buff & 0x0000FFFF;

	if (x > s_pHalContext->Width || y > s_pHalContext->Height)
	{
		isTouch = 0;
	}
	else
	{
		touchX = x;
		touchY = y;
		data->point.x = touchX;
		data->point.y = touchY;
	}
	data->state = isTouch ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

void lv_setup(void)
{
	lv_display_t *display;

	lv_color16_t buf1;
#define DRAW_BUFFER_SIZE (HSIZE * VSIZE * LV_COLOR_DEPTH /8)

	printf("height %d, width %d\n", s_pHalContext->Height, s_pHalContext->Width);
	display = lv_display_create(s_pHalContext->Width, s_pHalContext->Height);
	lv_display_set_flush_cb(display, eve_display_flush);

	lv_display_set_buffers(display, &buf1, NULL, DRAW_BUFFER_SIZE, LV_DISPLAY_RENDER_MODE_FULL);

	lv_indev_t *indev_touchpad = lv_indev_create();
	lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
	lv_indev_set_read_cb(indev_touchpad, eve_touch_read);
}

int main(int argc, char *argv[])
{
	s_pHalContext = &s_halContext;
	Gpu_Init(s_pHalContext);
	LVDS_Config(s_pHalContext, RGB8, MODE_PICTURE);
	lv_init();
	lv_tick_set_cb((lv_tick_get_cb_t)EVE_millis);
	lv_setup();
	// read and store calibration setting
#if !defined(BT8XXEMU_PLATFORM) && GET_CALIBRATION == 1
	EVE_Calibrate(s_pHalContext);
	Calibration_Save(s_pHalContext);
#endif

	EVE_Util_clearScreen(s_pHalContext);

	//WelcomeScreen(s_pHalContext, info);
	EVE_CoCmd_dlStart(s_pHalContext);
	EVE_CoDl_vertexFormat(s_pHalContext, 0);
	//lv_example_get_started_1();
	//lv_demo_widgets();
	lv_demo_benchmark();
	//lv_demo_music();


	while (1)
	{
		lv_timer_handler();
	}

	EVE_Util_clearScreen(s_pHalContext);
	EVE_Hal_close(s_pHalContext);
	EVE_Hal_release();
	lv_deinit();
#if !defined(BT8XXEMU_PLATFORM) && GET_CALIBRATION == 1
	Calibration_Restore(s_pHalContext);
#endif
	LVDS_Config(s_pHalContext, YCBCR, MODE_PICTURE);
	return 0;
}


// ************************************ application ************************************
