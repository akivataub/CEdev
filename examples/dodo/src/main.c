#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <rtc.h>
#include "gfx.h"
#include "b1.h"
#include "b2.h"

#define MAX_SPRITES 10
#define PHOTO_WIDTH 128
#define PHOTO_HEIGHT 128
#define PADDING 10
#define ARROW_SIZE 20

gfx_sprite_t *sprites[MAX_SPRITES] = {b1, b2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
int num_sprites = 2;
bool auto_scroll = true;
int scroll_speed = 3;
int current_pair = 0;

#define COLOR_BLACK 0
#define COLOR_WHITE 255
#define COLOR_BLUE 1

void draw_left_arrow(int x, int y) {
    gfx_SetColor(COLOR_WHITE);
    gfx_FillTriangle_NoClip(x, y, x + ARROW_SIZE, y + ARROW_SIZE / 2, x, y + ARROW_SIZE);
}

void draw_right_arrow(int x, int y) {
    gfx_SetColor(COLOR_WHITE);
    gfx_FillTriangle_NoClip(x + ARROW_SIZE, y, x, y + ARROW_SIZE / 2, x + ARROW_SIZE, y + ARROW_SIZE);
}

void draw_photo_border(int x, int y) {
    gfx_SetColor(COLOR_BLUE);
    gfx_Rectangle_NoClip(x - 2, y - 2, PHOTO_WIDTH + 4, PHOTO_HEIGHT + 4);
}

void fade_transition() {
    for (int i = 255; i >= 0; i -= 16) {
        gfx_SetTextFGColor(i);
        gfx_BlitBuffer();
        delay(20);
    }
    gfx_FillScreen(COLOR_BLACK);
    for (int i = 0; i <= 255; i += 16) {
        gfx_SetTextFGColor(i);
        gfx_BlitBuffer();
        delay(20);
    }
}

void display_pair() {
    int x1 = (320 - 2 * PHOTO_WIDTH - PADDING) / 2;
    int x2 = x1 + PHOTO_WIDTH + PADDING;
    int y = (240 - PHOTO_HEIGHT) / 2;

    gfx_FillScreen(COLOR_BLACK);

    draw_left_arrow(5, 110);
    draw_right_arrow(320 - ARROW_SIZE - 5, 110);

    if (current_pair < num_sprites) {
        gfx_TransparentSprite(sprites[current_pair], x1, y);
        draw_photo_border(x1, y);
    }
    if (current_pair + 1 < num_sprites) {
        gfx_TransparentSprite(sprites[current_pair + 1], x2, y);
        draw_photo_border(x2, y);
    }

    gfx_SetColor(COLOR_WHITE);
    gfx_HorizLine(0, 0, 320);
    gfx_PrintStringXY("Photo Viewer", 10, 5);

    char status[30];
    sprintf(status, "Photos %d-%d of %d", current_pair + 1, current_pair + 2, num_sprites);
    gfx_PrintStringXY(status, 200, 5);

    gfx_BlitBuffer();
}

int show_menu() {
    gfx_FillScreen(COLOR_BLACK);
    gfx_PrintStringXY("Menu:", 10, 10);

    char option_status[50];
    sprintf(option_status, "1: Toggle Auto-Scroll (Current: %s)", auto_scroll ? "On" : "Off");
    gfx_PrintStringXY(option_status, 10, 30);

    sprintf(option_status, "2: Change Speed (Current: %d sec)", scroll_speed);
    gfx_PrintStringXY(option_status, 10, 50);

    gfx_PrintStringXY("4: Exit", 10, 90);
    gfx_BlitBuffer();

    while (1) {
        kb_Scan();
        if (kb_Data[7] & kb_1) { auto_scroll = !auto_scroll; return 0; }
        if (kb_Data[7] & kb_2) { scroll_speed = (scroll_speed == 3) ? 5 : 3; return 0; }
        if (kb_Data[7] & kb_4) { return 1; }
        if (kb_Data[6] & kb_Clear) { return 0; }
    }
}

void main(void) {
    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(0);
    gfx_SetTextFGColor(COLOR_WHITE);
    gfx_SetDrawBuffer();

    rtc_Enable(RTC_ENABLE_CLOCK);

    uint32_t last_time = rtc_Time();

    display_pair();

    while (1) {
        kb_Scan();

        if (kb_Data[1] & kb_2nd) {
            if (show_menu()) break;
            display_pair();
        }

        if (auto_scroll) {
            uint32_t current_time = rtc_Time();
            if ((int)(current_time - last_time) >= scroll_speed) {
                fade_transition();
                current_pair = (current_pair + 2) % num_sprites;
                display_pair();
                last_time = current_time;
            }
        } else {
            if (kb_Data[7] & kb_Left) {
                fade_transition();
                current_pair = (current_pair - 2 + num_sprites) % num_sprites;
                display_pair();
            }
            if (kb_Data[7] & kb_Right) {
                fade_transition();
                current_pair = (current_pair + 2) % num_sprites;
                display_pair();
            }
        }

        if (kb_Data[6] & kb_Clear) break;
    }

    gfx_End();
}