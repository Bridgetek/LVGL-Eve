/**
 * @file lv_conf.h
 * Configuration file updated for LVGL 9.2.2
 */

#ifndef LV_CONF_H
#define LV_CONF_H

/*====================
   COLOR SETTINGS
 *====================*/
#define LV_COLOR_DEPTH 16

/*=========================
   STDLIB WRAPPER SETTINGS
 *=========================*/
#define LV_USE_STDLIB_MALLOC LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_STRING LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_SPRINTF LV_STDLIB_BUILTIN
#define LV_MEM_SIZE (96 * 1024U)
/* Use EVE GPU. */
#define LV_USE_DRAW_EVE 1
/*====================
   HAL SETTINGS
 *====================*/
#define LV_DEF_REFR_PERIOD 33
#define LV_DPI_DEF 130
#define LV_USE_OS LV_OS_NONE

/*========================
 * RENDERING CONFIGURATION
 *========================*/
#define LV_DRAW_BUF_STRIDE_ALIGN 1
#define LV_DRAW_BUF_ALIGN 4
#define LV_USE_DRAW_SW 0
#define LV_DRAW_SW_DRAW_UNIT_CNT 0
#define LV_DRAW_SW_COMPLEX 0

/*=======================
 * FEATURE CONFIGURATION
 *=======================*/
#define LV_USE_LOG 1
#define LV_LOG_LEVEL LV_LOG_LEVEL_WARN
#define LV_LOG_PRINTF 1
#define LV_LOG_USE_TIMESTAMP 0
#define LV_LOG_USE_FILE_LINE 1

/*==================
 * FONT USAGE
 *===================*/
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_24 1
#define LV_FONT_MONTSERRAT_16 1
#define LV_FONT_MONTSERRAT_40 1
#define LV_FONT_SIMSUN_16_CJK            0  /*1000 most common CJK radicals*/
#define LV_FONT_DEFAULT &lv_font_montserrat_16

/*==================
 * WIDGETS
 *================*/
#define LV_USE_ANIMIMG 1
#define LV_USE_ARC 1
#define LV_USE_BAR 1
#define LV_USE_BUTTON 1
#define LV_USE_BUTTONMATRIX  1
#define LV_USE_CALENDAR   1
#if LV_USE_CALENDAR
#define LV_CALENDAR_WEEK_STARTS_MONDAY 0
#if LV_CALENDAR_WEEK_STARTS_MONDAY
#define LV_CALENDAR_DEFAULT_DAY_NAMES {"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"}
#else
#define LV_CALENDAR_DEFAULT_DAY_NAMES {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"}
#endif

#define LV_CALENDAR_DEFAULT_MONTH_NAMES {"January", "February", "March",  "April", "May",  "June", "July", "August", "September", "October", "November", "December"}
#define LV_USE_CALENDAR_HEADER_ARROW 1
#define LV_USE_CALENDAR_HEADER_DROPDOWN 1
#define LV_USE_CALENDAR_CHINESE 0
#endif  /*LV_USE_CALENDAR*/
#define LV_USE_CANVAS     1

#define LV_USE_CHART      1

#define LV_USE_CHECKBOX   1

#define LV_USE_DROPDOWN   1   /*Requires: lv_label*/

#define LV_USE_IMAGE      1   /*Requires: lv_label*/

#define LV_USE_IMAGEBUTTON     1

#define LV_USE_KEYBOARD   1
#define LV_USE_LABEL 1
#if LV_USE_LABEL
#define LV_LABEL_TEXT_SELECTION 1 /*Enable selecting text of the label*/
#define LV_LABEL_LONG_TXT_HINT 1  /*Store some extra info in labels to speed up drawing of very long texts*/
#define LV_LABEL_WAIT_CHAR_COUNT 3  /*The count of wait chart*/
#endif

#define LV_USE_LED        1

#define LV_USE_LINE       1

#define LV_USE_LIST       1

#define LV_USE_LOTTIE     0  /*Requires: lv_canvas, thorvg */

#define LV_USE_MENU       1

#define LV_USE_MSGBOX     1

#define LV_USE_ROLLER     1   /*Requires: lv_label*/

#define LV_USE_SCALE      1

#define LV_USE_SLIDER     1   /*Requires: lv_bar*/

#define LV_USE_SPAN       1
#if LV_USE_SPAN
 /*A line text can contain maximum num of span descriptor */
#define LV_SPAN_SNIPPET_STACK_SIZE 64
#endif

#define LV_USE_SPINBOX    1

#define LV_USE_SPINNER    1

#define LV_USE_SWITCH     1

#define LV_USE_TEXTAREA   1   /*Requires: lv_label*/
#if LV_USE_TEXTAREA != 0
#define LV_TEXTAREA_DEF_PWD_SHOW_TIME 1500    /*ms*/
#endif

#define LV_USE_TABLE      1

#define LV_USE_TABVIEW    1

#define LV_USE_TILEVIEW   1

#define LV_USE_WIN        1
/*==================
 * THEMES
 *==================*/
#define LV_USE_THEME_DEFAULT 1
#define LV_THEME_DEFAULT_DARK 0


 /*1: Enable system monitor component*/
#define LV_USE_SYSMON   1
#if LV_USE_SYSMON
    /*Get the idle percentage. E.g. uint32_t my_get_idle(void);*/
#define LV_SYSMON_GET_IDLE lv_timer_get_idle

/*1: Show CPU usage and FPS count
 * Requires `LV_USE_SYSMON = 1`*/
#define LV_USE_PERF_MONITOR 1
#if LV_USE_PERF_MONITOR
#define LV_USE_PERF_MONITOR_POS LV_ALIGN_BOTTOM_RIGHT

 /*0: Displays performance data on the screen, 1: Prints performance data using log.*/
#define LV_USE_PERF_MONITOR_LOG_MODE 0
#endif

/*1: Show the used memory and the memory fragmentation
 * Requires `LV_USE_STDLIB_MALLOC = LV_STDLIB_BUILTIN`
 * Requires `LV_USE_SYSMON = 1`*/
#define LV_USE_MEM_MONITOR 0
#if LV_USE_MEM_MONITOR
#define LV_USE_MEM_MONITOR_POS LV_ALIGN_BOTTOM_LEFT
#endif

#endif /*LV_USE_SYSMON*/

 /*1: Enable the runtime performance profiler*/
#define LV_USE_PROFILER 0
#if LV_USE_PROFILER
    /*1: Enable the built-in profiler*/
#define LV_USE_PROFILER_BUILTIN 1
#if LV_USE_PROFILER_BUILTIN
    /*Default profiler trace buffer size*/
#define LV_PROFILER_BUILTIN_BUF_SIZE (16 * 1024)     /*[bytes]*/
#endif

/*Header to include for the profiler*/
#define LV_PROFILER_INCLUDE "lvgl/src/misc/lv_profiler_builtin.h"

/*Profiler start point function*/
#define LV_PROFILER_BEGIN    LV_PROFILER_BUILTIN_BEGIN

/*Profiler end point function*/
#define LV_PROFILER_END      LV_PROFILER_BUILTIN_END

/*Profiler start point function with custom tag*/
#define LV_PROFILER_BEGIN_TAG LV_PROFILER_BUILTIN_BEGIN_TAG

/*Profiler end point function with custom tag*/
#define LV_PROFILER_END_TAG   LV_PROFILER_BUILTIN_END_TAG
#endif


/*==================
 * EXAMPLES
 *==================*/
#define LV_BUILD_EXAMPLES 1
#define LV_USE_DEMO_MUSIC 0
#if LV_USE_DEMO_MUSIC
#define LV_DEMO_MUSIC_SQUARE    0
#define LV_DEMO_MUSIC_LANDSCAPE 1
#define LV_DEMO_MUSIC_ROUND     0
#define LV_DEMO_MUSIC_LARGE     0
#define LV_DEMO_MUSIC_AUTO_PLAY 0
#endif
#define LV_USE_DEMO_WIDGETS 1
#define LV_USE_DEMO_BENCHMARK 1
#define LV_USE_DRAW_SW_COMPLEX_GRADIENTS 0

#endif /*LV_CONF_H*/
