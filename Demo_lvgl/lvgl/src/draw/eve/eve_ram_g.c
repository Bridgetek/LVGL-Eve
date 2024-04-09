/**
 * @file eve_ram_g.c
 * keep a memory map of font and bitmap loaded to RAM_G
 *
 * @auther Bridgetek
 * @date 2024
 */
/*********************
 *      INCLUDES
 *********************/
#if LV_USE_DRAW_EVE
#include "eve_ram_g.h"

#include "Common.h"


/**********************
 * STATIC PROTOTYPES
 **********************/

/***********************
 * GLOBAL VARIABLES
 ***********************/
extern Gpu_Hal_Context_t *s_pHalContext;

/**********************
 *  STATIC VARIABLES
 **********************/

/**
 *  Memory blocks are organized by type, with 8 (MAX_FONT 8) spaces for Fonts and 32 (MAX_IMAGE 32 )spaces for images (these values can be modified as needed).
 *  This structure is designed to speed up the search process, eliminating the need to traverse the entire array of blocks.
 */
static ramg_mem_block blocks[MAX_BLOCKS];
static uint32_t ramGptr = RAM_G;


/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void init_eve_ramg()
{
    for (int i = 0; i < MAX_BLOCKS; i++)
    {
        blocks[i].address = 0;
        blocks[i].loaded = false;
        blocks[i].id = 0;
        blocks[i].size = 0;
        blocks[i].source = NULL;
    }
}

uint32_t get_bitmap_addr(uint8_t id)
{
    return blocks[id].address;
}

void update_ramg_ptr(uint32_t size)
{
    ramGptr += size;
    uint32_t alignment = 4;
    ramGptr = (ramGptr + alignment - 1) & ~(alignment - 1); /*RamG Aligned*/
}

uint32_t get_ramg_ptr(void)
{
    return ramGptr;
}

uint32_t next_free_ramg_block(DataType data)
{
    uint32_t start = 0;
    uint32_t end = 0;

    if(data == TYPE_IMAGE) {
        start = IMAGE_BLOCK_START;
        end =  MAX_IMAGE;
    }
    else if(data == TYPE_FONT) {
        start = FONT_BLOCK_START;
        end =  MAX_FONT;
    }

    for(uint32_t i = start; i < end; i++) {
        if(blocks[i].loaded == false) {
            blocks[i].loaded = true;
            return i;
        }
    }

    return NOT_FOUND_BLOCK;
}

void set_source_ramg_block(uint8_t Id, const uint8_t * src)
{
    if(Id < MAX_BLOCKS) {
        blocks[Id].source = (uint8_t *)src;
    }
}

void set_addr_ramg_block(uint8_t Id, uint32_t addr)
{
    blocks[Id].address = addr;
}

void set_size_ramg_block(uint8_t Id, uint32_t sz)
{
    blocks[Id].size = sz;
}

void set_state_ramg_block(uint8_t Id, bool state)
{
    blocks[Id].loaded = state;
}

bool update_ramg_block(uint8_t id, uint8_t * src, uint32_t addr, uint32_t sz)
{
    blocks[id].source = src;
    blocks[id].address = addr;
    blocks[id].size = sz;
    blocks[id].loaded = true;
    blocks[id].id = id;
    update_ramg_ptr(sz);

    if(ramGptr > RAM_G_SIZE) {
        return true;
    }
    else {
        return false;
    }
}

uint32_t find_ramg_image(const uint8_t * imageSource)
{

    for(uint32_t i = IMAGE_BLOCK_START; i < MAX_IMAGE; i++) {
        if(blocks[i].source == imageSource) {
            return i;
        }
    }

    return NOT_FOUND_BLOCK;
}

uint32_t find_ramg_font(const uint8_t * fontSource)
{

    for(uint32_t i = FONT_BLOCK_START; i < MAX_FONT; i++) {
        if(blocks[i].source == fontSource) {
            return i;
        }
    }

    return NOT_FOUND_BLOCK;
}

void load_buf_to_ramg(uint32_t start_addr_ramg, uint8_t *buffer, uint32_t img_size)
{
    uint32_t chunksize = 16 * 1024;
    uint32_t totalbufflen = 64 * 1024;
    uint32_t currreadlen = 0;
    uint32_t wrptr = start_addr_ramg;

    int offset = 0;
    while (img_size > 0)
    {
        currreadlen = img_size;
        if (currreadlen > chunksize)
        {
            currreadlen = chunksize;
        }

        EVE_Hal_wrMem(s_pHalContext, wrptr, &buffer[offset], currreadlen);
        offset += currreadlen;
        wrptr += currreadlen;
        wrptr = wrptr % (start_addr_ramg + totalbufflen);
        img_size -= currreadlen;

        // if the file is sent over and there is one more chunk size free space.
        if (img_size == 0)
        {
            // Clear the chunksize
            EVE_CoCmd_memSet(s_pHalContext, wrptr, 0, chunksize);
        }
    }
    EVE_Cmd_waitFlush(s_pHalContext);
}


#endif/*LV_USE_EVE_DRAW*/

