/**
 * @file lv_draw_eve_decoder.c
 *
 * @auther Bridgetek
 * @date 2024
 */

/*********************
 *      INCLUDES
 *********************/
#if LV_USE_DRAW_EVE
#include "lv_eve_image_decoder.h"

/**************************TODO**************************************/

void eve_decoder_init(void)
{
    lv_image_decoder_t * decoder;

    decoder = lv_image_decoder_create();
    LV_ASSERT_MALLOC(decoder);
    if(decoder == NULL) {
        LV_LOG_WARN("Out of memory");
        return;
    }

    lv_image_decoder_set_info_cb(decoder, eve_decoder_info);
    lv_image_decoder_set_open_cb(decoder, eve_decoder_open);
    lv_image_decoder_set_close_cb(decoder, eve_decoder_close);
    //lv_image_decoder_set_get_area_cb(decoder, eve_decoder_get_area);
}




lv_result_t eve_decoder_info(lv_image_decoder_t * decoder, const void * src, lv_image_header_t * header)
{
    /*Check whether the type `src` is known by the decoder*/
    //if(is_png(src) == false) return LV_RESULT_INVALID;

    /* Read the PNG header and find `width` and `height` */


    //header->cf = LV_COLOR_FORMAT_ARGB8888;
    //header->w = width;
    //header->h = height;
    lv_result_t res;
    return res;
}

lv_result_t eve_decoder_open(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc,
                             const lv_image_decoder_args_t * args)
{
    (void) decoder; /*Unused*/

    /*Check whether the type `src` is known by the decoder*/
    //if(is_png(dsc->src) == false) return LV_RESULT_INVALID;

    /*Decode and store the image. If `dsc->decoded` is `NULL`, the `read_line` function will be called to get the image data line-by-line*/
    //dsc->decoded = my_png_decoder(dsc->src);

    /*Change the color format if decoded image format is different than original format. For PNG it's usually decoded to ARGB8888 format*/
    //sc->decoded.header.cf = LV_COLOR_FORMAT_...

    /*Call a binary image decoder function if required. It's not required if `my_png_decoder` opened the image in true color format.*/
    lv_result_t res;// = lv_bin_decoder_open(decoder, dsc);

    return res;
}

void eve_decoder_close(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc)
{
    /*Free all allocated data*/

    /*Call the built-in close function if the built-in open/read_line was used*/
    //lv_bin_decoder_close(decoder, dsc);

}
#endif /*LV_USE_DRAW_EVE*/
