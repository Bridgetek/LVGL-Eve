/**
 * @file lv_draw_eve_letter.c
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
#include "src/font/lv_font_fmt_txt.h"
#include "src/misc/lv_utils.h"
#include "stdlib.h"

/*********************
 *      DEFINES
 *********************/

#if LV_FONT_SIMSUN_16_CJK
    #define MAX_GLYPH_DSC  1414
    #define _MAX_FONT MAX_FONT / 2 /* Reduce the number of allowed fonts to 4 to save memory */
#elif LV_FONT_DEJAVU_16_PERSIAN_HEBREW
    #define MAX_GLYPH_DSC  598
    #define _MAX_FONT MAX_FONT / 2 /* Reduce the number of allowed fonts to 4 to save memory */

#else
    #define MAX_GLYPH_DSC  160
    #define _MAX_FONT MAX_FONT /* Case for montserrat font */
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void lv_draw_eve_letter_cb(lv_draw_unit_t * draw_unit, lv_draw_glyph_dsc_t * glyph_draw_dsc,
                                  lv_draw_fill_dsc_t * fill_draw_dsc, const lv_area_t * fill_area);
static void bitmap_to_evenWidth(uint32_t addr, const uint8_t * img_in, uint8_t * img_out, uint16_t width,
                                uint16_t height);
static int32_t _unicode_list_compare(const void * ref, const void * element);
static uint32_t _get_glyph_dsc_id(const lv_font_t * font, uint32_t letter);
static uint32_t eve_lv_font_to_ramg(const lv_font_t * font_p, uint8_t font_eveId, uint32_t ad);

/**********************
 *  STATIC VARIABLES
 **********************/

static uint32_t addr_font[_MAX_FONT][MAX_GLYPH_DSC]; /* Save the current RAM_G font address */
static const lv_font_t * font_static;

static lv_draw_eve_unit_t * unit = NULL;

/***********************
 * GLOBAL VARIABLES
 ***********************/
extern Gpu_Hal_Context_t *s_pHalContext;

/**********************
 *      MACROS
 **********************/

#define get_nibble_1(w) ((uint8_t) ((w) >> 4))
#define get_nibble_2(w) ((uint8_t) ((w) & 0xf))


/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_draw_eve_label(lv_draw_eve_unit_t * draw_unit, const lv_draw_label_dsc_t * dsc, const lv_area_t * coords)
{
    if(dsc->opa <= LV_OPA_MIN)
        return;

    LV_LOG_INFO("clip_area: x1: %d. y1: %d, x2: %d, y2: %d\n", draw_unit->base_unit.clip_area->x1, draw_unit->base_unit.clip_area->y1,
        draw_unit->base_unit.clip_area->x2, draw_unit->base_unit.clip_area->y2);
    LV_LOG_INFO("coords: cx1: %d, cy1: %d, cx2: %d, cy2: %d\n", coords->x1, coords->y1, coords->x2, coords->y2);

    EVE_CoDl_scissorXY(s_pHalContext, draw_unit->base_unit.clip_area->x1, draw_unit->base_unit.clip_area->y1);
    EVE_CoDl_scissorSize(s_pHalContext, draw_unit->base_unit.clip_area->x2 - draw_unit->base_unit.clip_area->x1,
        draw_unit->base_unit.clip_area->y2 - draw_unit->base_unit.clip_area->y1);
    EVE_CoDl_saveContext(s_pHalContext);
    EVE_CoDl_begin(s_pHalContext, BITMAPS);
    //LV_LOG_INFO("%s, r %d, g %d, b %d, font %d\r\n ", dsc->text, dsc->color.red, dsc->color.green, dsc->color.blue, dsc->font);
    font_static = dsc->font;
    lv_draw_label_iterate_characters(&draw_unit->base_unit, dsc, coords, lv_draw_eve_letter_cb);
    EVE_CoDl_end(s_pHalContext);
    EVE_CoDl_restoreContext(s_pHalContext);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static int32_t _unicode_list_compare(const void * ref, const void * element)
{
    return ((int32_t)(*(uint16_t *)ref)) - ((int32_t)(*(uint16_t *)element));
}

/* same function duplicated from 
 * static uint32_t get_glyph_dsc_id(const lv_font_t * font, uint32_t letter)
 * in lv_font_fmt_txt.c
 */
static uint32_t _get_glyph_dsc_id(const lv_font_t * font, uint32_t letter)
{
    if(letter == '\0') return 0;

    lv_font_fmt_txt_dsc_t * fdsc = (lv_font_fmt_txt_dsc_t *)font->dsc;

    uint16_t i;
    for(i = 0; i < fdsc->cmap_num; i++) {

        /*Relative code point*/
        uint32_t rcp = letter - fdsc->cmaps[i].range_start;
        if(rcp >= fdsc->cmaps[i].range_length) continue;
        uint32_t glyph_id = 0;
        if(fdsc->cmaps[i].type == LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY) {
            glyph_id = fdsc->cmaps[i].glyph_id_start + rcp;
        }
        else if(fdsc->cmaps[i].type == LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL) {
            const uint8_t * gid_ofs_8 = fdsc->cmaps[i].glyph_id_ofs_list;
            glyph_id = fdsc->cmaps[i].glyph_id_start + gid_ofs_8[rcp];
        }
        else if(fdsc->cmaps[i].type == LV_FONT_FMT_TXT_CMAP_SPARSE_TINY) {
            uint16_t key = rcp;
            uint16_t * p = _lv_utils_bsearch(&key, fdsc->cmaps[i].unicode_list, fdsc->cmaps[i].list_length,
                                             sizeof(fdsc->cmaps[i].unicode_list[0]), _unicode_list_compare);

            if(p) {
                lv_uintptr_t ofs = p - fdsc->cmaps[i].unicode_list;
                glyph_id = fdsc->cmaps[i].glyph_id_start + ofs;
            }
        }
        else if(fdsc->cmaps[i].type == LV_FONT_FMT_TXT_CMAP_SPARSE_FULL) {
            uint16_t key = rcp;
            uint16_t * p = _lv_utils_bsearch(&key, fdsc->cmaps[i].unicode_list, fdsc->cmaps[i].list_length,
                                             sizeof(fdsc->cmaps[i].unicode_list[0]), _unicode_list_compare);

            if(p) {
                lv_uintptr_t ofs = p - fdsc->cmaps[i].unicode_list;
                const uint16_t * gid_ofs_16 = fdsc->cmaps[i].glyph_id_ofs_list;
                glyph_id = fdsc->cmaps[i].glyph_id_start + gid_ofs_16[ofs];
            }
        }

        return glyph_id;
    }

    return 0;

}



static void lv_draw_eve_letter_cb(lv_draw_unit_t * draw_unit, lv_draw_glyph_dsc_t * glyph_draw_dsc,
                                  lv_draw_fill_dsc_t * fill_draw_dsc, const lv_area_t * fill_area)
{
    if(fill_draw_dsc && fill_area) {
        /* draw UNDERLINE and STRIKETHROUGH */
        EVE_draw_rect_simple(fill_area->x1, fill_area->y1, fill_area->x2, fill_area->y2, 0);
    }

    if(glyph_draw_dsc == NULL)
        return;  //Important

    //load font bitmap to RAM_G
    const lv_font_t *font_in_use = font_static;
    lv_font_fmt_txt_dsc_t *font_dsc = (lv_font_fmt_txt_dsc_t *)font_in_use->dsc;
    uint32_t gid = _get_glyph_dsc_id(font_in_use, (int32_t)draw_unit->target_layer->user_data);
    if (!gid)
        return;

    uint32_t font_eveId = find_ramg_font((const uint8_t *)font_in_use);
    if (font_eveId == NOT_FOUND_BLOCK)
    { /* If the font is not yet loaded in RAM_G, load it */
        uint32_t free_ramg_block = next_free_ramg_block(TYPE_FONT);
        uint32_t start_addr_ramg = get_ramg_ptr();

        uint32_t total_font_size = eve_lv_font_to_ramg(font_in_use, free_ramg_block,
            start_addr_ramg); /* load font to ramG (The block Id is updated in this function) */

        update_ramg_block(free_ramg_block, (uint8_t *)font_in_use, start_addr_ramg, total_font_size);
        font_eveId = free_ramg_block;
    }

    uint8_t bpp = 0;
    switch (glyph_draw_dsc->format)
    {
        case 1 :
            bpp = L1;
            break;
        case 4 :
            bpp = L4;
            break;
        case 8 :
            bpp = L8;
            break;
        default :
            break;
    }

    EVE_CoDl_colorA(s_pHalContext, glyph_draw_dsc->opa);
    EVE_CoDl_colorRgb(s_pHalContext, glyph_draw_dsc->color.red, glyph_draw_dsc->color.green, glyph_draw_dsc->color.blue);
    //LV_LOG_INFO("bpp %d, box_w %d, box_h %d, color r %d, g %d, b %d\n", bpp, glyph_draw_dsc->g->box_w, glyph_draw_dsc->g->box_h, glyph_draw_dsc->color.red, glyph_draw_dsc->color.green, glyph_draw_dsc->color.blue);
    //LV_LOG_INFO("coords: cx1: %d, cy1: %d, cx2: %d, cy2: %d\n", glyph_draw_dsc->letter_coords->x1, glyph_draw_dsc->letter_coords->y1, glyph_draw_dsc->letter_coords->x2, glyph_draw_dsc->letter_coords->y2);
#if defined(FT81X_ENABLE) || defined(BT88X_ENABLE)
    EVE_CoCmd_setBitmap(s_pHalContext, addr_font[font_eveId][gid], bpp, glyph_draw_dsc->g->box_w, glyph_draw_dsc->g->box_h);
#else
    EVE_CoDl_bitmapSource(s_pHalContext, addr_font[font_eveId][gid]);
    EVE_CoDl_bitmapLayout(s_pHalContext, bpp, glyph_draw_dsc->g->box_w, glyph_draw_dsc->g->box_h);
    EVE_CoDl_bitmapSize(s_pHalContext, NEAREST, BORDER, BORDER, glyph_draw_dsc->g->box_w, glyph_draw_dsc->g->box_h);
#endif


    EVE_CoDl_vertex2f(s_pHalContext, glyph_draw_dsc->letter_coords->x1, glyph_draw_dsc->letter_coords->y1);
}


/**********************
 *   STATIC FUNCTIONS
 **********************/

static uint32_t eve_lv_font_to_ramg(const lv_font_t * font_p, uint8_t font_eveId, uint32_t addr)
{
    const uint8_t * map_p;
    lv_font_fmt_txt_dsc_t * font_dsc = (lv_font_fmt_txt_dsc_t *) font_p->dsc;
    uint8_t * temp_buff = lv_malloc(font_p->line_height * font_p->line_height * font_dsc->bpp); /*Extra mem*/
    uint32_t cmap_max = font_dsc->cmap_num;
    uint32_t range_end = 0;
    uint32_t range_start = 0;
    uint32_t cmap;

    for(cmap = 0; cmap < cmap_max; ++cmap) {

        range_start = font_dsc->cmaps[cmap].glyph_id_start;
        range_end = font_dsc->cmaps[cmap].range_length + range_start;

        if(font_dsc->cmaps[cmap].unicode_list || font_dsc->cmaps[cmap].glyph_id_ofs_list) {
            range_end = font_dsc->cmaps[cmap].list_length + range_start;

        }

        uint32_t i;
        for(i = range_start; i < range_end; ++i) { /*Iterate through each character range*/

            uint32_t idx = font_dsc->glyph_dsc[i].bitmap_index;

            map_p = &font_dsc->glyph_bitmap[idx];

            uint8_t g_box_w = font_dsc->glyph_dsc[i].box_w;
            uint8_t g_box_h = font_dsc->glyph_dsc[i].box_h;

            if(g_box_w != 0 && g_box_h != 0) {

                //LV_LOG_INFO("font_eveId %d, id %d, addr %x", font_eveId, i, addr);
                addr_font[font_eveId][i] = addr; /*Store the address of the glyph in the address table*/

                uint32_t buffer_size; /*Calculate buffer size based on glyph dimensions*/
                if(g_box_w % 2 != 0) {
                    bitmap_to_evenWidth(0, map_p, temp_buff, g_box_w, g_box_h); /*Adjust bitmap width to even width if necessary*/
                    buffer_size = ((g_box_w + 1) * g_box_h) / 2;

                    load_buf_to_ramg(addr, temp_buff, buffer_size);
                }
                else {
                    buffer_size = (g_box_w * g_box_h) / 2; /*Calculate buffer size based on glyph dimensions*/
                    load_buf_to_ramg(addr, map_p, buffer_size); /*Write glyph bitmap to EVE memory*/
                }

                addr += buffer_size;

            }
        }
    }
    lv_free(temp_buff);

    return addr;

}

static void bitmap_to_evenWidth(uint32_t addr, const uint8_t * img_in, uint8_t * img_out, uint16_t width,
                                uint16_t height)
{
    uint32_t addr_it = addr;
    uint8_t nibble_1;
    uint8_t nibble_2;
    uint8_t key = 0;
    uint16_t j = 0;

    /* Iterate through each row of the bitmap*/
    for(int i = 0; i <= height; i++) {
        /* Iterate through each byte of the row*/
        for(int var = 0; var < (width / 2); ++var) {
            /*Get the two nibbles from the current byte*/
            if(key == 0) {
                nibble_1 = get_nibble_1(img_in[addr_it]);
                nibble_2 = get_nibble_2(img_in[addr_it]);
            }
            if(key == 1) {
                nibble_1 = get_nibble_2(img_in[addr_it - 1]);
                nibble_2 = get_nibble_1(img_in[addr_it]);
            }

            /*Combine the nibbles and assign the result to the output byte*/
            img_out[j] = (nibble_1 << 4) | nibble_2;

            addr_it++;
            j++;
        }

        /*If the width is odd, process the last remaining nibble*/
        if(width % 2 >= 1) {
            img_out[j] =
                (key == 0) ?
                (get_nibble_1(img_in[addr_it])) << 4 | 0x0 : (get_nibble_2(img_in[addr_it - 1])) << 4 | 0x0;
            key = (key == 0) ? 1 : 0;
            addr_it += (key == 1) ? 1 : 0;
            j++;
        }
    }
}


#endif /*LV_USE_DRAW_EVE*/

