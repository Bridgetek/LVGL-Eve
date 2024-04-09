/**
 * @file lv_draw_eve_simple.h
 *
 * @auther BridgeTek
 * @date 2024
 */

#ifndef LV_DRAW_EVE_SIMPLE_H_
#define LV_DRAW_EVE_SIMPLE_H_

#ifdef __cplusplus
extern "C" {
#endif
#if LV_USE_DRAW_EVE
/*********************
 *      INCLUDES
 *********************/
#include "Common.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/
extern void EVE_draw_circle_simple(int16_t coord_x1, int16_t coord_y1, uint16_t radius_t);
extern void EVE_draw_rect_simple(int16_t coord_x1, int16_t coord_y1, int16_t coord_x2, int16_t coord_y2,
                                 uint16_t radius);
extern void EVE_mask_round(int16_t coord_x1, int16_t coord_y1, int16_t coord_x2, int16_t coord_y2, int16_t radius);

/**********************
 *  EXTERN VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   INLINE FUNCTIONS
 **********************/

/**********************
 *   STATIC FUNCTIONS
 **********************/

#endif /*LV_USE_DRAW_EVE*/


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* LV_DRAW_EVE_SIMPLE_H_ */
