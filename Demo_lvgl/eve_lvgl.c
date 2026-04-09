/**
 * @file eve_lvgl.c
 * @brief lvgl demo
 *
 * @author Bridgetek
 *
 * @date 2024
 * 
 * MIT License
 *
 * Copyright (c) [2024] [Bridgetek Pte Ltd (BRTChip)]
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "eve_lvgl.h"

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lvgl/examples/lv_examples.h"

static EVE_HalContext s_halContext;
EVE_HalContext* s_pHalContext;

#define HSIZE 480
#define VSIZE 480
#define LCD_SPI1_CS    17
#define LCD_SPI1_SCK   10
#define LCD_SPI1_MOSI  11
#define LCD_RESET      22

void send_data(uint8_t data, bool cmd)
{
	gpio_put(LCD_SPI1_CS, 0);
	EVE_sleep(1);
	uint16_t trans = 0;
	if (cmd)
		trans = data << 7;
	else
		trans = 0x8000 | data << 7;
	spi_write16_blocking(spi1, &trans, 1);
	EVE_sleep(1);
	gpio_put(LCD_SPI1_CS, 1);
}

void spi_write(spi_inst_t *spi, uint8_t *data, uint32_t length)
{
	//send command
	send_data(data[0], true);

	if (length > 1)
	{
		for (int i = 1; i < length; i++)
			send_data(data[i], false);
	}
}

void LCD_init()
{
	uint8_t data1[6] = { 0xFF, 0x77, 0x01, 0x00, 0x00, 0x10 };
	uint8_t data2[3] = { 0xC0, 0x3B, 0x00 };
	uint8_t data3[3] = { 0xC1, 0x0B, 0x02 };
	uint8_t data4[3] = { 0xC2, 0x07, 0x02 };
	uint8_t data5[2] = { 0xCC, 0x10 };
	uint8_t data6[17] = { 0xB0, 0x00, 0x11, 0x16, 0x0E, 0x11, 0x06, 0x05, 0x09, 0x08, 0x21, 0x06, 0x13, 0x10, 0x29, 0x31, 0x18 };
	uint8_t data7[17] = { 0xB1, 0x00, 0x11, 0x16, 0x0E, 0x11, 0x07, 0x05, 0x09, 0x09, 0x21, 0x05, 0x13, 0x11, 0x2A, 0x31, 0x18 };
	uint8_t data8[6] = { 0xFF, 0x77, 0x01, 0x00, 0x00, 0x11 };
	uint8_t data9[2] = { 0xB0, 0x6D };
	uint8_t data10[2] = { 0xB1, 0x37 };
	uint8_t data11[2] = { 0xB2, 0x81 };
	uint8_t data12[2] = { 0xB3, 0x80 };
	uint8_t data13[2] = { 0xB5, 0x43 };
	uint8_t data14[2] = { 0xB7, 0x85 };
	uint8_t data15[2] = { 0xB8, 0x20 };
	uint8_t data16[2] = { 0xC1, 0x78 };
	uint8_t data17[2] = { 0xC2, 0x78 };
	uint8_t data18[2] = { 0xD0, 0x88 };
	uint8_t data19[4] = { 0xE0, 0x00, 0x00, 0x02 };
	uint8_t data20[6] = { 0xE1, 0x03, 0xA0, 0x00, 0x00, 0x04 };
	uint8_t data21[14] = { 0xE2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t data22[5] = { 0xE3, 0x00, 0x00, 0x11, 0x00 };
	uint8_t data23[3] = { 0xE4, 0x22, 0x00 };
	uint8_t data24[17] = { 0xE5, 0x05, 0xEC, 0xA0, 0xA0, 0x07, 0xEE, 0xA0, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t data25[5] = { 0xE6, 0x00, 0x00, 0x11, 0x00 };
	uint8_t data26[3] = { 0xE7, 0x22, 0x00 };
	uint8_t data27[17] = { 0xE8, 0x06, 0xED, 0xA0, 0xA0, 0x08, 0xEF, 0xA0, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t data28[8] = { 0xEB, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00 };
	uint8_t data29[17] = { 0xED, 0xFF, 0xFF, 0xFF, 0xBA, 0x0A, 0xBF, 0x45, 0xFF, 0xFF, 0x54, 0xFB, 0xA0, 0xAB, 0xFF, 0xFF, 0xFF };
	uint8_t data30[7] = { 0xEF, 0x10, 0x0D, 0x04, 0x08, 0x3F, 0x1F };
	uint8_t data31[6] = { 0xFF, 0x77, 0x01, 0x00, 0x00, 0x13 };
	uint8_t data32[2] = { 0xEF, 0x08 };
	uint8_t data33[6] = { 0xFF, 0x77, 0x01, 0x00, 0x00, 0x00 };
	uint8_t data34[2] = { 0x36, 0x08 };
	uint8_t data35[2] = { 0x3A, 0x66 };
	uint8_t data36 = 0x11;
	uint8_t data37 = 0x29;

	spi_init(spi1, 1000 * 1000);
	gpio_set_function(LCD_SPI1_SCK, GPIO_FUNC_SPI);
	gpio_set_function(LCD_SPI1_MOSI, GPIO_FUNC_SPI);
	spi_set_format(spi1, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

	/* Chip select is active-low, so we'll initialise it to a driven-high state */
	gpio_init(LCD_SPI1_CS);
	gpio_set_dir(LCD_SPI1_CS, GPIO_OUT);
	gpio_put(LCD_SPI1_CS, 1);

	// reset LCD
	gpio_init(LCD_RESET);
	gpio_set_dir(LCD_RESET, GPIO_OUT);
	gpio_put(LCD_RESET, 1);
	EVE_sleep(20);
	gpio_put(LCD_RESET, 0);
	EVE_sleep(20);
	gpio_put(LCD_RESET, 1);
	EVE_sleep(20);

	spi_write(spi1, data1, sizeof(data1));
	spi_write(spi1, data2, sizeof(data2));
	spi_write(spi1, data3, sizeof(data3));
	spi_write(spi1, data4, sizeof(data4));
	spi_write(spi1, data5, sizeof(data5));
	spi_write(spi1, data6, sizeof(data6));
	spi_write(spi1, data7, sizeof(data7));
	spi_write(spi1, data8, sizeof(data8));
	spi_write(spi1, data9, sizeof(data9));
	spi_write(spi1, data10, sizeof(data10));
	spi_write(spi1, data11, sizeof(data11));
	spi_write(spi1, data12, sizeof(data12));
	spi_write(spi1, data13, sizeof(data13));
	spi_write(spi1, data14, sizeof(data14));
	spi_write(spi1, data15, sizeof(data15));
	spi_write(spi1, data16, sizeof(data16));
	spi_write(spi1, data17, sizeof(data17));
	spi_write(spi1, data18, sizeof(data18));
	spi_write(spi1, data19, sizeof(data19));
	spi_write(spi1, data20, sizeof(data20));
	spi_write(spi1, data21, sizeof(data21));
	spi_write(spi1, data22, sizeof(data22));
	spi_write(spi1, data23, sizeof(data23));
	spi_write(spi1, data24, sizeof(data24));
	spi_write(spi1, data25, sizeof(data25));
	spi_write(spi1, data26, sizeof(data26));
	spi_write(spi1, data27, sizeof(data27));
	spi_write(spi1, data28, sizeof(data28));
	spi_write(spi1, data29, sizeof(data29));
	spi_write(spi1, data30, sizeof(data30));
	spi_write(spi1, data31, sizeof(data31));
	spi_write(spi1, data32, sizeof(data32));
	spi_write(spi1, data33, sizeof(data33));
	spi_write(spi1, data34, sizeof(data34));
	spi_write(spi1, data35, sizeof(data35));
	EVE_sleep(100);
	spi_write(spi1, &data36, 1);
	EVE_sleep(100);
	spi_write(spi1, &data37, 1);
}

void LCD_deinit()
{
	spi_deinit(spi1);
}

/// /////////////////////////////////////////////////////////////////////////////////////////////
static void eve_touch_read(lv_indev_t* drv, lv_indev_data_t* data)
{
    uint16_t touchX=0, touchY = 0;
    uint16_t isTouch = EVE_Hal_rd32(s_pHalContext, REG_TOUCH_RAW_XY) != 0xFFFFFFFF;

    uint32_t x = EVE_Hal_rd16(s_pHalContext, REG_TOUCH_SCREEN_XY + 2);
    uint32_t y = EVE_Hal_rd16(s_pHalContext, REG_TOUCH_SCREEN_XY + 4);

    if (x > s_pHalContext->Width || y > s_pHalContext->Height)
    {
        isTouch = 0;
    }
    else
    {
        touchX = x;
        touchY = y;
        data->point.x = touchX;
        data->point.y = touchY;
    }

    data->state = (isTouch == 0 ? LV_INDEV_STATE_REL : LV_INDEV_STATE_PR);
}

///////////////////////////////////////////////////////////////////////////////////////////////
#define BUTTON_PIN 21
#define ENCODER_PIN_A 23
#define ENCODER_PIN_B 24
static int16_t encoder = 0;
static int16_t button = 0;
static lv_indev_t *indev_encoder;
void gpio_callback(uint gpio, uint32_t events)
{
	static uint32_t prev_time = 0;
	static int prev_level_A = 0;
	int level_A = gpio_get(ENCODER_PIN_A);
	int level_B = gpio_get(ENCODER_PIN_B);
	int level_btn = gpio_get(BUTTON_PIN);
	static int prev_level_btn = 1;

	if (gpio == ENCODER_PIN_A)
	{
		uint32_t time = EVE_millis();
		if (time - prev_time > 5)
		{
			prev_time = time;
			if ((level_A == 0) && (prev_level_A == 1))
			{
				if (level_B == 1)
				{
					encoder --;
					printf("left\n");
				}
				else
				{
					encoder++;
					printf("right\n");
				}
			}	
		}
		prev_level_A = level_A;
	}
	else if (gpio == BUTTON_PIN)
	{
		if ((level_btn == 1) && (prev_level_btn == 0))
		{
			button++;
			printf("down");
		}
		prev_level_btn = level_btn;
	}
}

void encoder_init()
{
	gpio_set_irq_enabled_with_callback(ENCODER_PIN_A, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
}

void encoder_read(lv_indev_t *indev_encoder, lv_indev_data_t *data)
{
	static int16_t diff = 0;
	//printf("encoder %d, diff %d\n", encoder, diff);

	if (encoder != diff)
	{
		if (encoder > diff)
		{
			data->key = LV_KEY_DOWN;
			data->enc_diff++;
		}
		else
		{
			data->key = LV_KEY_UP;
			data->enc_diff--;
		}
		diff = encoder;
	}
	else
		data->enc_diff = 0;
}

void button_init()
{
	gpio_init(BUTTON_PIN);
	gpio_set_dir(BUTTON_PIN, GPIO_IN);
	gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
}

/*Test if `id` button is pressed or not*/
static bool button_is_pressed(uint8_t id)
{
	static int16_t pressed = 0;
	if ((id == 0) && (button > pressed))
	{
		pressed = button;
		return true;
	}
	else
		return false;
}

/*Get ID  (0, 1, 2 ..) of the pressed button*/
static int8_t button_get_pressed_id(void)
{
	uint8_t i;

	/*Check to buttons see which is being pressed (assume there are 2 buttons)*/
	for (i = 0; i < 2; i++)
	{
		/*Return the pressed button's ID*/
		if (button_is_pressed(i))
		{
			return i;
		}
	}

	/*No button pressed*/
	return -1;
}

void button_read(lv_indev_t *indev, lv_indev_data_t *data)
{
	static uint8_t last_btn = 0; /*Store the last pressed button*/

	/*Get the pressed button's ID*/
	int8_t btn_act = button_get_pressed_id();

	if (btn_act >= 0)
	{
		data->state = LV_INDEV_STATE_PR;
		last_btn = btn_act;
	}
	else
	{
		data->state = LV_INDEV_STATE_REL;
	}

	/*Save the last pressed button's ID*/
	data->btn_id = last_btn;
}


void eve_display_flush(lv_display_t* disp, const lv_area_t* area, uint8_t* color_p)
{
	if (lv_display_flush_is_last(disp))
	{
		EVE_CoCmd_dl(s_pHalContext, DISPLAY());
		EVE_CoCmd_swap(s_pHalContext);
		EVE_Cmd_waitFlush(s_pHalContext);
		uint32_t dl_buff = EVE_Hal_rd32(s_pHalContext, REG_CMD_DL);
		printf("display list %d \n", dl_buff);

		// restart a new display list with proper clearing
		EVE_CoCmd_dlStart(s_pHalContext);
		EVE_CoDl_clearColorRgb(s_pHalContext, 255, 255, 255); /* FT800 FIX: Set clear color */
		EVE_CoDl_clear(s_pHalContext, 1, 1, 1); /* FT800 FIX: Clear color, stencil, and tag buffers */
		EVE_CoDl_vertexFormat(s_pHalContext, 0);
	}
    lv_display_flush_ready(disp);
}

void lv_setup(void)
{
    lv_display_t *display;
	lv_color16_t buf1;
#define DRAW_BUFFER_SIZE (HSIZE * VSIZE * LV_COLOR_DEPTH / 8)
	LCD_init();
    printf("height %d, width %d\n", s_pHalContext->Height, s_pHalContext->Width);
    display = lv_display_create(s_pHalContext->Width, s_pHalContext->Height);
    lv_display_set_flush_cb(display, eve_display_flush);
	lv_display_set_buffers(display, &buf1, NULL, DRAW_BUFFER_SIZE, LV_DISPLAY_RENDER_MODE_FULL);

	/**************** touch ***********************/
    /*Register a touchpad input device*/
    lv_indev_t *indev_touchpad;
    indev_touchpad = lv_indev_create();
    lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_touchpad, eve_touch_read);
	/************** encoder ***********************/
	/*Initialize encoder*/
	encoder_init();
	/*Register a encoder input device*/
	//lv_indev_t *indev_encoder;
	indev_encoder = lv_indev_create();
	lv_indev_set_type(indev_encoder, LV_INDEV_TYPE_ENCODER);
	lv_indev_set_read_cb(indev_encoder, encoder_read);
	lv_indev_set_driver_data(indev_encoder, 0);

	button_init();
	lv_indev_t *indev_button;
	indev_button = lv_indev_create();
	lv_indev_set_type(indev_button, LV_INDEV_TYPE_BUTTON);
	lv_indev_set_read_cb(indev_button, button_read);

	/*Assign buttons to points on the screen*/
	static const lv_point_t btn_points[2] = {
		{ 200, 400 }, /*Button 0 -> x:200; y:400*/
		{ 0, 0 },
	};
	lv_indev_set_button_points(indev_button, btn_points);
}
// ************************************ main loop ************************************
int main(int argc, char* argv[])
{
    s_pHalContext = &s_halContext;
    Gpu_Init(s_pHalContext);

    lv_init(); /* LVGL Init */
    lv_tick_set_cb((lv_tick_get_cb_t)EVE_millis);
    lv_setup();

    // read and store calibration setting
#if !defined(BT8XXEMU_PLATFORM) && GET_CALIBRATION == 1
    //Esd_Calibrate(s_pHalContext);
    //Calibration_Save(s_pHalContext);
#endif

    EVE_Util_clearScreen(s_pHalContext);

    char* info[] =
    { "Lvgl demo",
        "Support QVGA, WQVGA, WVGA",
        "EVE2/3/4",
        "IDM2040"
    };

    //WelcomeScreen(s_pHalContext, info);
    EVE_CoCmd_dlStart(s_pHalContext);
    EVE_CoDl_vertexFormat(s_pHalContext, 0);
    //lv_demo_widgets();
    //lv_demo_benchmark();

    while (1)
    {
        lv_timer_handler();
    }

    EVE_Util_clearScreen(s_pHalContext);
    EVE_Hal_close(s_pHalContext);
    EVE_Hal_release();
    lv_deinit();


#if !defined(BT8XXEMU_PLATFORM) && GET_CALIBRATION == 1
    Calibration_Restore(s_pHalContext);
#endif

    return 0;
}

// ************************************ application ************************************
