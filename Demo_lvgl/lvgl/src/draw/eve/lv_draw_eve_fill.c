/**
 * @file lv_draw_eve_fill.c
 *
 * @auther Bridgetek
 * @date 2024
 */
/*********************
 *      INCLUDES
 *********************/
#if LV_USE_DRAW_EVE
#include "lv_draw_eve.h"

/**********************
 * STATIC PROTOTYPES
 **********************/


static void eve_draw_circle_border(int32_t coord_x1, int32_t coord_y1, int32_t radius, int32_t border, uint32_t color,
                                   uint8_t col_A);

/***********************
 * GLOBAL VARIABLES
 ***********************/
extern Gpu_Hal_Context_t *s_pHalContext;

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_draw_eve_fill(lv_draw_eve_unit_t * draw_unit, const lv_draw_fill_dsc_t * dsc, const lv_area_t * coords)
{

    int32_t rad = dsc->radius;
    int32_t bg_w = lv_area_get_width(coords);
    int32_t bg_h = lv_area_get_height(coords);
    int32_t real_radius = LV_MIN3(bg_w / 2, bg_h / 2, rad);

    LV_LOG_INFO("clip_area: x1: %d. y1: %d, x2: %d, y2: %d\n", draw_unit->base_unit.clip_area->x1, draw_unit->base_unit.clip_area->y1, 
        draw_unit->base_unit.clip_area->x2, draw_unit->base_unit.clip_area->y2);
    LV_LOG_INFO("coords: cx1: %d, cy1: %d, cx2: %d, cy2: %d\n", coords->x1, coords->y1, coords->x2, coords->y2);

    LV_LOG_INFO("color: r %x, g %x, b %x, a %d\n", dsc->color.red, dsc->color.green, dsc->color.blue, dsc->opa);
    if (dsc->grad.stops_count > 0)
        LV_LOG_INFO("dir %d, Stop %d, color: r %x, g %x, b %x, a %d\n", dsc->grad.dir, dsc->grad.stops_count, dsc->grad.stops[0].color.red, dsc->grad.stops[0].color.green, dsc->grad.stops[0].color.blue, dsc->grad.stops[0].opa);
    if (dsc->grad.stops_count > 1)
        LV_LOG_INFO("dir %d, Stop %d, color: r %x, g %x, b %x, a %d\n", dsc->grad.dir, dsc->grad.stops_count, dsc->grad.stops[1].color.red, dsc->grad.stops[1].color.green, dsc->grad.stops[1].color.blue, dsc->grad.stops[1].opa);

    EVE_CoDl_saveContext(s_pHalContext);

    if ((draw_unit->base_unit.clip_area->x1 == 0) && (draw_unit->base_unit.clip_area->x2 == (s_pHalContext->Width - 1))
        && (draw_unit->base_unit.clip_area->y1 == 0) && (draw_unit->base_unit.clip_area->y2 == (s_pHalContext->Height - 1)))
    {
        LV_LOG_INFO("fill backgroud\n");
        EVE_CoDl_clearColorRgb(s_pHalContext, dsc->color.red, dsc->color.green, dsc->color.blue);
        EVE_CoDl_clear(s_pHalContext, 1, 1, 1);
    }
    else {
        int margin = 0;
        if (dsc->grad.dir != LV_GRAD_DIR_NONE)
            margin = 2;
        EVE_CoDl_scissorXY(s_pHalContext, draw_unit->base_unit.clip_area->x1 - margin, draw_unit->base_unit.clip_area->y1 - margin);
        EVE_CoDl_scissorSize(s_pHalContext, draw_unit->base_unit.clip_area->x2 - draw_unit->base_unit.clip_area->x1 + 2 * margin,
            draw_unit->base_unit.clip_area->y2 - draw_unit->base_unit.clip_area->y1 + 2 * margin);

        if (dsc->grad.dir != LV_GRAD_DIR_NONE)
        {
            EVE_CoDl_stencilFunc(s_pHalContext, NEVER, 0x00, 0x00);
            EVE_CoDl_stencilOp(s_pHalContext, INCR, INCR);
        }

        EVE_CoDl_colorRgb(s_pHalContext, dsc->color.red, dsc->color.green, dsc->color.blue);
        EVE_CoDl_colorA(s_pHalContext, dsc->opa);

        if (bg_w == bg_h && rad == LV_RADIUS_CIRCLE)
        {
            EVE_draw_circle_simple(coords->x1 + (bg_w / 2), coords->y1 + (bg_h / 2), real_radius);
        }
        else
        {
            EVE_draw_rect_simple(coords->x1, coords->y1, coords->x2, coords->y2, real_radius);
        }

        if (dsc->grad.dir != LV_GRAD_DIR_NONE)
        {
            int rgb1, rgb2;
            rgb1 = (dsc->grad.stops[0].color.red << 16) | (dsc->grad.stops[0].color.green << 8) | dsc->grad.stops[0].color.blue;
            rgb2 = (dsc->grad.stops[1].color.red << 16) | (dsc->grad.stops[1].color.green << 8) | dsc->grad.stops[1].color.blue;
            
            // Add gradient
            EVE_CoDl_stencilFunc(s_pHalContext, EQUAL, 0x01, 0x01);
            EVE_CoDl_stencilOp(s_pHalContext, KEEP, KEEP);
            
            if (dsc->grad.dir == LV_GRAD_DIR_VER)
            {
                EVE_CoCmd_gradient(s_pHalContext, draw_unit->base_unit.clip_area->x1, draw_unit->base_unit.clip_area->y1, rgb1, 
                    draw_unit->base_unit.clip_area->x1, draw_unit->base_unit.clip_area->y2, rgb2);
            }
            else // LV_GRAD_DIR_HOR
            {
                EVE_CoCmd_gradient(s_pHalContext, draw_unit->base_unit.clip_area->x1, draw_unit->base_unit.clip_area->y1, rgb1,
                    draw_unit->base_unit.clip_area->x2, draw_unit->base_unit.clip_area->y1, rgb2);
            }
        }

        EVE_CoDl_restoreContext(s_pHalContext);

        // Reset stencil
        EVE_CoDl_stencilFunc(s_pHalContext, ALWAYS, 0, 255);
        EVE_CoDl_stencilOp(s_pHalContext, KEEP, KEEP);
    }
}


/**********************
 *   STATIC FUNCTIONS
 **********************/



void lv_draw_eve_border(lv_draw_eve_unit_t * draw_unit, const lv_draw_border_dsc_t * dsc, const lv_area_t * coords)
{
#if 0
    if(dsc->opa <= LV_OPA_MIN) return;
    if(dsc->width == 0) return;
    if(dsc->side == LV_BORDER_SIDE_NONE) return;

    LV_LOG_INFO("clip_area: x1: %d. y1: %d, x2: %d, y2: %d\n", draw_unit->base_unit.clip_area->x1, draw_unit->base_unit.clip_area->y1,
        draw_unit->base_unit.clip_area->x2, draw_unit->base_unit.clip_area->y2);
    LV_LOG_INFO("coords: cx1: %d, cy1: %d, cx2: %d, cy2: %d\n", coords->x1, coords->y1, coords->x2, coords->y2);
    LV_LOG_INFO("width: %d\n", dsc->width);

    int32_t coords_w = lv_area_get_width(coords);
    int32_t coords_h = lv_area_get_height(coords);
    int32_t rout = dsc->radius;
    int32_t short_side = LV_MIN(coords_w, coords_h);
    if(rout > short_side >> 1) rout = short_side >> 1;

    /*Get the inner area*/
    lv_area_t area_inner;
    lv_area_copy(&area_inner, coords);
    area_inner.x1 += ((dsc->side & LV_BORDER_SIDE_LEFT) ? dsc->width : - (dsc->width ));
    area_inner.x2 -= ((dsc->side & LV_BORDER_SIDE_RIGHT) ? dsc->width : - (dsc->width ));
    area_inner.y1 += ((dsc->side & LV_BORDER_SIDE_TOP) ? dsc->width : - (dsc->width ));
    area_inner.y2 -= ((dsc->side & LV_BORDER_SIDE_BOTTOM) ? dsc->width : - (dsc->width ));

    int32_t rin = rout - dsc->width;
    if(rin < 0) rin = 0;

    EVE_CoDl_scissorXY(s_pHalContext, coords->x1, coords->y1);
    EVE_CoDl_scissorSize(s_pHalContext, coords->x2 - coords->x1, coords->y2 - coords->y1);

    EVE_CoDl_saveContext(s_pHalContext);
    EVE_CoDl_colorRgb(s_pHalContext, dsc->color.red, dsc->color.green, dsc->color.blue);
    EVE_CoDl_colorA(s_pHalContext, dsc->opa);  

    EVE_CoDl_colorMask(s_pHalContext, 0, 0, 0, 1);
    EVE_CoDl_stencilFunc(s_pHalContext, ALWAYS, 0, 1);
    EVE_CoDl_stencilOp(s_pHalContext, REPLACE, REPLACE);
    EVE_draw_rect_simple(coords->x1, coords->y1, coords->x2, coords->y2, 0);

    EVE_CoDl_blendFunc(s_pHalContext, ONE, ZERO);
    EVE_draw_rect_simple(area_inner.x1 - 2, area_inner.y1 - 1, area_inner.x2 + 1, area_inner.y2 + 2, rin);

    EVE_CoDl_stencilFunc(s_pHalContext, ALWAYS, 1, 1);
    EVE_CoDl_stencilOp(s_pHalContext, REPLACE, REPLACE);
    EVE_CoDl_blendFunc(s_pHalContext, ZERO, ONE_MINUS_SRC_ALPHA);
    EVE_CoDl_colorA(s_pHalContext, 255);
    EVE_draw_rect_simple(area_inner.x1, area_inner.y1, area_inner.x2, area_inner.y2, rin);

    EVE_CoDl_colorMask(s_pHalContext, 1, 1, 1, 1);

    if (dsc->side == LV_BORDER_SIDE_FULL)
    {
        EVE_CoDl_blendFunc(s_pHalContext, DST_ALPHA, ONE_MINUS_DST_ALPHA);
        EVE_draw_rect_simple(area_inner.x1, area_inner.y1, area_inner.x2, area_inner.y2, rin);
    }

    EVE_CoDl_stencilFunc(s_pHalContext, NOTEQUAL, 1, 255);
    EVE_CoDl_blendFunc(s_pHalContext, SRC_ALPHA, ONE_MINUS_SRC_ALPHA);

    EVE_CoDl_colorA(s_pHalContext, dsc->opa);
    EVE_draw_rect_simple(coords->x1, coords->y1, coords->x2, coords->y2, rout);

    EVE_CoDl_restoreContext(s_pHalContext);
#endif
}

void lv_draw_eve_box_shadow(lv_draw_eve_unit_t *draw_unit, const lv_draw_box_shadow_dsc_t *dsc,
                            const lv_area_t * coords)
{
#if 0
    /*Check whether the shadow is visible*/
    if(dsc->width == 0) return;
    if(dsc->opa <= LV_OPA_MIN) return;

    if(dsc->width == 1 && dsc->spread <= 0 &&
       dsc->ofs_x == 0 && dsc->ofs_y == 0)
        return;
    
    LV_LOG_INFO("clip_area: x1: %d. y1: %d, x2: %d, y2: %d\n", draw_unit->base_unit.clip_area->x1, draw_unit->base_unit.clip_area->y1,
        draw_unit->base_unit.clip_area->x2, draw_unit->base_unit.clip_area->y2);
    LV_LOG_INFO("coords: cx1: %d, cy1: %d, cx2: %d, cy2: %d\n", coords->x1, coords->y1, coords->x2, coords->y2);
    LV_LOG_INFO("shadow width %d, spread %d, offset x %d, y %d\n", dsc->width, dsc->spread, dsc->ofs_x, dsc->ofs_y);
    LV_LOG_INFO("color r %x, g %x, b %x , a %d\n", dsc->color.red, dsc->color.green, dsc->color.blue, dsc->opa);

    /*Calculate the rectangle which is blurred to get the shadow in `shadow_area`*/
    lv_area_t core_area;
    core_area.x1 = coords->x1  + dsc->ofs_x - dsc->spread;
    core_area.x2 = coords->x2  + dsc->ofs_x + dsc->spread;
    core_area.y1 = coords->y1  + dsc->ofs_y - dsc->spread;
    core_area.y2 = coords->y2  + dsc->ofs_y + dsc->spread;

    /*Calculate the bounding box of the shadow*/
    lv_area_t shadow_area;
    shadow_area.x1 = core_area.x1 - dsc->width / 2 - 1;
    shadow_area.x2 = core_area.x2 + dsc->width / 2 + 1;
    shadow_area.y1 = core_area.y1 - dsc->width / 2 - 1;
    shadow_area.y2 = core_area.y2 + dsc->width / 2 + 1;

    lv_opa_t opa = dsc->opa;
    if(opa > LV_OPA_MAX) opa = LV_OPA_COVER;

    /*Get clipped draw area which is the real draw area.
     *It is always the same or inside `shadow_area`*/
    lv_area_t draw_area;
    if (!_lv_area_intersect(&draw_area, &shadow_area, draw_unit->base_unit.clip_area))
        return;
    LV_LOG_INFO("core_area: x1: %d. y1: %d, x2: %d, y2: %d\n", core_area.x1, core_area.y1, core_area.x2, core_area.y2);
    LV_LOG_INFO("shadow_area: x1: %d. y1: %d, x2: %d, y2: %d\n", shadow_area.x1, shadow_area.y1, shadow_area.x2, shadow_area.y2);
    LV_LOG_INFO("draw_area: x1: %d. y1: %d, x2: %d, y2: %d\n", draw_area.x1, draw_area.y1, draw_area.x2, draw_area.y2);

    /*Consider 1 px smaller bg to be sure the edge will be covered by the shadow*/
    lv_area_t bg_area;
    lv_area_copy(&bg_area, coords);
    lv_area_increase(&bg_area, -1, -1);

    /*Get the clamped radius*/
    int32_t r_bg = dsc->radius;
    int32_t short_side = LV_MIN(lv_area_get_width(&bg_area), lv_area_get_height(&bg_area));
    if(r_bg > short_side >> 1) r_bg = short_side >> 1;

    /*Get the clamped radius*/
    int32_t r_sh = dsc->radius;
    short_side = LV_MIN(lv_area_get_width(&core_area), lv_area_get_height(&core_area));
    if(r_sh > short_side >> 1) r_sh = short_side >> 1;


    /*Get how many pixels are affected by the blur on the corners*/
    int32_t corner_size = dsc->width  + r_sh;

    EVE_CoDl_colorRgb(s_pHalContext, dsc->color.red, dsc->color.green, dsc->color.blue);
    uint32_t opa_steps = 255 / (dsc->width * 2);

    for(int steps = 1; steps <= dsc->width; steps += 3) {

        EVE_CoDl_colorA(s_pHalContext, opa_steps);
        EVE_draw_rect_simple(draw_area.x1 + steps, draw_area.y1 + steps, draw_area.x2 - steps, draw_area.y2 - steps,
                             r_sh + dsc->width / 2);

    }
#endif
}

static void eve_draw_circle_border(int32_t coord_x1, int32_t coord_y1, int32_t radius, int32_t border, uint32_t color,
                                   uint8_t col_A)
{

    int16_t innerRadius = radius - border;

    // Use local rendering context
    EVE_CoDl_colorRgb_ex(s_pHalContext, color);
    EVE_CoDl_colorA(s_pHalContext, col_A);

    EVE_CoDl_saveContext(s_pHalContext);
    // Outer reset
    EVE_CoDl_colorMask(s_pHalContext, 0, 0, 0, 1);
    EVE_CoDl_stencilFunc(s_pHalContext, ALWAYS, 0, 1);
    EVE_CoDl_stencilOp(s_pHalContext, REPLACE, REPLACE);
    EVE_draw_circle_simple(coord_x1, coord_y1, radius);

    // Inner alpha quantity
    EVE_CoDl_blendFunc(s_pHalContext, ONE, ZERO);
    EVE_draw_circle_simple(coord_x1, coord_y1, innerRadius + 2); //verificar ? 2 pixel

    // Inner alpha edge mask
    EVE_CoDl_stencilFunc(s_pHalContext, ALWAYS, 1, 1);
    EVE_CoDl_stencilOp(s_pHalContext, REPLACE, REPLACE);
    EVE_CoDl_blendFunc(s_pHalContext, ZERO, ONE_MINUS_SRC_ALPHA);
    EVE_CoDl_colorA(s_pHalContext, 255);
    EVE_draw_circle_simple(coord_x1, coord_y1, innerRadius);

    // Inner color, outer rect stencil mask
    EVE_CoDl_colorMask(s_pHalContext, 1, 1, 1, 1);
    EVE_CoDl_blendFunc(s_pHalContext, DST_ALPHA, ONE_MINUS_DST_ALPHA);
    EVE_draw_circle_simple(coord_x1, coord_y1, innerRadius);

    // Outer rect
    EVE_CoDl_stencilFunc(s_pHalContext, NOTEQUAL, 1, 255);
    EVE_CoDl_blendFunc(s_pHalContext, SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
    EVE_CoDl_colorA(s_pHalContext, col_A);
    EVE_draw_circle_simple(coord_x1, coord_y1, radius);

    EVE_CoDl_restoreContext(s_pHalContext);
}

#endif /*LV_USE_DRAW_EVE*/

