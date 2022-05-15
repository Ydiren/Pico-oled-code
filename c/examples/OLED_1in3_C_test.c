/*****************************************************************************
* | File      	:   OLED_1in3_test.c
* | Author      :   
* | Function    :   
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2021-03-16
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
******************************************************************************/
#include <stdio.h>
#include <time.h>
#include "pico/stdio.h"
#include "EPD_Test.h"
#include "LCD_1in14.h"
#include "OLED_1in3_c.h"
#include "ImageData.h"
#include "GUI_Paint.h"
#include "ds3231.h"

//#define RESET_TIME  1

extern char buf[];
extern char *week[];

sFONT* font = &Font24; 

void init_ds3231(void) 
{
   	stdio_init_all();
	i2c_init(I2C_PORT,100*1000);
	gpio_set_function(I2C_SDA,GPIO_FUNC_I2C);
	gpio_set_function(I2C_SCL,GPIO_FUNC_I2C);
	gpio_pull_up(I2C_SDA);
	gpio_pull_up(I2C_SCL);
}

PAINT_TIME getTime(void)  
{
	int i = 0;
   	
    ds3231ReadTime();
    buf[0] = buf[0]&0x7F; //sec
    buf[1] = buf[1]&0x7F; //min
    buf[2] = buf[2]&0x3F; //hour
    buf[3] = buf[3]&0x07; //week
    buf[4] = buf[4]&0x3F; //day
    buf[5] = buf[5]&0x1F; //mouth
    // //year/month/day
    // char date[10];
    // sprintf(date, "20%02x/%02x/%02x  ",buf[6],buf[5],buf[4]);

    // //hour:minute/second
    // char time[10];
    // sprintf(time, "%02x:%02x:%02x  ",buf[2],buf[1],buf[0]);

    // //weekday
    // char day[4];
    // sprintf(day, "%s\n",week[(unsigned char)buf[3]-1]);

    PAINT_TIME paintTime;
    paintTime.Sec = buf[0];
    paintTime.Min = buf[1];
    paintTime.Hour = buf[2];
    paintTime.Day = buf[4];
    paintTime.Month = buf[5];
    paintTime.Year = buf[6];

    // if(buf[6]==0x00 && buf[5]==0x01 && buf[4]==0x01)
    // {
    //     ds3231SetTime(); 
    // }
        
	return paintTime;
}

int setTime(PAINT_TIME newTime)
{
	char buf[2];
	//set second
	buf[0]=0x00;
	buf[1] = newTime.Sec;
	i2c_write_blocking(I2C_PORT,addr,buf,2,false);
	//set minute
	buf[0]=0x01;
	buf[1]=newTime.Min;
	i2c_write_blocking(I2C_PORT,addr,buf,2,false);
	//set hour
	buf[0]=0x02;
	buf[1]=newTime.Hour;
	i2c_write_blocking(I2C_PORT,addr,buf,2,false);

	//set weekday
	// buf[0]=0x03;
	// buf[1]=values[3];
	// i2c_write_blocking(I2C_PORT,addr,buf,2,false);

	//set day
	buf[0]=0x04;
	buf[1]=newTime.Day;
	i2c_write_blocking(I2C_PORT,addr,buf,2,false);
	//set month
	buf[0]=0x05;
	buf[1]=newTime.Month;
	i2c_write_blocking(I2C_PORT,addr,buf,2,false);
	//set year
	buf[0]=0x06;
	buf[1]=newTime.Year;
	i2c_write_blocking(I2C_PORT,addr,buf,2,false);
	return 0;
}

/******************************************************************************
function:	Display time
parameter:
    Xstart           ：X coordinate
    Ystart           : Y coordinate
    pTime            : Time-related structures
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void Paint_DrawTime_NoSecs_Hex(UWORD Xstart, UWORD Ystart, PAINT_TIME *pTime, sFONT* Font,
                    UWORD Color_Foreground, UWORD Color_Background)
{
    uint8_t value[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    UWORD Dx = Font->Width;

    //Write data into the cache
    Paint_DrawChar(Xstart                           , Ystart, value[pTime->Hour / 16], Font, Color_Background, Color_Foreground);
    Paint_DrawChar(Xstart + Dx                      , Ystart, value[pTime->Hour % 16], Font, Color_Background, Color_Foreground);
    Paint_DrawChar(Xstart + Dx  + Dx / 4 + Dx / 2   , Ystart, ':'                    , Font, Color_Background, Color_Foreground);
    Paint_DrawChar(Xstart + Dx * 2 + Dx / 2         , Ystart, value[pTime->Min / 16] , Font, Color_Background, Color_Foreground);
    Paint_DrawChar(Xstart + Dx * 3 + Dx / 2         , Ystart, value[pTime->Min % 16] , Font, Color_Background, Color_Foreground);
    // Paint_DrawChar(Xstart + Dx * 4 + Dx / 2 - Dx / 4, Ystart, ':'                    , Font, Color_Background, Color_Foreground);
    // Paint_DrawChar(Xstart + Dx * 5                  , Ystart, value[pTime->Sec / 16] , Font, Color_Background, Color_Foreground);
    // Paint_DrawChar(Xstart + Dx * 6                  , Ystart, value[pTime->Sec % 16] , Font, Color_Background, Color_Foreground);
    
}

int OLED_1in3_C_test(void)
{
    DEV_Delay_ms(100);
    
    printf("OELD_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        while(1){
            printf("END\r\n");
        }
    }
    
    /* Init */
    init_ds3231();
    OLED_1in3_C_Init();
    OLED_1in3_C_Clear();

    srand(time(NULL));
    
    UBYTE *BlackImage;
    UWORD Imagesize = ((OLED_1in3_C_WIDTH%8==0)? (OLED_1in3_C_WIDTH/8): (OLED_1in3_C_WIDTH/8+1)) * OLED_1in3_C_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        while(1){
            printf("Failed to apply for black memory...\r\n");
        }
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, OLED_1in3_C_WIDTH, OLED_1in3_C_HEIGHT, 0, WHITE);	
    
    #ifdef RESET_TIME

    PAINT_TIME newTime;
    newTime.Sec = 0x0;
    newTime.Min = 0x43;
    newTime.Hour = 0x21;
    newTime.Day = 0x13;
    newTime.Month = 5;
    newTime.Year = 0x22;
    setTime(newTime);

    #endif
    
    printf("Drawing\r\n");
    Paint_SelectImage(BlackImage);
    DEV_Delay_ms(500);
    Paint_Clear(BLACK);

    PAINT_TIME currentTime;
    PAINT_TIME previousTime;
    uint16_t x = ((OLED_1in3_C_WIDTH - 72)/2);
    uint16_t y = 22;
    uint8_t quietTime = false;
    while(1)
    {
        Paint_Clear(BLACK);

        previousTime = currentTime;
        currentTime = getTime();

        if (currentTime.Hour >= 0x22 || currentTime.Hour < 0x08) 
        {
            if (!quietTime)
            {
                //Clear the display only when entering quiet time
                OLED_1in3_C_Display(BlackImage);
                quietTime = true;
            }
        }
        else 
        {
            quietTime = false;
            if (currentTime.Min != previousTime.Min && (currentTime.Min % 16) % 5 == 0)
            {
                x = rand() % (OLED_1in3_C_WIDTH - 72);
                y = rand() % (OLED_1in3_C_HEIGHT - 24);
            }
            Paint_DrawTime_NoSecs_Hex(x, y, &currentTime, font, WHITE, BLACK);
            OLED_1in3_C_Display(BlackImage);
        }        

        DEV_Delay_ms(1000);
    }
    

    // 2.Drawing on the image
    // printf("Drawing:page 1\r\n");
    // Paint_DrawPoint(20, 10, WHITE, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    // Paint_DrawPoint(30, 10, WHITE, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    // Paint_DrawPoint(40, 10, WHITE, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    // Paint_DrawLine(10, 10, 10, 20, WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    // Paint_DrawLine(20, 20, 20, 30, WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    // Paint_DrawLine(30, 30, 30, 40, WHITE, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    // Paint_DrawLine(40, 40, 40, 50, WHITE, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    // Paint_DrawCircle(60, 30, 15, WHITE, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    // Paint_DrawCircle(100, 40, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    // Paint_DrawRectangle(50, 30, 60, 40, WHITE, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    // Paint_DrawRectangle(90, 30, 110, 50, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    // BlackImage[0] = 0xf0;
    // // 3.Show image on page1
    // OLED_1in3_C_Display(BlackImage);
    // DEV_Delay_ms(2000);
    // Paint_Clear(BLACK);
    
    
    
    // // Drawing on the image
    // printf("Drawing:page 2\r\n");
    // Paint_DrawString_EN(10, 0, "Pico-OLED", &Font16, WHITE, BLACK);
    // Paint_DrawString_EN(10, 17, "hello world", &Font8, WHITE, BLACK);
    // Paint_DrawNum(10, 30, 123.1, &Font8,2, WHITE, WHITE);
    // Paint_DrawNum(10, 43, 987654.2, &Font12,2, WHITE, WHITE);
    // // Show image on page2
    // OLED_1in3_C_Display(BlackImage);
    // DEV_Delay_ms(2000);
    // Paint_Clear(BLACK);
    
    // // Drawing on the image
    // printf("Drawing:page 3\r\n");
    // Paint_DrawString_CN(10, 0,"���Abc", &Font12CN, WHITE, WHITE);
    // Paint_DrawString_CN(0, 20, "����", &Font24CN, WHITE, WHITE);
    // // Show image on page3
    // OLED_1in3_C_Display(BlackImage);
    // DEV_Delay_ms(2000);
    
    // // Show image on page4
    // OLED_1in3_C_Display(gImage_1inch3_C_1);
    // DEV_Delay_ms(5000);
    
    
    // Paint_NewImage(BlackImage, OLED_1in3_C_WIDTH, OLED_1in3_C_HEIGHT, 180, WHITE);	
    // Paint_Clear(BLACK);
    // int key0 = 15; 
    // int key1 = 17;
    // int key = 0;
    // DEV_GPIO_Mode(key0, 0);
    // DEV_GPIO_Mode(key1, 0);
    
    // Paint_Clear(BLACK);
    // OLED_1in3_C_Display(BlackImage);
    
    
    // while(1){
    //     if(DEV_Digital_Read(key1 ) == 0){
    //         Paint_DrawRectangle(115, 5, 125, 15, WHITE, DOT_PIXEL_2X2,DRAW_FILL_FULL);
    //         key = 1;
    //     }else {
    //         Paint_DrawRectangle(115, 5, 125, 15, BLACK, DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    //     }
            
    //     if(DEV_Digital_Read(key0 ) == 0){
    //         Paint_DrawRectangle(115, 50, 125, 60, WHITE, DOT_PIXEL_2X2,DRAW_FILL_FULL);
    //     }else {
    //         Paint_DrawRectangle(115, 50, 125, 60, BLACK, DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    //         key = 1;
            
    //     }
    //     if(key == 1){
    //         OLED_1in3_C_Display(BlackImage);
    //         Paint_Clear(BLACK);
    //     }
        
    // }

    
    
    DEV_Module_Exit();
    return 0;
}
