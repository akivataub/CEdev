//infinte scrolling ground demo with sprites
#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <stdlib.h>
#include <time.h>
#include "gfx.h"
#include "global_palette.h"
#include "bush.h"

#define SCREEN_WIDTH 320 //x
#define SCREEN_HEIGHT 240 //y

int lanes = 3;
int whichlanetoputspritein = 0;
int fbushy; //stands for first bush y
int sbushy; //stands for second bush y
int tbushy; //stands for third bush y

void copy_background_to_sprite(gfx_sprite_t *sprite, int x, int y) {
    gfx_TransparentSprite(sprite, x, y);
}

void makelinesthicker(int height, int x, int y, int thickness) {
    for (int i = 0; i < thickness; i++) {
        gfx_Line(x + i, y, x + i, height);
    }
}

void drawground() {
    //set color to green not in hex code
    gfx_SetColor(6);
    //make lines thicker
    makelinesthicker(SCREEN_HEIGHT, 0, 0, 40);
    makelinesthicker(SCREEN_HEIGHT, (SCREEN_WIDTH - 40), 0, 40);
    copy_background_to_sprite(bush, 4, fbushy);
    copy_background_to_sprite(bush, 284, sbushy);
    copy_background_to_sprite(bush, 284, tbushy);
}

void randomInteger(int min, int max, int *result) {
    static uint32_t seed = 0;
    // Initialize seed once
    if (seed == 0) {
        seed = time(NULL);
        if (seed == 0) seed = 12345;
    }
    // Handle single value case
    if (min == max) {
        *result = min;
        return;
    }
    // Ensure min <= max
    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }
    int range = max - min + 1;
    // Generate next random number using LCG
    seed = (1664525 * seed + 1013904223) & 0x7FFFFFFF;
    // Extract random value and scale to range
    int rand_val = (seed >> 16) & 0x7FFF;
    *result = min + (rand_val % range);
}

int main(void)
{
    gfx_Begin();
    gfx_FillScreen(255);
    randomInteger(0, 240, &fbushy);
    randomInteger(0, 240, &sbushy);
    randomInteger(0, 240, &tbushy);
    while (!os_GetCSC()) {
        // first bush
        if (fbushy == SCREEN_HEIGHT) {
            fbushy = 1;
            drawground();
        } else if (fbushy < SCREEN_HEIGHT) {
            fbushy += 1;
            drawground();
        }
        // same for second bush just sbushy
        if (sbushy == SCREEN_HEIGHT) {
            sbushy = 1;
            drawground();
        } else if (sbushy < SCREEN_HEIGHT) {
            sbushy += 1;
            drawground();
        }
        // same for third bush just tbushy
        if (tbushy == sbushy || abs(tbushy - sbushy) < 32) {
            do {
                randomInteger(0, 240, &tbushy);
            } while (abs(tbushy - sbushy) < 32); // Keep trying until they're far enough apart
            drawground();
        } else if (tbushy == SCREEN_HEIGHT) {
            tbushy = 1;
            drawground();
        } else if (tbushy < SCREEN_HEIGHT) {
            tbushy += 1;
            drawground();
        }
        gfx_SwapDraw();
        delay(0);
    }
    gfx_End();
    return 0;
}
