#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <sys/rtc.h>
#include "gfx.h"

#define MAX_SPRITES 10
#define PADDING 10
#define STATUS_BAR_H 20
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

gfx_sprite_t *sprites[MAX_SPRITES] = { b1, b2, b3, b4, b5, b6, my_sprites_b7, my_sprites_b8, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
int num_sprites = 8;
bool auto_scroll = true;
int scroll_speed = 3;
int current_index = 0;
bool is_horizontal = false;
int view_mode = 0; // 0: pair, 1: single

#define VIEW_PAIR 0
#define VIEW_SINGLE 1

#define COLOR_BLACK 0
#define COLOR_WHITE 255
#define COLOR_BLUE 1
#define COLOR_RED 2 
#define COLOR_TRANSPARENT_GRAY 100 

void draw_photo_border(int x, int y, int w, int h) {
    gfx_SetColor(COLOR_BLUE);
    gfx_Rectangle_NoClip(x - 2, y - 2, w + 4, h + 4);
}

void draw_status_bar(bool menu_active) {
    gfx_SetColor(COLOR_BLACK);
    gfx_FillRectangle_NoClip(0, 0, SCREEN_WIDTH, STATUS_BAR_H); 

    gfx_SetTextFGColor(COLOR_WHITE);
    gfx_SetTextScale(1, 1);
    gfx_PrintStringXY("Photo Viewer", 5, 5);

    char status[30];
    sprintf(status, "Photo %d/%d", current_index + 1, num_sprites); 
    gfx_PrintStringXY(status, SCREEN_WIDTH - gfx_GetStringWidth(status) - 5, 5);

    gfx_SetColor(COLOR_BLUE);
    gfx_HorizLine(0, STATUS_BAR_H, SCREEN_WIDTH);

    if (menu_active) {
        gfx_SetTextFGColor(COLOR_RED);
        gfx_PrintStringXY("[2ND] MENU", 100, 5);
    }
}

void display_current() {
    int photo_area_height = SCREEN_HEIGHT - STATUS_BAR_H;

    gfx_FillScreen(COLOR_BLACK);
    draw_status_bar(false);

    if (view_mode == VIEW_SINGLE) {
        if (current_index < num_sprites) {
            gfx_sprite_t *spr = sprites[current_index];
            int w = spr->width;
            int h = spr->height;
            uint8_t scale = min(SCREEN_WIDTH / w, photo_area_height / h);
            scale = (scale > 255 ? 255 : scale);
            if (scale < 1) scale = 1;
            int drawn_w = w * scale;
            int drawn_h = h * scale;
            int x = (SCREEN_WIDTH - drawn_w) / 2;
            int y = STATUS_BAR_H + (photo_area_height - drawn_h) / 2;
            gfx_ScaledTransparentSprite_NoClip(spr, x, y, scale, scale);
            draw_photo_border(x, y, drawn_w, drawn_h);
        }
    } else {
        int slot_w1, slot_h1, slot_x1, slot_y1, slot_w2, slot_h2, slot_x2, slot_y2;
        if (is_horizontal) {
            slot_w1 = slot_w2 = (SCREEN_WIDTH - PADDING) / 2;
            slot_h1 = slot_h2 = photo_area_height;
            slot_x1 = 0;
            slot_y1 = STATUS_BAR_H;
            slot_x2 = slot_x1 + slot_w1 + PADDING;
            slot_y2 = slot_y1;
        } else {
            slot_w1 = slot_w2 = SCREEN_WIDTH;
            slot_h1 = slot_h2 = (photo_area_height - PADDING) / 2;
            slot_x1 = 0;
            slot_y1 = STATUS_BAR_H;
            slot_x2 = slot_x1;
            slot_y2 = slot_y1 + slot_h1 + PADDING;
        }

        gfx_sprite_t *spr1 = (current_index < num_sprites) ? sprites[current_index] : NULL;
        gfx_sprite_t *spr2 = (current_index + 1 < num_sprites) ? sprites[current_index + 1] : NULL;

        if (spr1) {
            int w = spr1->width;
            int h = spr1->height;
            uint8_t scale = min(slot_w1 / w, slot_h1 / h);
            scale = (scale > 255 ? 255 : scale);
            if (scale < 1) scale = 1;
            int drawn_w = w * scale;
            int drawn_h = h * scale;
            int x = slot_x1 + (slot_w1 - drawn_w) / 2;
            int y = slot_y1 + (slot_h1 - drawn_h) / 2;
            gfx_ScaledTransparentSprite_NoClip(spr1, x, y, scale, scale);
            draw_photo_border(x, y, drawn_w, drawn_h);
        }

        if (spr2) {
            int w = spr2->width;
            int h = spr2->height;
            uint8_t scale = min(slot_w2 / w, slot_h2 / h);
            scale = (scale > 255 ? 255 : scale);
            if (scale < 1) scale = 1;
            int drawn_w = w * scale;
            int drawn_h = h * scale;
            int x = slot_x2 + (slot_w2 - drawn_w) / 2;
            int y = slot_y2 + (slot_h2 - drawn_h) / 2;
            gfx_ScaledTransparentSprite_NoClip(spr2, x, y, scale, scale);
            draw_photo_border(x, y, drawn_w, drawn_h);
        } else if (spr1) {
            gfx_SetTextFGColor(COLOR_WHITE);
            gfx_SetTextScale(2, 2);
            gfx_PrintStringXY("End of Album", slot_x2 + 10, slot_y2 + 50);
            gfx_SetTextScale(1, 1);
        }
    }
    
    gfx_Blit(gfx_buffer);
}

void scroll_photos(int direction) {
    int step = (view_mode == VIEW_PAIR ? 2 : 1);
    current_index = (current_index + direction * step + num_sprites) % num_sprites;
    display_current();
}

int show_menu() {
    gfx_SetDrawScreen(); 
    gfx_FillScreen(COLOR_BLACK);
    
    gfx_SetTextFGColor(COLOR_WHITE);
    gfx_SetTextScale(2, 2); 
    gfx_PrintStringXY("SETTINGS", 10, 10);
    
    gfx_SetTextScale(1, 1);
    char option_status[50];
    
    sprintf(option_status, "1: Auto-Scroll (%s)", auto_scroll ? "ON" : "OFF");
    gfx_PrintStringXY(option_status, 10, 50);

    sprintf(option_status, "2: Speed (%d sec)", scroll_speed);
    gfx_PrintStringXY(option_status, 10, 70);

    sprintf(option_status, "3: View Mode (%s)", view_mode == VIEW_SINGLE ? "Single (Zoom)" : "Pair");
    gfx_PrintStringXY(option_status, 10, 90);

    sprintf(option_status, "4: Layout (%s)", is_horizontal ? "Horizontal" : "Vertical");
    gfx_PrintStringXY(option_status, 10, 110);

    gfx_SetTextFGColor(COLOR_BLUE);
    gfx_PrintStringXY("5: EXIT", 10, 130);
    
    gfx_SetTextFGColor(COLOR_RED);
    gfx_PrintStringXY("Press CLEAR to return", 10, 200);

    while (1) {
        kb_Scan();
        if (kb_Data[7] & kb_1) { auto_scroll = !auto_scroll; return 0; }
        if (kb_Data[7] & kb_2) { scroll_speed = (scroll_speed == 3) ? 5 : 3; return 0; }
        if (kb_Data[7] & kb_3) { view_mode = (view_mode == VIEW_SINGLE ? VIEW_PAIR : VIEW_SINGLE); return 0; }
        if (kb_Data[7] & kb_4) { is_horizontal = !is_horizontal; return 0; }
        if (kb_Data[7] & kb_5) { return 1; }
        if (kb_Data[6] & kb_Clear) { return 0; }
    }
}

int main(void) {
    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(0);
    gfx_SetTextFGColor(COLOR_WHITE);
    gfx_SetDrawBuffer(); 

    rtc_Enable(RTC_INT_MASK);

    uint32_t last_time = rtc_Time();

    display_current();

    while (1) {
        kb_Scan();

        if (kb_Data[1] & kb_2nd) {
            if (show_menu()) break;
            gfx_SetDrawBuffer(); 
            display_current();
        }

        if (auto_scroll) {
            uint32_t current_time = rtc_Time();
            if ((int)(current_time - last_time) >= scroll_speed) {
                scroll_photos(1); // Scroll forward
                last_time = current_time;
            }
        } else {
            // Manual navigation logic
            if (kb_Data[7] & kb_Left) {
                scroll_photos(-1); // Scroll backward
                while(kb_Data[7] & kb_Left) kb_Scan(); 
            }
            if (kb_Data[7] & kb_Right) {
                scroll_photos(1); // Scroll forward
                while(kb_Data[7] & kb_Right) kb_Scan(); 
            }
        }

        if (kb_Data[6] & kb_Clear) break;
    }

    gfx_End();
    return 0;
}