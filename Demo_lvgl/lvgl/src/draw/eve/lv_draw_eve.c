/**
 * @file lv_draw_eve.c
 *
 * @auther Bridgetek
 * @date 2024
 */
#if LV_USE_DRAW_EVE
/*********************
 *      INCLUDES
 *********************/
#include "../../core/lv_refr.h"
#include "../../display/lv_display_private.h"
#include "../../stdlib/lv_string.h"
#include "eve_ram_g.h"
#include "lv_draw_eve.h"
/*********************
 *      DEFINES
 *********************/

#define DRAW_UNIT_ID_EVE     18 /*18?*/
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void eve_execute_drawing(lv_draw_eve_unit_t * u);

static int32_t eve_dispatch(lv_draw_eve_unit_t * draw_unit, lv_layer_t * layer);

static int32_t eve_evaluate(lv_draw_eve_unit_t * draw_unit, lv_draw_task_t * task);

/**********************
 *  GLOBAL PROTOTYPES
 **********************/


/**********************
 *  STATIC VARIABLES
 **********************/


/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_draw_eve_init(void)
{
    init_eve_ramg();

    lv_draw_eve_unit_t * draw_eve_unit = lv_draw_create_unit(sizeof(lv_draw_eve_unit_t));
    draw_eve_unit->base_unit.dispatch_cb = eve_dispatch;
    draw_eve_unit->base_unit.evaluate_cb = eve_evaluate;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static int32_t eve_dispatch(lv_draw_eve_unit_t * draw_unit, lv_layer_t * layer)
{
    lv_draw_eve_unit_t * draw_eve_unit = (lv_draw_eve_unit_t *) draw_unit;

    /*Return immediately if it's busy with draw task*/
    if(draw_eve_unit->task_act) return 0;

    lv_draw_task_t * t = NULL;
    t = lv_draw_get_next_available_task(layer, NULL, DRAW_UNIT_ID_EVE);
    if(t == NULL) return -1;


    t->state = LV_DRAW_TASK_STATE_IN_PROGRESS;
    draw_eve_unit->base_unit.target_layer = layer;
    draw_eve_unit->base_unit.clip_area = &t->clip_area;
    draw_eve_unit->task_act = t;

    eve_execute_drawing(draw_eve_unit);

    draw_eve_unit->task_act->state = LV_DRAW_TASK_STATE_READY;
    draw_eve_unit->task_act = NULL;

    /*The draw unit is free now. Request a new dispatching as it can get a new task*/
    lv_draw_dispatch_request();

    return 1;
}

static int32_t eve_evaluate(lv_draw_eve_unit_t * draw_unit, lv_draw_task_t * task)
{
    LV_UNUSED(draw_unit);

    if(((lv_draw_dsc_base_t *)task->draw_dsc)->user_data == NULL) {
        task->preference_score = 0;
        task->preferred_draw_unit_id = DRAW_UNIT_ID_EVE;
    }
    return 0;
}

static void eve_execute_drawing(lv_draw_eve_unit_t * u)
{
    lv_draw_task_t * t = u->task_act;

    switch(t->type) {
        case LV_DRAW_TASK_TYPE_FILL:
            LV_LOG_INFO("LV_DRAW_TASK_TYPE_FILL\n");
            lv_draw_eve_fill(u, t->draw_dsc, &t->area);
            break;
        case LV_DRAW_TASK_TYPE_BORDER:
            LV_LOG_INFO("LV_DRAW_TASK_TYPE_BORDER\n");
            lv_draw_eve_border(u, t->draw_dsc, &t->area);
            break;
        case LV_DRAW_TASK_TYPE_BOX_SHADOW:
            LV_LOG_INFO("LV_DRAW_TASK_TYPE_BOX_SHADOW\n");
            lv_draw_eve_box_shadow(u, t->draw_dsc, &t->area);
            break;
        case LV_DRAW_TASK_TYPE_LABEL:
            LV_LOG_INFO("LV_DRAW_TASK_TYPE_LABEL\n");
            lv_draw_eve_label(u, t->draw_dsc, &t->area);
            break;
        case LV_DRAW_TASK_TYPE_IMAGE:
            LV_LOG_INFO("LV_DRAW_TASK_TYPE_IMAGE\n");
            lv_draw_eve_image(u, t->draw_dsc, &t->area);
            break;
        case LV_DRAW_TASK_TYPE_LAYER:
            LV_LOG_INFO("LV_DRAW_TASK_TYPE_LAYER\n");
            lv_draw_eve_layer(u, t->draw_dsc, &t->area);
            break;
        case LV_DRAW_TASK_TYPE_LINE:
            LV_LOG_INFO("LV_DRAW_TASK_TYPE_LINE\n");
            lv_draw_eve_line(u, t->draw_dsc);
            break;
        case LV_DRAW_TASK_TYPE_ARC:
            LV_LOG_INFO("LV_DRAW_TASK_TYPE_ARC\n");
            lv_draw_eve_arc(u, t->draw_dsc, &t->area);
            break;
        case LV_DRAW_TASK_TYPE_TRIANGLE:
            LV_LOG_INFO("LV_DRAW_TASK_TYPE_TRIANGLE\n");
            lv_draw_eve_triangle(u, t->draw_dsc);
            break;
        case LV_DRAW_TASK_TYPE_MASK_RECTANGLE:
            LV_LOG_INFO("LV_DRAW_TASK_TYPE_MASK_RECTANGLE\n");
            //lv_draw_eve_mask_rect(u, t->draw_dsc, &t->area);
            break;
        case LV_DRAW_TASK_TYPE_MASK_BITMAP:
            LV_LOG_INFO("LV_DRAW_TASK_TYPE_MASK_BITMAP\n");
            //lv_draw_eve_mask_bitmap(u, t->draw_dsc, &t->area);
            break;
        case LV_DRAW_TASK_TYPE_VECTOR:
            LV_LOG_INFO("LV_DRAW_TASK_TYPE_VECTOR\n");
            break;
        default:
            break;
    }
}



#endif /*LV_USE_DRAW_EVE*/
