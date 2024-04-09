/**
 * @file eve_lvgl.c
 * @brief lvgl demo
 *
 * @author Bridgetek
 *
 * @date 2024
 * 
 * MIT License
 *
 * Copyright (c) [2024] [Bridgetek Pte Ltd (BRTChip)]
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "eve_lvgl.h"

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lvgl/examples/lv_examples.h"

static EVE_HalContext s_halContext;
EVE_HalContext* s_pHalContext;

#define HSIZE 800

void eve_display_flush(lv_display_t* disp, const lv_area_t* area, uint8_t* color_p)
{
    EVE_CoCmd_dl(s_pHalContext, DISPLAY());
    EVE_CoCmd_swap(s_pHalContext);
    EVE_Cmd_waitFlush(s_pHalContext);

    // restart a new display list 
    EVE_CoCmd_dlStart(s_pHalContext);
    EVE_CoDl_vertexFormat(s_pHalContext, 0);

    lv_display_flush_ready(disp);
}

static void eve_touch_read(lv_indev_t* drv, lv_indev_data_t* data)
{
    uint16_t touchX=0, touchY = 0;
    uint16_t isTouch = EVE_Hal_rd32(s_pHalContext, REG_TOUCH_RAW_XY) != 0xFFFFFFFF;

    uint32_t x = EVE_Hal_rd16(s_pHalContext, REG_TOUCH_SCREEN_XY + 2);
    uint32_t y = EVE_Hal_rd16(s_pHalContext, REG_TOUCH_SCREEN_XY + 4);

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

    data->state = (isTouch == 0 ? LV_INDEV_STATE_REL : LV_INDEV_STATE_PR);
}

void lv_setup(void)
{
    lv_display_t *display;
    static uint8_t buf1[HSIZE * 160]; // TODO: change with LCD size, Declare a buffer for 1/10 screen size?

    printf("height %d, width %d\n", s_pHalContext->Height, s_pHalContext->Width);
    display = lv_display_create(s_pHalContext->Width, s_pHalContext->Height);
    lv_display_set_flush_cb(display, eve_display_flush);
    lv_display_set_buffers(display, buf1, NULL, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_render_mode(display, LV_DISPLAY_RENDER_MODE_FULL); /*Force to use LV_DISPLAY_RENDER_MODE_FULL*/

    /*Register a touchpad input device*/
    lv_indev_t *indev_touchpad;
    indev_touchpad = lv_indev_create();
    lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_touchpad, eve_touch_read);
}

// ************************************ main loop ************************************
int main(int argc, char* argv[])
{
    s_pHalContext = &s_halContext;
    Gpu_Init(s_pHalContext);

    lv_init(); /* LVGL Init */
    lv_tick_set_cb((lv_tick_get_cb_t)EVE_millis);
    lv_setup();

    // read and store calibration setting
#if !defined(BT8XXEMU_PLATFORM) && GET_CALIBRATION == 1
    Esd_Calibrate(s_pHalContext);
    Calibration_Save(s_pHalContext);
#endif

    EVE_Util_clearScreen(s_pHalContext);

    char* info[] =
    { "Lvgl demo",
        "Support QVGA, WQVGA, WVGA",
        "EVE2/3/4",
        "IDM2040"
    };

    //WelcomeScreen(s_pHalContext, info);
    EVE_CoCmd_dlStart(s_pHalContext);
    EVE_CoDl_vertexFormat(s_pHalContext, 0);
    //lv_example_get_started_1();
    lv_demo_widgets();
    //lv_demo_benchmark();
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

    return 0;
}

// ************************************ application ************************************