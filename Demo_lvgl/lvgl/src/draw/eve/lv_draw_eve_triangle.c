/**
 * @file lv_draw_eve_triangle.c
 *
 * @auther Bridgetek
 * @date 2024
 */

/*********************
 *      INCLUDES
 *********************/
#if LV_USE_DRAW_EVE

#include "../../misc/lv_math.h"
#include "../../stdlib/lv_mem.h"
#include "../../stdlib/lv_string.h"
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

void lv_draw_eve_triangle(lv_draw_eve_unit_t *draw_unit, const lv_draw_triangle_dsc_t *dsc)
{

    lv_area_t tri_area;
    tri_area.x1 = (int32_t)LV_MIN3(dsc->p[0].x, dsc->p[1].x, dsc->p[2].x);
    tri_area.y1 = (int32_t)LV_MIN3(dsc->p[0].y, dsc->p[1].y, dsc->p[2].y);
    tri_area.x2 = (int32_t)LV_MAX3(dsc->p[0].x, dsc->p[1].x, dsc->p[2].x);
    tri_area.y2 = (int32_t)LV_MAX3(dsc->p[0].y, dsc->p[1].y, dsc->p[2].y);

    bool is_common;
    lv_area_t draw_area;
    is_common = _lv_area_intersect(&draw_area, &tri_area, draw_unit->base_unit.clip_area);
    if (!is_common)
        return;

    lv_point_t p[3];
    /*If there is a vertical side use it as p[0] and p[1]*/
    if (dsc->p[0].x == dsc->p[1].x)
    {
        p[0] = lv_point_from_precise(&dsc->p[0]);
        p[1] = lv_point_from_precise(&dsc->p[1]);
        p[2] = lv_point_from_precise(&dsc->p[2]);
    }
    else if (dsc->p[0].x == dsc->p[2].x)
    {
        p[0] = lv_point_from_precise(&dsc->p[0]);
        p[1] = lv_point_from_precise(&dsc->p[2]);
        p[2] = lv_point_from_precise(&dsc->p[1]);
    }
    else if (dsc->p[1].x == dsc->p[2].x)
    {
        p[0] = lv_point_from_precise(&dsc->p[1]);
        p[1] = lv_point_from_precise(&dsc->p[2]);
        p[2] = lv_point_from_precise(&dsc->p[0]);
    }
    else
    {
        p[0] = lv_point_from_precise(&dsc->p[0]);
        p[1] = lv_point_from_precise(&dsc->p[1]);
        p[2] = lv_point_from_precise(&dsc->p[2]);

        /*Set the smallest y as p[0]*/
        if (p[0].y > p[1].y)
            lv_point_swap(&p[0], &p[1]);
        if (p[0].y > p[2].y)
            lv_point_swap(&p[0], &p[2]);

        /*Set the greatest y as p[1]*/
        if (p[1].y < p[2].y)
            lv_point_swap(&p[1], &p[2]);
    }

    /*Be sure p[0] is on the top*/
    if (p[0].y > p[1].y)
        lv_point_swap(&p[0], &p[1]);

    EVE_CoDl_scissorXY(s_pHalContext, draw_unit->base_unit.clip_area->x1, draw_unit->base_unit.clip_area->y1);
    EVE_CoDl_scissorSize(s_pHalContext, draw_unit->base_unit.clip_area->x2 - draw_unit->base_unit.clip_area->x1,
        draw_unit->base_unit.clip_area->y2 - draw_unit->base_unit.clip_area->y1);

    EVE_CoDl_colorRgb(s_pHalContext, dsc->bg_color.red, dsc->bg_color.green, dsc->bg_color.blue);
    EVE_CoDl_colorA(s_pHalContext, dsc->bg_opa);

    EVE_CoDl_saveContext(s_pHalContext);

    EVE_CoDl_colorMask(s_pHalContext, 0, 0, 0, 0);
    EVE_CoDl_stencilOp(s_pHalContext, KEEP, INVERT);
    EVE_CoDl_stencilFunc(s_pHalContext, ALWAYS, 255, 255);
    EVE_CoDl_begin(s_pHalContext, EDGE_STRIP_B);

    EVE_CoDl_vertex2f(s_pHalContext, p[0].x, p[0].y);
    EVE_CoDl_vertex2f(s_pHalContext, p[1].x, p[1].y);
    EVE_CoDl_vertex2f(s_pHalContext, p[2].x, p[2].y);
    EVE_CoDl_vertex2f(s_pHalContext, p[0].x, p[0].y);

    EVE_CoDl_colorMask(s_pHalContext, 1, 1, 1, 1);
    EVE_CoDl_stencilFunc(s_pHalContext, EQUAL, 255, 255);

    EVE_CoDl_vertex2f(s_pHalContext, 0, 0);
    EVE_CoDl_vertex2f(s_pHalContext, 1022, 0);
    EVE_CoDl_end(s_pHalContext);

    EVE_CoDl_restoreContext(s_pHalContext);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#endif /*LV_USE_DRAW_EVE*/


