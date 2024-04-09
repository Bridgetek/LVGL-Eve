/**
 * @file lv_draw_eve_simple.c
 *
 * @auther Bridgetek
 * @date 2024
 */
#if LV_USE_DRAW_EVE
/*********************
 *      INCLUDES
 *********************/
#include "lv_draw_eve_simple.h"
#include "lvgl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/***********************
 * GLOBAL VARIABLES
 ***********************/
extern Gpu_Hal_Context_t *s_pHalContext;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void EVE_draw_circle_simple(int16_t x, int16_t y, uint16_t radius_t)
{
    LV_LOG_INFO("x %d, y %d, radius %d\n", x, y, radius_t);
    EVE_CoDl_begin(s_pHalContext, POINTS);
    EVE_CoDl_pointSize(s_pHalContext, radius_t * 16);
    EVE_CoDl_vertex2f(s_pHalContext, x, y);
    EVE_CoDl_end(s_pHalContext);

}

void EVE_draw_rect_simple(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t radius)
{
    LV_LOG_INFO("x1 %d, y1 %d, x2 %d, y2 %d, radius %d\n", x1, y1, x2, y2, radius);
    EVE_CoDl_begin(s_pHalContext, RECTS);
    if(radius > 1) {
        EVE_CoDl_lineWidth(s_pHalContext, radius * 16);
    }

    EVE_CoDl_vertex2f(s_pHalContext, x1 + radius, y1 + radius);
    EVE_CoDl_vertex2f(s_pHalContext, x2 - radius, y2 - radius);
    EVE_CoDl_end(s_pHalContext);
}

void EVE_mask_round(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t radius)
{
    LV_LOG_INFO("x1 %d, y1 %d, x2 %d, y2 %d, radius %d\n", x1, y1, x2, y2, radius);
    EVE_CoDl_colorMask(s_pHalContext, 0, 0, 0, 1);
    EVE_CoDl_clear(s_pHalContext, 1, 1, 1);

    EVE_draw_rect_simple(x1, y1, x2, y2, radius);
    EVE_CoDl_colorMask(s_pHalContext, 1, 1, 1, 0);
    EVE_CoDl_blendFunc(s_pHalContext, DST_ALPHA, ONE_MINUS_DST_ALPHA);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#endif /*LV_USE_DRAW_EVE*/
