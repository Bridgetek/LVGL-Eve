/**
 * @file eve_ram_g.h
 *
 * @auther Bridgetek
 * @date 2024
 */

#ifndef EVE_RAM_G_H_
#define EVE_RAM_G_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#if LV_USE_DRAW_EVE
#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

/*********************
 *      DEFINES
 *********************/

#define MAX_MEMORY RAM_G_SIZE  /* Defines the size of memory as 1MB */

#define FONT_BLOCK_START 0      /* Starting index for font blocks in the memory */
#define MAX_FONT 8              /* Maximum number of font blocks */

#define IMAGE_BLOCK_START MAX_FONT  /* Starting index for image blocks in the memory.*/
#define MAX_IMAGE 32                 /* Maximum number of image blocks.*/

#define MAX_BLOCKS 256               /* Total number of blocks in the memory.*/
#define NOT_FOUND_BLOCK 0xFFAAFFAA   /* A constant to represent 'block not found'.*/

/**********************
 *      TYPEDEFS
 **********************/

/* Structure representing a block in the memory.*/
typedef struct {
    uint32_t address; /* Address of the block in memory.*/
    uint32_t size;    /* Size of the block.*/
    uint8_t id;       /* Identifier for the block.*/
    uint8_t * source; /* Pointer to the source of the data stored in the block.*/
    bool loaded;      /* Flag to indicate if the block is loaded.*/
} ramg_mem_block;


/* Enumeration to define the types of data.*/
typedef enum {
    TYPE_IMAGE = 0,
    TYPE_FONT = 1,
    OTHER
} DataType;



/**********************
 * GLOBAL PROTOTYPES
 **********************/

/* Init Memory blocks */
extern void init_eve_ramg();

/* Function to update the RAM_G pointer with the given size.*/
extern void update_ramg_ptr(uint32_t size);

/* Function to get the current RAM_G pointer value. */
extern uint32_t get_ramg_ptr(void);

/* Searches for the next free memory block */
extern uint32_t next_free_ramg_block(DataType data);

/* This function checks if the image has already been loaded into ram_g. If it hasn't been loaded, it returns 'NOT FOUND',
 * otherwise it returns the ID of the block where it is located."
 */
extern uint32_t find_ramg_image(const uint8_t * imageSource);

/* "This function checks if the font has already been loaded into ram_g. If it hasn't been loaded, it returns 'NOT FOUND',
 * otherwise it returns the ID of the block where it is located."
 */
extern uint32_t find_ramg_font(const uint8_t * fontSource);

extern uint32_t get_bitmap_addr(uint8_t id);

extern void set_source_ramg_block(uint8_t Id, const  uint8_t * src);

extern void set_addr_ramg_block(uint8_t Id, uint32_t addr);

extern void set_state_ramg_block(uint8_t Id, bool state);

extern void set_size_ramg_block(uint8_t Id, uint32_t sz);

extern bool update_ramg_block(uint8_t id, uint8_t * src, uint32_t addr, uint32_t sz);

extern void load_buf_to_ramg(uint32_t start_addr_ramg, uint8_t *buffer, uint32_t img_size);


#endif/*LV_USE_DRAW_EVE*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* EVE_RAM_G_H_ */
