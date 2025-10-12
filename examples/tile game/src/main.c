#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>    // optional, if you use files
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>      // For sprintf in debug (optional)
#include "gfx.h"
#include "tile_back.h"
#include "ntwo.h"
#include "nthree.h"
#include "nfour.h"
#include "ace.h"
#include "global_palette.h"

// Pre-bake all tile backgrounds at startup to avoid malloc/free every frame
gfx_sprite_t *baked_tiles[10];
int tile_positions[10][2] = {
    {53, 51},   // firstt, begin_first_row_y
    {101, 51},  // secondt, begin_first_row_y
    {149, 51},  // thirdt, begin_first_row_y
    {53, 119},  // fourtht, begin_second_row_y
    {101, 119}, // fiftht, begin_second_row_y
    {149, 119}, // sixtht, begin_second_row_y
    {197, 51},  // seventht, begin_first_row_y
    {245, 51},  // eightht, begin_first_row_y
    {197, 119}, // ninetht, begin_second_row_y
    {245, 119}  // tentht, begin_second_row_y
};

void pre_bake_tiles() {
    for (int i = 0; i < 10; i++) {
        baked_tiles[i] = gfx_MallocSprite(tile_back->width, tile_back->height);
        if (baked_tiles[i] == NULL) continue;
        
        // Capture background
        gfx_GetSprite(baked_tiles[i], tile_positions[i][0], tile_positions[i][1]);
        
        // Overlay non-transparent pixels
        uint8_t *orig_data = tile_back->data;
        uint8_t *baked_data = baked_tiles[i]->data;
        int size = tile_back->width * tile_back->height;
        
        for (int k = 0; k < size; k++) {
            if (orig_data[k] != 1) {
                baked_data[k] = orig_data[k];
            }
            if ((k == 0 || k == 1 || k == 37 || k == 38) && orig_data[k] == 0) {
                baked_data[k] = 1;
            }
        }
    }
}

void free_baked_tiles() {
    for (int i = 0; i < 10; i++) {
        if (baked_tiles[i] != NULL) {
            free(baked_tiles[i]);
        }
    }
}

// Fast version using pre-baked tiles
void fast_draw_tile(int index) {
    if (baked_tiles[index] != NULL) {
        gfx_Sprite(baked_tiles[index], tile_positions[index][0], tile_positions[index][1]);
    }
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

// Positions: 10px spacing between sprites, 20px on sides
// Sprites: 38x58px
// To move: +48 x, +68 y
int level = 1;
int firstt = 53;
int secondt = 101;
int thirdt = 149;
int fourtht = 53;
int fiftht = 101;
int sixtht = 149;
int seventht = 197;
int eightht = 245;
int ninetht = 197;
int tentht = 245;
int begin_first_row_y = 51;
int begin_second_row_y = 119;

int regular_tiles[10] = {1,1,2,2,3,3,4,4,5,5};
int shuffled_tiles[10];

int i = 0;
int boxx = 51; // Start with first tile highlighted
int boxy = 49;

int selected_tile;
int firstst = -1;
int secondst = -1;
int first_index_tile_num_to_flip_over = -1; // index of the tile number for the first tile flipped over
int second_index_tile_num_to_flip_over = -1; // index of the tile number for the second tile flipped over

void draw_board() {
    gfx_FillScreen(255);  // White background
    // Draw all 10 backs
    fast_draw_tile(0);
    fast_draw_tile(1);
    fast_draw_tile(2);
    fast_draw_tile(3);
    fast_draw_tile(4);
    fast_draw_tile(5);
    fast_draw_tile(6);
    fast_draw_tile(7);
    fast_draw_tile(8);
    fast_draw_tile(9);
    if (flipedovertilenum == 1) {
        //flip ovee the correct tile based on 
    }
    gfx_SetColor(244);
    // Highlight current tile
    gfx_Rectangle(boxx, boxy, 42, 62);
    gfx_SetColor(0);
    gfx_SwapDraw();
}

void flip_over_tile(int wttfo) {  // wttfo = which tile to flip over    
    int fotn = shuffled_tiles[wttfo];  // Get the tile number
    char fotnas[2];
    itoa(fotn, fotnas);
    draw_board();
    gfx_PrintStringXY(fotnas, 1, 1);
    gfx_SwapDraw();
    msleep(1000);
    draw_board();
    //fotn is the tile number (1-5)
    //wttfo is the tile index (0-9) / (1-10)
    if (firstst != -1) {
        secondst = fotn;
        if (firstst == secondst) {
            draw_board();
            gfx_PrintStringXY("match", 1, 1);
            int flipedovertilenum = fotn;
            gfx_SwapDraw();
        } else {
            draw_board();
            gfx_PrintStringXY("nope", 1, 1);
            gfx_SwapDraw();
        }
        firstst = -1; // Reset for next pair
        secondst = -1;
    } else if (firstst == -1) {
        firstst = fotn;
    }
}

/* Main function, called first */
int main(void)
{
    pre_bake_tiles();
    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);  // Apply the generated palette
    gfx_SetDrawBuffer();
    gfx_FillScreen(0);
    gfx_SetColor(15);
    gfx_SetTextFGColor(15);
    gfx_SetTextBGColor(0);
    gfx_SetTextScale(2, 2);
    gfx_PrintStringXY("Tile Game", 100, 20);
    printCentered("Match the tiles!", 60, 1);
    printCentered("Press [ENTER] to start", 100, 1);
    printCentered("Press [CLEAR] to quit", 120, 1);
    gfx_SwapDraw();
    while (!kb_IsDown(kb_KeyEnter));
    
    if (level == 1) {
        // Logic part: Shuffle once
        rand2(regular_tiles, shuffled_tiles, 10);
        
        // Show numbers on white background briefly (1 iteration for demo)
        for (i = 0; i < 1; i++) {
            int tile1 = shuffled_tiles[0];
            int tile2 = shuffled_tiles[1];
            int tile3 = shuffled_tiles[2];
            int tile4 = shuffled_tiles[3];
            int tile5 = shuffled_tiles[4];
            int tile6 = shuffled_tiles[5];
            int tile7 = shuffled_tiles[6];
            int tile8 = shuffled_tiles[7];
            int tile9 = shuffled_tiles[8];
            int tile10 = shuffled_tiles[9];
            gfx_FillScreen(255);
            gfx_SetColor(255);
            gfx_SetTextBGColor(255);
            gfx_SetTextFGColor(0);
            char t1[2];
            char t2[2];
            char t3[2];
            char t4[2];
            char t5[2];
            char t6[2];
            char t7[2];
            char t8[2];
            char t9[2];
            char t10[2];
            itoa(tile1, t1);
            itoa(tile2, t2);
            itoa(tile3, t3);
            itoa(tile4, t4);
            itoa(tile5, t5);
            itoa(tile6, t6);
            itoa(tile7, t7);
            itoa(tile8, t8);
            itoa(tile9, t9);
            itoa(tile10, t10);
            gfx_PrintStringXY(t1, firstt, begin_first_row_y);
            gfx_PrintStringXY(t2, secondt, begin_first_row_y);
            gfx_PrintStringXY(t3, thirdt, begin_first_row_y);
            gfx_PrintStringXY(t7, seventht, begin_first_row_y);
            gfx_PrintStringXY(t8, eightht, begin_first_row_y);
            gfx_PrintStringXY(t4, fourtht, begin_second_row_y);
            gfx_PrintStringXY(t5, fiftht, begin_second_row_y);
            gfx_PrintStringXY(t6, sixtht, begin_second_row_y);
            gfx_PrintStringXY(t9, ninetht, begin_second_row_y);
            gfx_PrintStringXY(t10, tentht, begin_second_row_y);
            gfx_SwapDraw();  // Swap here to show numbers
            msleep(2000);    // Pause 2 seconds to memorize (add <tice.h> if needed)
        }
        draw_board();
    }
    
    // Game loop
    while (1) {
        uint8_t key = os_GetCSC();
        if (key == sk_Clear) {
            break;
        }
        if (key == sk_Right) {
            // Move right, but don't exceed last position (243 for 5th tile)
            if (boxx < 243) {
                boxx += 48;
                draw_board();
            }
        }
        if (key == sk_Left) {
            // Move left, but don't exceed first position (51 for 1st tile)
            if (boxx > 51) {    
                boxx -= 48;
                draw_board();
            }
        }
        if (key == sk_Down) {
            if (boxy < 117) {
                boxy += 68;
                draw_board();
            }
        }
        if (key == sk_Up) {
            if (boxy > 51) {
                boxy -= 68;
                draw_board();
            }
        }
        
    // Determine selected tile based on boxx and boxy - keep explicit chain
    // but ensure constants match tile_positions (outline origin = tile_pos - 2)
    if (boxx == 51 && boxy == 49) selected_tile = 0;
    else if (boxx == 99 && boxy == 49) selected_tile = 1;
    else if (boxx == 147 && boxy == 49) selected_tile = 2;
    else if (boxx == 51 && boxy == 117) selected_tile = 3;
    else if (boxx == 99 && boxy == 117) selected_tile = 4;
    else if (boxx == 147 && boxy == 117) selected_tile = 5;
    else if (boxx == 195 && boxy == 49) selected_tile = 6;
    else if (boxx == 243 && boxy == 49) selected_tile = 7;
    else if (boxx == 195 && boxy == 117) selected_tile = 8;
    else if (boxx == 243 && boxy == 117) selected_tile = 9;
        if (key == sk_Enter) {
            flip_over_tile(selected_tile);
        }
    }
    
    free_baked_tiles();
    gfx_End();
    return 0;
}