/**
 * @file lv_draw_eve.h
 *
 * @auther Bridgetek
 * @date 2024
 */
#ifndef LV_DRAW_EVE_H_
#define LV_DRAW_EVE_H_

#ifdef __cplusplus
extern "C" {
#endif

#if LV_USE_DRAW_EVE
/*********************
 *      INCLUDES
 *********************/
#include "../lv_draw.h"
#include "../../misc/lv_area.h"
#include "../../misc/lv_color.h"
#include "../../display/lv_display.h"
#include "../../osal/lv_os.h"

#include "lv_draw_eve_simple.h"

/*********************
 *      DEFINES
 *********************/


/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_draw_unit_t base_unit;
    lv_draw_task_t * task_act;
    void * eve;
} lv_draw_eve_unit_t;


/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Initialize the EVE renderer. Called in internally.
 * It creates as many SW renderers as defined in LV_DRAW_SW_DRAW_UNIT_CNT
 */
extern void lv_draw_eve_init(void);

/**
 * Deinitialize the EVE renderers
 */


/**
 * Fill an area using EVE render. Handle gradient and radius.
 * @param draw_unit     pointer to a draw unit
 * @param dsc           the draw descriptor
 * @param coords        the coordinates of the rectangle
 */
extern void lv_draw_eve_fill(lv_draw_eve_unit_t *draw_unit, const lv_draw_fill_dsc_t *dsc, const lv_area_t *coords);

/**
 * Draw border with EVE render.
 * @param draw_unit     pointer to a draw unit
 * @param dsc           the draw descriptor
 * @param coords        the coordinates of the rectangle
 */
extern void lv_draw_eve_border(lv_draw_eve_unit_t *draw_unit, const lv_draw_border_dsc_t *dsc, const lv_area_t *coords);

/**
 * Draw box shadow with EVE render.
 * @param draw_unit     pointer to a draw unit
 * @param dsc           the draw descriptor
 * @param coords        the coordinates of the rectangle for which the box shadow should be drawn
 */
//extern void lv_draw_eve_box_shadow(lv_draw_eve_unit_t *draw_unit, const lv_draw_box_shadow_dsc_t *dsc, const lv_area_t *coords);

/**
 * Draw an image with EVE render. It handles image decoding, tiling, transformations, and recoloring.
 * @param draw_unit     pointer to a draw unit
 * @param dsc           the draw descriptor
 * @param coords        the coordinates of the image
 */
extern void lv_draw_eve_image(lv_draw_eve_unit_t *draw_unit, const lv_draw_image_dsc_t *dsc, const lv_area_t *coords);

/**
 * Draw a label with EVE render.
 * @param draw_unit     pointer to a draw unit
 * @param dsc           the draw descriptor
 * @param coords        the coordinates of the label
 */
extern void lv_draw_eve_label(lv_draw_eve_unit_t *draw_unit, const lv_draw_label_dsc_t *dsc, const lv_area_t *coords);

/**
 * Draw an arc with EVE render.
 * @param draw_unit     pointer to a draw unit
 * @param dsc           the draw descriptor
 * @param coords        the coordinates of the arc
 */
extern void lv_draw_eve_arc(lv_draw_eve_unit_t *draw_unit, const lv_draw_arc_dsc_t *dsc, const lv_area_t *coords);

/**
 * Draw a line with EVE render.
 * @param draw_unit     pointer to a draw unit
 * @param dsc           the draw descriptor
 */
extern void lv_draw_eve_line(lv_draw_eve_unit_t *draw_unit, const lv_draw_line_dsc_t *dsc);

/**
 * Blend a layer with EVE render
 * @param draw_unit     pointer to a draw unit
 * @param dsc           the draw descriptor
 * @param coords        the coordinates of the layer
 */
extern void lv_draw_eve_layer(lv_draw_eve_unit_t * draw_unit, const lv_draw_image_dsc_t * dsc, const lv_area_t * coords);

/**
 * Draw a triangle with EVE render.
 * @param draw_unit     pointer to a draw unit
 * @param dsc           the draw descriptor
 */
extern void lv_draw_eve_triangle(lv_draw_eve_unit_t *draw_unit, const lv_draw_triangle_dsc_t *dsc);

/**
 * Mask out a rectangle with radius from a current layer
 * @param draw_unit     pointer to a draw unit
 * @param dsc           the draw descriptor
 * @param coords        the coordinates of the mask
 */
//void lv_draw_eve_mask_rect(lv_draw_eve_unit_t *draw_unit, const lv_draw_mask_rect_dsc_t *dsc, const lv_area_t *coords);






/**********************
 *      MACROS
 **********************/

#define DEGREES(n) ((65536UL * (n)) / 3600)


#endif /*LV_USE_DRAW_EVE*/

#ifdef __cplusplus
} /*extern "C"*/
#endif


#endif /* LV_DRAW_EVE_H_ */
