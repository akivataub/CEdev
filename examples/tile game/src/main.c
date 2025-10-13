#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "gfx.h"
#include "tile_back.h"
#include "ntwo.h"
#include "nthree.h"
#include "nfour.h"
#include "nfive.h"
#include "ace.h"
#include "global_palette.h"

gfx_sprite_t *baked_tiles[10];
int tile_positions[10][2] = {
    {53, 51},
    {101, 51},
    {149, 51},
    {53, 119},
    {101, 119},
    {149, 119},
    {197, 51},
    {245, 51},
    {197, 119},
    {245, 119}
};

int regular_tiles[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
int shuffled_tiles[10];
int level = 1;
int boxx = 51;
int boxy = 49;
int selected_tile;
int firstst = -1;
int secondst = -1;
int first_flipped_index = -1;
bool is_flipped[10] = {false};

void pre_bake_tiles() {
    for (int i = 0; i < 10; i++) {
        baked_tiles[i] = gfx_MallocSprite(tile_back->width, tile_back->height);
        if (baked_tiles[i] == NULL) continue;
        
        gfx_GetSprite(baked_tiles[i], tile_positions[i][0], tile_positions[i][1]);
        
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

void fast_draw_tile(int index) {
    if (baked_tiles[index] != NULL) {
        gfx_Sprite(baked_tiles[index], tile_positions[index][0], tile_positions[index][1]);
    }
}

void copy_background_to_sprite(gfx_sprite_t *sprite, int x, int y, int width, int height) {
    if (sprite == NULL || sprite->width != width || sprite->height != height) {
        sprite = gfx_MallocSprite(width, height);
        if (sprite == NULL) {
            return;
        }
    }
    
    gfx_GetSprite(sprite, x, y);
}

void rand2(int input[], int output[], int size) {
    uint32_t seed = time(NULL);
    if (seed == 0) seed = 12345;
    
    for (int i = 0; i < size; i++) {
        output[i] = input[i];
    }
    
    for (int i = size - 1; i > 0; i--) {
        seed = (1664525 * seed + 1013904223) & 0x7FFFFFFF;
        int rand_val = (seed >> 16) & 0x7FFF;
        int j = rand_val % (i + 1);
        int temp = output[i];
        output[i] = output[j];
        output[j] = temp;
    }
}

void draw_board() {
    gfx_FillScreen(255);
    
    for (int i = 0; i < 10; i++) {
        fast_draw_tile(i);
    }
    
    for (int i = 0; i < 10; i++) {
        if (is_flipped[i]) {
            int tile_num = shuffled_tiles[i];
            int x = tile_positions[i][0];
            int y = tile_positions[i][1];
            switch (tile_num) {
                case 1:
                    gfx_Sprite(ace, x, y);
                    break;
                case 2:
                    gfx_Sprite(ntwo, x, y);
                    break;
                case 3:
                    gfx_Sprite(nthree, x, y);
                    break;
                case 4:
                    gfx_Sprite(nfour, x, y);
                    break;
                case 5:
                    gfx_Sprite(nfive, x, y);
                    break;
            }
        }
    }
    
    gfx_SetColor(244);
    gfx_Rectangle(boxx, boxy, 42, 62);
    gfx_SetColor(0);
    gfx_SwapDraw();
}

void flip_over_tile(int wttfo) {
    int fotn = shuffled_tiles[wttfo];
    
    is_flipped[wttfo] = true;
    draw_board();
    
    if (firstst != -1) {
        secondst = fotn;
        if (firstst == secondst) {
            draw_board();
            gfx_SwapDraw();
        } else {
            draw_board();
            gfx_SwapDraw();
            is_flipped[wttfo] = false;
            if (first_flipped_index != -1) {
                is_flipped[first_flipped_index] = false;
            }
            draw_board();
        }
        firstst = -1;
        secondst = -1;
        first_flipped_index = -1;
    } else {
        firstst = fotn;
        first_flipped_index = wttfo;
    }
}

int main(void)
{
    pre_bake_tiles();
    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetDrawBuffer();
    
    rand2(regular_tiles, shuffled_tiles, 10);
    draw_board();
    
    while (1) {
        uint8_t key = os_GetCSC();
        if (key == sk_Clear) {
            break;
        }
        if (key == sk_Right) {
            if (boxx < 243) {
                boxx += 48;
                draw_board();
            }
        }
        if (key == sk_Left) {
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