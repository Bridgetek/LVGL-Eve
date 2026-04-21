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


static void eve_draw_circle_border(int32_t coord_x1, int32_t coord_y1, int32_t radius, int32_t border, uint32_t color);

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

    LV_LOG_INFO("color: r %x, g %x, b %x, a %d, r %d\n", dsc->color.red, dsc->color.green, dsc->color.blue, dsc->opa, dsc->radius);
    if (dsc->grad.stops_count > 0)
        LV_LOG_INFO("dir %d, Stop %d, color: r %x, g %x, b %x, a %d\n", dsc->grad.dir, dsc->grad.stops_count, dsc->grad.stops[0].color.red, dsc->grad.stops[0].color.green, dsc->grad.stops[0].color.blue, dsc->grad.stops[0].opa);
    if (dsc->grad.stops_count > 1)
        LV_LOG_INFO("dir %d, Stop %d, color: r %x, g %x, b %x, a %d\n", dsc->grad.dir, dsc->grad.stops_count, dsc->grad.stops[1].color.red, dsc->grad.stops[1].color.green, dsc->grad.stops[1].color.blue, dsc->grad.stops[1].opa);

    EVE_CoDl_saveContext(s_pHalContext);

    if ((coords->x1 == 0) && (coords->x2 == (s_pHalContext->Width - 1))
        && (coords->y1 == 0) && (coords->y2 == (s_pHalContext->Height - 1)))
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
        EVE_CoDl_scissorSize(s_pHalContext, lv_area_get_width(draw_unit->base_unit.clip_area) + 2 * margin,
            lv_area_get_height(draw_unit->base_unit.clip_area) + 2 * margin);

        if (dsc->grad.dir != LV_GRAD_DIR_NONE)
        {
            EVE_CoDl_stencilFunc(s_pHalContext, NEVER, 0x00, 0x00);
            EVE_CoDl_stencilOp(s_pHalContext, INCR, INCR);
        }

        EVE_CoDl_colorRgb(s_pHalContext, dsc->color.red, dsc->color.green, dsc->color.blue);
        EVE_CoDl_colorA(s_pHalContext, dsc->opa);

		if (dsc->opa > LV_OPA_MIN)
		{
			if (bg_w == bg_h && rad == LV_RADIUS_CIRCLE)
			{
				EVE_draw_circle_simple(coords->x1 + (bg_w / 2), coords->y1 + (bg_h / 2), real_radius);
			}
			else
			{
				EVE_draw_rect_simple(coords->x1, coords->y1, coords->x2, coords->y2, real_radius);
			}
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
    }
    EVE_CoDl_restoreContext(s_pHalContext);
}


/**********************
 *   STATIC FUNCTIONS
 **********************/

void lv_draw_eve_border(lv_draw_eve_unit_t * draw_unit, const lv_draw_border_dsc_t * dsc, const lv_area_t * coords)
{
    if(dsc->opa <= LV_OPA_MIN) return;
    if(dsc->width == 0) return;
    if(dsc->side == LV_BORDER_SIDE_NONE) return;

    LV_LOG_INFO("clip_area: x1: %d. y1: %d, x2: %d, y2: %d\n", draw_unit->base_unit.clip_area->x1, draw_unit->base_unit.clip_area->y1,
        draw_unit->base_unit.clip_area->x2, draw_unit->base_unit.clip_area->y2);
    LV_LOG_INFO("coords: cx1: %d, cy1: %d, cx2: %d, cy2: %d\n", coords->x1, coords->y1, coords->x2, coords->y2);
	LV_LOG_INFO("color r %x, g %x, b %x, a %d, r %d, width: %d, side %d\n", dsc->color.red, dsc->color.green, dsc->color.blue, dsc->opa, dsc->radius, dsc->width, dsc->side);

	lv_area_t draw_area;
	if (!_lv_area_intersect(&draw_area, coords, draw_unit->base_unit.clip_area))
		return; // nothing to draw

    int32_t coords_w = lv_area_get_width(coords);
    int32_t coords_h = lv_area_get_height(coords);

    if ((dsc->radius == LV_RADIUS_CIRCLE) && (coords_w == coords_h))
    {
        uint32_t color = ((uint8_t)dsc->opa << 24) | (dsc->color.red << 16) | (dsc->color.green << 8) | (dsc->color.blue);
        int32_t radius = lv_area_get_width(coords) / 2;
        eve_draw_circle_border(coords->x1 + coords_w / 2, coords->y1 + coords_h / 2, radius, dsc->width, color);
        return;
    }

	EVE_CoDl_saveContext(s_pHalContext);
    EVE_CoDl_scissorSize(s_pHalContext, lv_area_get_width(&draw_area), lv_area_get_height(&draw_area));
	EVE_CoDl_scissorXY(s_pHalContext, draw_area.x1, draw_area.y1);

    EVE_CoDl_colorRgb(s_pHalContext, dsc->color.red, dsc->color.green, dsc->color.blue);
    EVE_CoDl_colorA(s_pHalContext, dsc->opa);  

    /* Outer radius */
    int32_t rout = dsc->radius;
    int32_t short_side = LV_MIN(coords_w, coords_h);
    if(rout > (short_side >> 1)) rout = short_side >> 1;

    /* Inner radius */
    int32_t rin = rout - dsc->width;
    if(rin < 0) rin = 0;

    /* Inner area (FIXED LOGIC) */
    lv_area_t area_inner;
    lv_area_copy(&area_inner, coords);
    area_inner.x1 += ((dsc->side & LV_BORDER_SIDE_LEFT) ? dsc->width : - (dsc->width));
    area_inner.x2 -= ((dsc->side & LV_BORDER_SIDE_RIGHT) ? dsc->width : - (dsc->width));
    area_inner.y1 += ((dsc->side & LV_BORDER_SIDE_TOP) ? dsc->width : - (dsc->width));
    area_inner.y2 -= ((dsc->side & LV_BORDER_SIDE_BOTTOM) ? dsc->width : - (dsc->width));

    // workaround for top/bottom only case
    if ((dsc->side == LV_BORDER_SIDE_BOTTOM) || (dsc->side == LV_BORDER_SIDE_TOP))
    {
        EVE_CoDl_begin(s_pHalContext, LINES);
        if (dsc->width > 1)
            EVE_CoDl_lineWidth(s_pHalContext, dsc->width * 16);
        EVE_CoDl_vertex2f(s_pHalContext, area_inner.x1, area_inner.y2);
        EVE_CoDl_vertex2f(s_pHalContext, area_inner.x2, area_inner.y2);
        EVE_CoDl_end(s_pHalContext);
        EVE_CoDl_restoreContext(s_pHalContext);
        return;
    }

    EVE_CoDl_colorMask(s_pHalContext, 0, 0, 0, 1);
    EVE_CoDl_stencilFunc(s_pHalContext, ALWAYS, 0, 1);
    EVE_CoDl_stencilOp(s_pHalContext, REPLACE, REPLACE);
    EVE_draw_rect_simple(coords->x1, coords->y1, coords->x2, coords->y2, 0);

    EVE_CoDl_blendFunc(s_pHalContext, ONE, ZERO);
    EVE_draw_rect_simple(area_inner.x1 - 2, area_inner.y1 - 2, area_inner.x2 + 2, area_inner.y2 + 2, rin);

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
}

void lv_draw_eve_box_shadow(lv_draw_eve_unit_t *draw_unit, const lv_draw_box_shadow_dsc_t *dsc,
                            const lv_area_t * coords)
{
    /* Visibility check */
    if (dsc->width == 0 || dsc->opa <= LV_OPA_MIN) return;

    if(dsc->width == 1 && dsc->spread <= 0 &&
       dsc->ofs_x == 0 && dsc->ofs_y == 0)
        return;
    
    LV_LOG_INFO("clip_area: x1: %d. y1: %d, x2: %d, y2: %d\n", draw_unit->base_unit.clip_area->x1, draw_unit->base_unit.clip_area->y1,
        draw_unit->base_unit.clip_area->x2, draw_unit->base_unit.clip_area->y2);
    LV_LOG_INFO("coords: cx1: %d, cy1: %d, cx2: %d, cy2: %d\n", coords->x1, coords->y1, coords->x2, coords->y2);
    LV_LOG_INFO("shadow width %d, spread %d, offset x %d, y %d\n", dsc->width, dsc->spread, dsc->ofs_x, dsc->ofs_y);
	LV_LOG_INFO("color r %x, g %x, b %x , a %d, r %d\n", dsc->color.red, dsc->color.green, dsc->color.blue, dsc->opa, dsc->radius);

    /*Calculate the rectangle which is blurred to get the shadow in `shadow_area`*/
    lv_area_t core_area = {
        .x1 = coords->x1 + dsc->ofs_x - dsc->spread,
        .x2 = coords->x2 + dsc->ofs_x + dsc->spread,
        .y1 = coords->y1 + dsc->ofs_y - dsc->spread,
        .y2 = coords->y2 + dsc->ofs_y + dsc->spread
    };

    /*Calculate the bounding box of the shadow*/
    lv_area_t shadow_area = {
        .x1 = core_area.x1 - dsc->width,
        .x2 = core_area.x2 + dsc->width,
        .y1 = core_area.y1 - dsc->width,
        .y2 = core_area.y2 + dsc->width
    };

    /*Get clipped draw area which is the real draw area */
    lv_area_t draw_area;
    if (!_lv_area_intersect(&draw_area, &shadow_area, draw_unit->base_unit.clip_area))
        return;
    LV_LOG_INFO("core_area: x1: %d. y1: %d, x2: %d, y2: %d\n", core_area.x1, core_area.y1, core_area.x2, core_area.y2);
    LV_LOG_INFO("shadow_area: x1: %d. y1: %d, x2: %d, y2: %d\n", shadow_area.x1, shadow_area.y1, shadow_area.x2, shadow_area.y2);
    LV_LOG_INFO("draw_area: x1: %d. y1: %d, x2: %d, y2: %d\n", draw_area.x1, draw_area.y1, draw_area.x2, draw_area.y2);

	EVE_CoDl_saveContext(s_pHalContext);
    EVE_CoDl_scissorSize(s_pHalContext, lv_area_get_width(&draw_area), lv_area_get_height(&draw_area));
	EVE_CoDl_scissorXY(s_pHalContext, draw_area.x1, draw_area.y1);
    EVE_CoDl_colorRgb(s_pHalContext, dsc->color.red, dsc->color.green, dsc->color.blue);

    /* Clamp opacity */
    lv_opa_t opa = (dsc->opa > LV_OPA_MAX) ? LV_OPA_COVER : dsc->opa;
	
	int32_t short_side;
	/*Get the clamped radius*/
	int32_t sh_r = dsc->radius + dsc->width / 2;
	short_side = LV_MIN(lv_area_get_width(&shadow_area), lv_area_get_height(&shadow_area));
	if (sh_r > short_side / 2)
		sh_r = short_side / 2;
	int32_t w = lv_area_get_width(&draw_area);
	int32_t h = lv_area_get_height(&draw_area);

    /* draw shadow area */
    uint32_t shadow_width = abs(core_area.x1 - draw_area.x1); // real shadow width after clip
    /* Step size (trade-off: quality vs DL size) */
    uint32_t step = LV_MAX(2, shadow_width / 5);
    opa = dsc->opa >> (shadow_width / step);
    if (opa == 0) opa = 1;
	for (uint32_t i = 0; i < shadow_width; i += step) 
	{
		LV_LOG_INFO("opa: %d\n", opa);
		EVE_CoDl_colorA(s_pHalContext, opa);
		if (w == h && dsc->radius == LV_RADIUS_CIRCLE)
		{
			EVE_draw_circle_simple((core_area.x1 - shadow_width + i) + ((w / 2) - i), (core_area.y1 - shadow_width + i) + ((h / 2) - i), ((w / 2) - i));
		}
		else
		{
			EVE_draw_rect_simple((core_area.x1 - shadow_width) + i, (core_area.y1 - shadow_width) + i, (core_area.x2 + shadow_width) - i, (core_area.y2 + shadow_width) - i, sh_r);
		}
		opa = 2 * opa > 128 ? 128 : 2 * opa;
    }

	/*draw spread area*/
	if (dsc->spread > 0)
	{
		EVE_CoDl_colorA(s_pHalContext, opa);
		w = lv_area_get_width(&core_area);
		h = lv_area_get_height(&core_area);
		if (w == h && dsc->radius == LV_RADIUS_CIRCLE)
		{
			EVE_draw_circle_simple(core_area.x1 + (w / 2), core_area.y1 + (h / 2), w / 2);
		}
		else
		{
			/*Get the clamped radius*/
			int32_t sp_r = dsc->radius + dsc->spread / 2;
			short_side = LV_MIN(lv_area_get_width(&core_area), lv_area_get_height(&core_area));
			if (sp_r > short_side / 2)
				sp_r = short_side / 2;
			EVE_draw_rect_simple(core_area.x1, core_area.y1, core_area.x2, core_area.y2, sp_r);
		}
	}

	EVE_CoDl_restoreContext(s_pHalContext);
}

static void eve_draw_circle_border(int32_t coord_x1, int32_t coord_y1, int32_t radius, int32_t border, uint32_t color)
{

    int16_t innerRadius = radius - border;

    // Use local rendering context
    EVE_CoDl_colorArgb_ex(s_pHalContext, color);

    EVE_CoDl_saveContext(s_pHalContext);
    // Outer reset
    EVE_CoDl_colorMask(s_pHalContext, 0, 0, 0, 1);
    EVE_CoDl_stencilFunc(s_pHalContext, ALWAYS, 0, 1);
    EVE_CoDl_stencilOp(s_pHalContext, REPLACE, REPLACE);
    EVE_draw_circle_simple(coord_x1, coord_y1, radius);

    // Inner alpha quantity
    EVE_CoDl_blendFunc(s_pHalContext, ONE, ZERO);
    EVE_draw_circle_simple(coord_x1, coord_y1, innerRadius + 2);

    // Inner alpha edge mask
    EVE_CoDl_stencilFunc(s_pHalContext, ALWAYS, 1, 1);
    EVE_CoDl_stencilOp(s_pHalContext, REPLACE, REPLACE);
    EVE_CoDl_blendFunc(s_pHalContext, ZERO, ONE_MINUS_SRC_ALPHA);
    EVE_CoDl_colorA(s_pHalContext, 255);
    EVE_draw_circle_simple(coord_x1, coord_y1, innerRadius);

    // Inner color, outer circle stencil mask
    EVE_CoDl_colorMask(s_pHalContext, 1, 1, 1, 1);
    EVE_CoDl_blendFunc(s_pHalContext, DST_ALPHA, ONE_MINUS_DST_ALPHA);
    EVE_draw_circle_simple(coord_x1, coord_y1, innerRadius);

    // Outer circle
    EVE_CoDl_stencilFunc(s_pHalContext, NOTEQUAL, 1, 255);
    EVE_CoDl_blendFunc(s_pHalContext, SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
    EVE_CoDl_colorA(s_pHalContext, (color >> 24) & 0xFF);
    EVE_draw_circle_simple(coord_x1, coord_y1, radius);

    EVE_CoDl_restoreContext(s_pHalContext);
}

#endif /*LV_USE_DRAW_EVE*/

