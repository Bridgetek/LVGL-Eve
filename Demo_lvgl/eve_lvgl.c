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

#define TOUCH_DATA_LEN 1620
static uint8_t c_TouchDataU8[TOUCH_DATA_LEN] = {
26,255,255,255,84,37,16,0,4,0,0,0,1,0,0,0,26,255,255,255,224,36,16,0,4,0,0,0,1,0,0,0,26,255,255,255,0,192,28,0,8,6,0,0,201,66,0,128,140,102,1,128,143,98,2,128,143,98,39,97,16,97,29,107,156,97,16,97,39,97,16,97,29,107,140,96,0,128,143,103,16,97,157,97,17,97,157,97,3,97,143,97,39,97,39,97,20,64,29,107,29,107,7,64,39,97,7,64,29,107,140,96,29,64,22,0,39,97,140,96,39,97,17,97,29,107,156,97,3,103,140,102,16,64,140,102,0,128,143,104,46,64,140,102,0,128,16,97,143,104,3,104,140,102,51,64,140,102,16,97,143,111,0,106,140,102,59,64,143,98,17,96,46,64,67,32,59,0,140,96,20,64,3,104,73,32,3,97,74,0,143,96,140,96,20,64,51,64,70,0,17,96,143,98,1,128,143,105,17,96,0,108,16,97,3,64,140,108,10,128,51,96,143,97,7,64,87,64,51,96,143,97,17,96,46,0,1,128,0,128,7,64,3,98,39,97,17,97,3,98,17,96,7,64,3,111,110,32,29,107,3,64,111,0,29,107,140,96,0,102,16,97,0,102,16,97,96,0,17,96,46,64,121,32,112,0,140,96,39,97,0,128,0,128,29,107,17,96,141,32,36,96,1,128,3,99,134,32,24,64,98,64,29,64,20,64,98,64,29,64,29,107,80,64,126,0,3,97,34,0,1,128,16,97,51,96,143,97,0,128,16,97,51,96,143,97,0,128,143,64,0,96,0,96,0,96,0,96,0,96,0,128,147,64,0,96,0,96,0,96,140,96,0,128,17,101,169,32,3,64,165,0,22,0,1,128,143,64,9,128,0,108,3,128,3,99,3,128,3,98,151,64,0,106,0,128,0,103,178,32,3,97,1,128,147,64,151,64,151,64,151,64,151,64,151,64,151,64,151,64,151,64,151,64,151,64,3,128,140,108,20,64,19,128,82,64,7,64,122,64,39,97,39,97,17,128,82,64,7,64,122,64,29,107,29,107,98,64,21,128,82,64,98,64,3,96,11,64,25,128,82,64,7,64,122,64,39,97,39,97,23,128,82,64,7,64,122,64,29,107,29,107,98,64,27,128,82,64,98,64,143,96,120,131,4,128,51,96,143,97,186,131,4,128,51,96,143,97,184,131,4,128,51,96,143,97,189,131,4,128,51,96,143,97,188,131,4,128,51,96,143,97,185,131,4,128,51,96,143,97,184,131,4,128,51,96,143,97,248,131,4,128,51,96,143,97,4,128,0,106,17,96,16,64,11,33,143,97,10,65,10,1,144,129,16,65,16,65,0,106,17,96,16,64,19,33,3,97,0,106,17,96,16,64,18,33,143,97,2,65,16,65,250,64,16,65,246,0,254,64,16,65,2,65,16,65,250,64,16,65,246,64,16,1,254,64,16,65,246,64,16,65,250,64,16,65,2,1,170,64,9,128,143,105,254,64,16,65,6,65,51,65,57,33,16,65,2,65,51,65,254,64,16,1,72,33,254,64,10,65,2,65,16,65,254,64,10,65,78,1,246,64,10,65,250,64,16,65,246,64,10,1,140,96,17,96,128,128,3,99,64,65,78,0,254,64,10,65,79,65,79,65,79,65,79,65,79,65,79,65,79,65,79,65,3,97,54,1,78,64,54,65,143,98,0,128,96,65,96,65,96,65,96,65,96,65,96,65,96,65,96,65,16,97,64,1,113,33,0,128,36,0,140,96,44,129,18,65,0,228,31,128,51,96,3,97,44,129,18,65,17,96,31,128,51,96,3,97,114,1,140,96,0,128,99,1,128,65,8,128,3,113,128,65,143,100,255,255,0,102,157,96,135,65,20,64,20,64,20,64,20,0,31,128,51,96,3,97,35,128,51,96,3,97,34,128,90,64,33,128,90,64,30,128,90,64,32,128,90,0,31,65,112,128,84,65,110,65,84,65,110,1,1,64,99,65,3,97,44,1,36,96,0,128,3,103,174,33,32,128,90,64,207,1,17,107,1,128,3,103,181,33,30,128,90,64,207,1,17,107,2,128,3,103,188,33,33,128,90,64,207,1,17,107,3,128,3,103,195,33,34,128,90,64,207,1,17,107,4,128,3,103,206,33,31,128,51,96,3,97,35,128,51,96,3,97,207,1,22,64,12,96,140,96,17,96,5,128,3,111,218,33,1,128,16,97,3,113,3,100,219,1,143,97,140,96,39,97,17,96,229,33,16,97,17,107,36,64,16,97,0,106,221,1,3,97,12,96,163,65,17,96,0,128,17,96,5,128,3,104,251,33,39,97,17,96,1,128,3,99,16,64,247,33,135,65,17,107,167,65,80,64,29,107,3,64,234,1,22,0,130,65,255,143,3,99,130,65,17,96,255,143,3,99,16,97,12,128,3,105,15,128,3,99,39,97,130,65,3,97,198,64,17,107,167,65,29,107,209,1,1,128,157,65,44,65,31,65,113,128,84,65,110,65,128,65,3,97,0,128,128,65,252,129,220,65,143,97,1,64,1,64,255,255,0,102,17,96,255,255,140,96,31,128,51,96,3,97,32,128,90,64,20,64,30,128,90,64,33,128,90,0,2,128,157,65,44,65,31,65,113,128,84,65,110,65,128,65,66,34,130,65,255,143,3,99,130,65,255,143,3,99,20,64,198,64,200,128,67,2,30,66,163,65,37,2,1,128,37,128,51,96,143,97,234,64,16,1,11,128,0,108,0,128,3,103,143,100,73,66,17,96,36,64,75,66,110,65,51,65,16,64,81,34,143,97,73,66,17,96,36,64,75,66,110,65,51,65,90,34,143,97,13,128,0,108,0,128,143,103,97,66,140,102,101,130,80,66,16,66,69,66,101,130,89,66,103,2,140,96,0,128,39,97,97,130,80,66,47,66,69,66,97,130,89,66,113,2,140,96,31,65,232,128,84,65,110,65,84,65,110,1,128,65,0,106,39,97,130,65,130,65,130,65,3,97,198,64,17,107,167,65,29,107,209,1,31,65,233,128,84,65,110,65,0,128,128,65,7,128,3,99,127,130,220,1,31,65,233,128,84,65,110,65,128,65,7,128,3,99,169,34,128,65,128,128,3,99,163,34,30,66,168,2,130,65,130,65,20,64,198,64,200,128,170,2,30,66,163,65,37,2,97,130,80,66,97,130,89,66,149,66,69,66,172,2,140,96,0,128,101,130,80,66,101,130,89,66,139,66,18,64,3,100,190,34,69,66,181,2,140,96,73,66,0,128,3,64,32,206,3,103,51,65,3,100,194,34,143,97,242,64,11,128,0,108,0,128,42,64,202,34,101,66,17,96,213,34,30,66,37,66,215,2,138,65,143,65,69,66,238,64,9,128,0,108,4,128,3,99,16,64,228,34,5,128,18,65,242,64,44,129,18,65,101,66,42,64,110,65,1,128,18,65,31,65,112,128,84,65,44,65,16,64,244,34,101,66,243,34,111,66,244,2,103,66,192,66,31,65,232,128,84,65,44,65,16,64,0,35,101,66,255,34,172,66,0,3,180,66,10,128,18,65,1,64,110,1,26,255,255,255,84,37,16,0,4,0,0,0,0,0,0,0,26,255,255,255,28,36,16,0,4,0,0,0,0,0,0,0

};
/**
 * @brief Upload touch firmware for FT800 chip
 *
 * @param phost  Pointer to Hal context
 */
static inline void uploadTouchFirmware(EVE_HalContext *phost)
{
	eve_assert_do(EVE_Cmd_wrProgMem(phost, c_TouchDataU8, TOUCH_DATA_LEN));
	eve_assert_do(EVE_Cmd_waitFlush(phost));
}

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

void eve_display_reconfigure_init()
{
	int16_t Width = VSIZE; /**< Line buffer width (pixels) */
	int16_t Height = HSIZE; /**< Screen and render height (lines) */
	int16_t HCycle = 548; /**< Total number of clocks per line, including blanking */
	int16_t HOffset = 43; /**< Start of active line (clocks) */
	int16_t HSync0 = 0; /**< Start of horizontal sync pulse (clocks) */
	int16_t HSync1 = 41; /**< End of horizontal sync pulse (clocks) */
	int16_t VCycle = 512; /**< Total number of lines per frame, including blanking */
	int16_t VOffset = 12; /**< Start of active frame (lines) */
	int16_t VSync0 = 0; /**< Start of vertical sync pulse (lines) */
	int16_t VSync1 = 10; /**< End of vertical sync pulse (lines) */
	uint8_t PCLK = 7; /**< Pixel clock frequency (MHz) */
	int8_t Swizzle = 0;
	int8_t PCLKPol = 1;
	int8_t CSpread = 1;
	uint8_t OutBitsR = 5;
	uint8_t OutBitsG = 6;
	uint8_t OutBitsB = 5;
	bool Dither = 1;
	EVE_Hal_wr16(s_pHalContext, REG_HCYCLE, HCycle);
	EVE_Hal_wr16(s_pHalContext, REG_HOFFSET, HOffset);
	EVE_Hal_wr16(s_pHalContext, REG_HSYNC0, HSync0);
	EVE_Hal_wr16(s_pHalContext, REG_HSYNC1, HSync1);
	EVE_Hal_wr16(s_pHalContext, REG_VCYCLE, VCycle);
	EVE_Hal_wr16(s_pHalContext, REG_VOFFSET, VOffset);
	EVE_Hal_wr16(s_pHalContext, REG_VSYNC0, VSync0);
	EVE_Hal_wr16(s_pHalContext, REG_VSYNC1, VSync1);
	EVE_Hal_wr8(s_pHalContext, REG_SWIZZLE, Swizzle);
	EVE_Hal_wr8(s_pHalContext, REG_PCLK_POL, PCLKPol);
	EVE_Hal_wr16(s_pHalContext, REG_HSIZE, Width);
	EVE_Hal_wr16(s_pHalContext, REG_VSIZE, Height);
	EVE_Hal_wr16(s_pHalContext, REG_CSPREAD, CSpread);
	EVE_Hal_wr16(s_pHalContext, REG_DITHER, Dither);
	EVE_Hal_wr16(s_pHalContext, REG_OUTBITS, (uint16_t)(((OutBitsR & 0x7) << 6) | ((OutBitsG & 0x7) << 3) | (	OutBitsB & 0x7)));
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
	s_pHalContext->Width = VSIZE;
	s_pHalContext->Height = HSIZE;
	eve_display_reconfigure_init();
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
	uploadTouchFirmware(s_pHalContext);
	EVE_Hal_flush(s_pHalContext);
	EVE_sleep(100);
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
