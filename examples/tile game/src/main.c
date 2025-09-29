#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>    // optional, if you use files
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "gfx.h"
#include "tile_back.h"
#include "global_palette.h"

// Define TI-84 CE color constants
#define COLOR_RED     gfx_RGBTo1555(255, 0, 0)  // 0xF800
#define COLOR_GREEN   gfx_RGBTo1555(0, 255, 0)  // 0x07E0
#define COLOR_BLUE    gfx_RGBTo1555(0, 0, 255)  // 0x001F
#define COLOR_BLACK   gfx_RGBTo1555(0, 0, 0)    // 0x0000
#define COLOR_WHITE   gfx_RGBTo1555(255, 255, 255)  // 0xFFFF

void set_custom_palette(void) {
    // Set palette index 1 to red
    gfx_palette[1] = COLOR_RED;
    // Set palette index 2 to black (or whatever you want)
    gfx_palette[2] = COLOR_BLACK;
    gfx_palette[3] = COLOR_BLACK;
}

void rand2(int input[], int output[], int size) {
    // Initialize seed with rtc_GetTime()
    uint32_t seed = time(NULL);
    if (seed == 0) seed = 12345; // Fallback to avoid zero seed
    
    // Copy input to output
    for (int i = 0; i < size; i++) {
        output[i] = input[i];
    }
    
    // Fisher-Yates shuffle on output using LCG
    for (int i = size - 1; i > 0; i--) {
        // Generate random number (0–32767)
        seed = (1664525 * seed + 1013904223) & 0x7FFFFFFF; // LCG: 31 bits
        int rand_val = (seed >> 16) & 0x7FFF; // Upper 15 bits
        int j = rand_val % (i + 1); // Random index
        // Swap elements
        int temp = output[i];
        output[i] = output[j];
        output[j] = temp;
    }
}

void itoa(int num, char* str) {
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    int i = 0;
    while (num > 0) {
        int digit = num % 10;
        str[i] = '0' + digit;
        num /= 10;
        i++;
    }
    str[i] = '\0';
    int len = i;
    for (int j = 0; j < len / 2; j++) {
        char temp = str[j];
        str[j] = str[len - 1 - j];
        str[len - 1 - j] = temp;
    }
}

void printCentered(const char* t, int y, uint8_t scale){
    gfx_SetTextScale(scale, scale);
    int w = gfx_GetStringWidth(t) * scale;
    int x = (LCD_WIDTH - w + 1) / 2; // +1 fixes rounding for odd widths
    gfx_PrintStringXY(t, x, y);
}

int level = 1;
int firstt = 124;
int secondt = 150;
int thirdt = 176;
int fourtht = 124;
int fiftht = 150;
int sixtht = 176;
int begin_first_row_y = 107;
int begin_second_row_y = 133;

int regular_tiles[6] = {1,1,2,2,3,3};
int shuffled_tiles[6];

int i = 0;
/* Main function, called first */
int main(void)
{
gfx_Begin();
set_custom_palette();
gfx_SetDrawBuffer();
gfx_FillScreen(0);
gfx_SetColor(15);
gfx_SetTextFGColor(15);
gfx_SetTextBGColor(0);
gfx_SetTextScale(2, 2);
gfx_PrintStringXY("Tile Game", 100, 20);
printCentered("Match the tiles!", 60, 1);
printCentered("Press [ENTER] to start", 100, 1);
gfx_SwapDraw();
while (!os_GetCSC());
if (level == 1) {


    for (i = 0; i < 1; i++) {
        rand2(regular_tiles, shuffled_tiles, 6);
    }
    for (i = 0; i < 1; i++) {
        int tile1 = shuffled_tiles[0];
        int tile2 = shuffled_tiles[1];
        int tile3 = shuffled_tiles[2];
        int tile4 = shuffled_tiles[3];
        int tile5 = shuffled_tiles[4];
        int tile6 = shuffled_tiles[5];
        gfx_FillScreen(255);
        gfx_SetColor(255);
        gfx_SetTextBGColor(255);
        gfx_SetTextFGColor(0);
        // draw each tile numbers
        char t1[2];
        char t2[2];
        char t3[2];
        char t4[2];
        char t5[2];
        char t6[2];
        itoa(tile1, t1);
        itoa(tile2, t2);
        itoa(tile3, t3);
        itoa(tile4, t4);
        itoa(tile5, t5);
        itoa(tile6, t6);
        gfx_PrintStringXY(t1, firstt, begin_first_row_y);
        gfx_PrintStringXY(t2, secondt, begin_first_row_y);
        gfx_PrintStringXY(t3, thirdt, begin_first_row_y);
        gfx_PrintStringXY(t4, fourtht, begin_second_row_y);
        gfx_PrintStringXY(t5, fiftht, begin_second_row_y);
        gfx_PrintStringXY(t6, sixtht, begin_second_row_y);
        gfx_Sprite(tile_back, 0, 0);
        gfx_SwapDraw();
        delay(2000);
    }
}
gfx_End();
return 0;
}
