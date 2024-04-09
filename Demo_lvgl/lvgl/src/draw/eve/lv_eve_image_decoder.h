/**
 * @file lv_eve_image_decoder.h
 *
 * @auther BridgeTek
 * @date 2024
 */

#ifndef LV_EVE_IMAGE_DECODER_H_
#define LV_EVE_IMAGE_DECODER_H_

#ifdef __cplusplus
extern "C" {
#endif
#if LV_USE_DRAW_EVE
#include "../../misc/lv_types.h"
#include "../../misc/lv_area.h"
#include "../lv_draw_buf.h"
#include "../lv_image_decoder.h"

void eve_decoder_init(void);
/**
 * Get info about a PNG image
 * @param decoder   pointer to the decoder where this function belongs
 * @param src       can be file name or pointer to a C array
 * @param header    image information is set in header parameter
 * @return          LV_RESULT_OK: no error; LV_RESULT_INVALID: can't get the info
 */

lv_result_t eve_decoder_info(lv_image_decoder_t * decoder, const void * src, lv_image_header_t * header);
/**
 * Open a PNG image and decode it into dsc.decoded
 * @param decoder   pointer to the decoder where this function belongs
 * @param dsc       image descriptor
 * @return          LV_RESULT_OK: no error; LV_RESULT_INVALID: can't open the image
 */
lv_result_t eve_decoder_open(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc,
                             const lv_image_decoder_args_t * args);

/**
 * Close PNG image and free data
 * @param decoder   pointer to the decoder where this function belongs
 * @param dsc       image decoder descriptor
 * @return          LV_RESULT_OK: no error; LV_RESULT_INVALID: can't open the image
 */
void eve_decoder_close(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc);

#endif /*LV_USE_DRAW_EVE*/


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* LV_EVE_IMAGE_DECODER_H_ */