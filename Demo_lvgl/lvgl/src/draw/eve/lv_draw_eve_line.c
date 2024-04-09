/**
 * @file lv_draw_eve_line.c
 *
 * @auther Bridgetek
 * @date 2024
 */
/*********************
 *      INCLUDES
 *********************/
#if LV_USE_DRAW_EVE
#include "lv_draw_eve.h"

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

void lv_draw_eve_line(lv_draw_eve_unit_t * draw_unit, const lv_draw_line_dsc_t * dsc)
{

    if(dsc->width == 0)
        return;
    if(dsc->opa <= LV_OPA_MIN)
        return;
    if(dsc->p1.x == dsc->p2.x && dsc->p1.y == dsc->p2.y)
        return;
    LV_LOG_INFO("clip_area: x1: %d. y1: %d, x2: %d, y2: %d\n", draw_unit->base_unit.clip_area->x1, draw_unit->base_unit.clip_area->y1,
        draw_unit->base_unit.clip_area->x2, draw_unit->base_unit.clip_area->y2);
    LV_LOG_INFO("line: x1: %d, y1: %d, x2: %d, y2: %d, width %d\n", dsc->p1.x, dsc->p1.y, dsc->p2.x, dsc->p2.y, dsc->width);

    uint32_t line_w = dsc->width * 8;
    EVE_CoDl_scissorXY(s_pHalContext, draw_unit->base_unit.clip_area->x1, draw_unit->base_unit.clip_area->y1);
    EVE_CoDl_scissorSize(s_pHalContext, draw_unit->base_unit.clip_area->x2 - draw_unit->base_unit.clip_area->x1,
        draw_unit->base_unit.clip_area->y2 - draw_unit->base_unit.clip_area->y1);
    EVE_CoDl_colorRgb(s_pHalContext, dsc->color.red, dsc->color.green, dsc->color.blue);
    EVE_CoDl_colorA(s_pHalContext, dsc->opa);

    if(dsc->dash_gap && dsc->dash_width) {
        /*TODO*/
    }
    /* Check if it's a vertical or horizontal line without rounding */
    bool is_vertical = (dsc->p1.x == dsc->p2.x);
    bool is_horizontal = (dsc->p1.y == dsc->p2.y);
    bool no_round = (!dsc->round_end || !dsc->round_start);

    if((is_vertical || is_horizontal) && no_round) {
        EVE_CoDl_begin(s_pHalContext, LINES);
        EVE_CoDl_lineWidth(s_pHalContext, line_w);
        EVE_CoDl_vertex2f(s_pHalContext, dsc->p1.x, dsc->p1.y);
        EVE_CoDl_vertex2f(s_pHalContext, dsc->p2.x, dsc->p2.y);
        EVE_CoDl_end(s_pHalContext);
    }
    else {
        /* Draw inclined line or line with rounding (not possible without rounding)*/
        EVE_CoDl_begin(s_pHalContext, LINE_STRIP);
        EVE_CoDl_lineWidth(s_pHalContext, line_w);
        EVE_CoDl_vertex2f(s_pHalContext, dsc->p1.x, dsc->p1.y);
        EVE_CoDl_vertex2f(s_pHalContext, dsc->p2.x, dsc->p2.y);
        EVE_CoDl_end(s_pHalContext);
    }

}

#endif /*LV_USE_DRAW_EVE*/
