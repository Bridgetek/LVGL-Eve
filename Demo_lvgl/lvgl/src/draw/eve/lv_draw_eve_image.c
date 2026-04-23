/**
 * @file lv_draw_eve_image.c
 *
 * @auther Bridgetek
 * @date 2024
 */

/*********************
 *      INCLUDES
 *********************/
#if LV_USE_DRAW_EVE
#include "lv_draw_eve.h"
#include "eve_ram_g.h"
/*********************
 *      DEFINES
 *********************/


/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void convert_RGB565A8_to_ARGB4(const uint8_t * src, uint8_t * dst_argb4444, uint16_t width, uint16_t height);
static void convert_RGB565A8_to_ARGB1555(const uint8_t * src, uint8_t * dst, uint16_t width, uint16_t height);
static void convert_ARGB8888_to_ARGB4(const uint8_t * src, uint8_t * dst, uint16_t width, uint16_t height);

/**********************
 *  STATIC VARIABLES
 **********************/
static uint8_t handler = 0;
static uint32_t tbl[15] = { NOT_FOUND_BLOCK };
/***********************
 * GLOBAL VARIABLES
 ***********************/
extern Gpu_Hal_Context_t *s_pHalContext;

/**********************
 *      MACROS
 **********************/

#define F16_PIVOT_SHIFT(x) ((int32_t)((((x) >> 1)) * 65536L))
#define F16_SCALE_DIV_256(x) ((int32_t)(((x) / 256.0f) * 65536L))

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_draw_eve_layer(lv_draw_eve_unit_t * draw_unit, const lv_draw_image_dsc_t * draw_dsc, const lv_area_t * coords)
{
    /* no action */
}


void lv_draw_eve_image(lv_draw_eve_unit_t *draw_unit, const lv_draw_image_dsc_t *draw_dsc, const lv_area_t *coords)
{

    const lv_image_dsc_t *img_dsc = draw_dsc->src;
    const uint8_t *img_src = img_dsc->data;

    uint16_t img_w = img_dsc->header.w;
    uint16_t img_h = img_dsc->header.h;
    int32_t clip_w = lv_area_get_width(draw_unit->base_unit.clip_area);
    int32_t clip_h = lv_area_get_height(draw_unit->base_unit.clip_area);
    lv_color_format_t img_format = img_dsc->header.cf;
    uint16_t img_stride = img_dsc->header.stride;
    int32_t buf_size = img_w * img_h * LV_COLOR_DEPTH / 8;
    uint16_t eve_format = ARGB4;

    LV_LOG_INFO("clip_area: x1: %d. y1: %d, x2: %d, y2: %d\n", draw_unit->base_unit.clip_area->x1, draw_unit->base_unit.clip_area->y1,
        draw_unit->base_unit.clip_area->x2, draw_unit->base_unit.clip_area->y2);
    LV_LOG_INFO("coords: cx1: %d, cy1: %d, cx2: %d, cy2: %d\n", coords->x1, coords->y1, coords->x2, coords->y2);
    LV_LOG_INFO("pivot.x %d, y %d, rotation %d, scale_x %d, scale_y %d, tile %d\n", draw_dsc->pivot.x, draw_dsc->pivot.y, draw_dsc->rotation, draw_dsc->scale_x, draw_dsc->scale_y, draw_dsc->tile);
    LV_LOG_INFO("img w %d, h %d, r %d, g %d, b %d, a %d\n", img_w, img_h, draw_dsc->recolor.red, draw_dsc->recolor.green, draw_dsc->recolor.blue, draw_dsc->recolor_opa);

    if (img_stride == 0) {
        img_stride = img_w * lv_color_format_get_size(img_format);
    }

    uint32_t img_eveId = find_ramg_image(img_src);
    if (img_eveId == NOT_FOUND_BLOCK)
    { /* New image to load  */

        uint32_t free_ramg_block = next_free_ramg_block(TYPE_IMAGE); // index
        uint32_t start_addr_ramg = get_ramg_ptr();
        uint8_t *buffer_converted = NULL;
        LV_ATTRIBUTE_MEM_ALIGN uint8_t* temp_buff = lv_malloc_zeroed(buf_size);

        switch (img_format)
        {
        case LV_COLOR_FORMAT_L8:
            buffer_converted = (uint8_t *)img_src;
            break;
        case LV_COLOR_FORMAT_RGB565:
            buffer_converted = (uint8_t *)img_src;
            break;
        case LV_COLOR_FORMAT_RGB565A8:
            convert_RGB565A8_to_ARGB4(img_src, temp_buff, img_w, img_h);
            buffer_converted = temp_buff;
            break;
        case LV_COLOR_FORMAT_ARGB8888:
            convert_ARGB8888_to_ARGB4(img_src, temp_buff, img_w, img_h);
            buffer_converted = temp_buff;
            break;
        default:
            break;
        }
        lv_free(temp_buff);
        load_buf_to_ramg(start_addr_ramg, buffer_converted, (uint32_t)buf_size);

        /* Save RAM_G Memory Block ID info */
        update_ramg_block(free_ramg_block, (uint8_t *)img_src, start_addr_ramg, buf_size);

        img_eveId = find_ramg_image(img_src);
        tbl[handler++] = img_eveId;
        if (handler > 15)
        {
            LV_LOG_WARN("Too many different bitmaps\n");
            handler = 0;
        }
    }

	EVE_CoDl_saveContext(s_pHalContext);

    if (draw_dsc->rotation == 0 && draw_dsc->scale_x == LV_SCALE_NONE && draw_dsc->scale_y == LV_SCALE_NONE)
    {
        EVE_CoDl_scissorXY(s_pHalContext, draw_unit->base_unit.clip_area->x1, draw_unit->base_unit.clip_area->y1);
        EVE_CoDl_scissorSize(s_pHalContext, clip_w, clip_h);
    }

    if(draw_dsc->recolor_opa > LV_OPA_MIN) {
        EVE_CoDl_colorA(s_pHalContext, draw_dsc->recolor_opa);
        EVE_CoDl_colorRgb(s_pHalContext, draw_dsc->recolor.red, draw_dsc->recolor.green, draw_dsc->recolor.blue);
    }

    uint8_t i = 0;
    for (i = 0; i <= handler; i++)
    {
        if (tbl[i] == img_eveId)
            break;
    }
    EVE_CoDl_bitmapHandle(s_pHalContext, i);

    uint32_t img_addr = get_bitmap_addr(img_eveId);

    switch (img_format)
    {
    case LV_COLOR_FORMAT_L8:
        eve_format = L8;
        break;
    case LV_COLOR_FORMAT_RGB565:
        eve_format = RGB565;
        break;
    case LV_COLOR_FORMAT_RGB565A8:
    case LV_COLOR_FORMAT_ARGB8888:
        eve_format = ARGB4;
        break;
    default:
        break;
    }
#if defined(FT81X_ENABLE) || defined(BT88X_ENABLE)
    EVE_CoCmd_setBitmap(s_pHalContext, img_addr, eve_format, img_w, img_h);
#else
    EVE_CoDl_bitmapSource(s_pHalContext, img_addr);
    EVE_CoDl_bitmapLayout(s_pHalContext, eve_format, img_stride, img_h);
    EVE_CoDl_bitmapSize(s_pHalContext, NEAREST, BORDER, BORDER, img_w, img_h);
#endif

    EVE_CoDl_begin(s_pHalContext, BITMAPS);
    if(draw_dsc->rotation || draw_dsc->scale_x != LV_SCALE_NONE || draw_dsc->scale_y != LV_SCALE_NONE) {
        EVE_CoCmd_loadIdentity(s_pHalContext);

        EVE_CoCmd_translate(s_pHalContext, F16(draw_dsc->pivot.x), F16(draw_dsc->pivot.y));
        if (draw_dsc->rotation != 0) {
            /*Image Rotate*/
            EVE_CoCmd_rotate(s_pHalContext, DEGREES(draw_dsc->rotation));
        }
        if(draw_dsc->scale_x != LV_SCALE_NONE || draw_dsc->scale_y != LV_SCALE_NONE) {
            /*Image Scale*/
            EVE_CoCmd_scale(s_pHalContext, F16_SCALE_DIV_256(draw_dsc->scale_x), F16_SCALE_DIV_256(draw_dsc->scale_y));
        }
        
        EVE_CoCmd_translate(s_pHalContext, -F16(draw_dsc->pivot.x), -F16(draw_dsc->pivot.y));
        EVE_CoCmd_setMatrix(s_pHalContext);
    }
    if (draw_dsc->tile)
    {
        if (clip_h > img_h)
        {
            int32_t y = coords->y1;
            while (y > draw_unit->base_unit.clip_area->y1)
            {
                y -= img_h;
            }
            while (y < draw_unit->base_unit.clip_area->y2)
            {
                int32_t x = coords->x1;
                if (clip_w > img_w)
                {
                    while (x > draw_unit->base_unit.clip_area->x1)
                    {
                        x -= img_w;
                    }
                    while (x < draw_unit->base_unit.clip_area->x2)
                    {
                        //LV_LOG_WARN("clip_w %d, clip_h %d, img_w %d, img_h %d, x %d, y %d\n", clip_w, clip_h, img_w, img_h, x, y);
                        EVE_CoDl_vertex2f(s_pHalContext, x, y);
                        x += img_w;
                    }
                }
                y += img_h;
            }
        }
    }
    else
    {
        EVE_CoDl_vertex2f(s_pHalContext, coords->x1, coords->y1);
    }
    EVE_CoDl_end(s_pHalContext);
	EVE_CoCmd_loadIdentity(s_pHalContext);
	EVE_CoCmd_setMatrix(s_pHalContext);
    EVE_CoDl_restoreContext(s_pHalContext);

}





/**********************
 *   STATIC FUNCTIONS
 **********************/

static void convert_RGB565A8_to_ARGB4(const uint8_t * src, uint8_t * dst, uint16_t width, uint16_t height)
{
    uint32_t pixel_count = (uint32_t)width * height;
    uint16_t *src_rgb565 = (uint16_t *)src;
    uint8_t *src_alpha = (uint8_t *)src + 2 * pixel_count;

    for(uint32_t i = 0; i < pixel_count; i++) {
        uint16_t rgb565 = src_rgb565[i];
        uint8_t alpha = src_alpha[i];
        uint8_t r5 = (rgb565 >> 11) & 0x1F;
        uint8_t g6 = (rgb565 >> 5) & 0x3F;
        uint8_t b5 = rgb565 & 0x1F;
        uint8_t r4 = r5 >> 1;
        uint8_t g4 = g6 >> 2;
        uint8_t b4 = b5 >> 1;
        uint8_t a4 = alpha >> 4;
        uint16_t argb4444 = (a4 << 12) | (r4 << 8) | (g4 << 4) | b4;

        dst[2 * i] = argb4444 & 0xFF;
        dst[2 * i + 1] = (argb4444 >> 8) & 0xFF;
    }
}

static void convert_RGB565A8_to_ARGB1555(const uint8_t * src, uint8_t * dst, uint16_t width, uint16_t height)
{
    int pixel_count = width * height;
    uint16_t * src_rgb565 = (uint16_t *) src;
    uint8_t * src_alpha = (uint8_t *)src + 2 * pixel_count;

    for(int i = 0; i < pixel_count; i++) {

        uint16_t rgb565 = src_rgb565[i];
        uint8_t alpha = src_alpha[i];
        uint8_t r5 = (rgb565 >> 11) & 0x1F;
        uint8_t g6 = (rgb565 >> 5) & 0x3F;
        uint8_t b5 = rgb565 & 0x1F;
        uint8_t a1 = alpha >= 128 ? 1 : 0;

        uint16_t argb1555 = (a1 << 15) | (r5 << 10) | ((g6 >> 1) << 5) | b5;

        dst[2 * i] = argb1555 & 0xFF;
        dst[2 * i + 1] = (argb1555 >> 8) & 0xFF;

    }
}

static void convert_ARGB8888_to_ARGB4(const uint8_t * src, uint8_t * dst, uint16_t width, uint16_t height)
{
    uint32_t pixel_count = (uint32_t)width * height;

    for(uint32_t i = 0; i < pixel_count; i++) {
        uint8_t blue = src[4 * i];
        uint8_t green = src[4 * i + 1];
        uint8_t red = src[4 * i + 2];
        uint8_t alpha = src[4 * i + 3];
        uint8_t r4 = red >> 4;
        uint8_t g4 = green >> 4;
        uint8_t b4 = blue >> 4;
        uint8_t a4 = alpha >> 4;
        uint16_t argb4444 = (a4 << 12) | (r4 << 8) | (g4 << 4) | b4;

        dst[2 * i] = argb4444 & 0xFF;
        dst[2 * i + 1] = (argb4444 >> 8) & 0xFF;
    }
}

#endif /*LV_USE_DRAW_EVE*/
