#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <sys/rtc.h>
#include "gfx.h"
#include "b1.h"
#include "b2.h"

#define MAX_SPRITES 10
#define MAX_PHOTO_WIDTH 155
#define MAX_PHOTO_HEIGHT 105
#define PADDING 10
#define ARROW_SIZE 12 

gfx_sprite_t *sprites[MAX_SPRITES] = {b1, b2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
int num_sprites = 2;
bool auto_scroll = true;
int scroll_speed = 3;
int current_index = 0;
bool is_horizontal = true;
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
    gfx_FillRectangle_NoClip(0, 0, 320, 20); 

    gfx_SetTextFGColor(COLOR_WHITE);
    gfx_SetTextScale(1, 1);
    gfx_PrintStringXY("Photo Viewer", 5, 5);

    char status[30];
    sprintf(status, "Photo %d/%d", current_index + 1, num_sprites); 
    gfx_PrintStringXY(status, 320 - gfx_GetStringWidth(status) - 5, 5);

    gfx_SetColor(COLOR_BLUE);
    gfx_HorizLine(0, 20, 320);

    if (menu_active) {
        gfx_SetTextFGColor(COLOR_RED);
        gfx_PrintStringXY("[2ND] MENU", 100, 5);
    }
}

void display_current() {
    int photo_area_height = 240 - 20;

    gfx_FillScreen(COLOR_BLACK);
    draw_status_bar(false);

    if (view_mode == VIEW_SINGLE) {
        if (current_index < num_sprites) {
            gfx_sprite_t *spr = sprites[current_index];
            int w = spr->width;
            int h = spr->height;
            int scale_x = 320 / w;
            int scale_y = photo_area_height / h;
            int scale = (scale_x < scale_y ? scale_x : scale_y);
            if (scale < 1) scale = 1;
            scale = (scale > 255 ? 255 : scale);
            int drawn_w = w * scale;
            int drawn_h = h * scale;
            int x = (320 - drawn_w) / 2;
            int y = 20 + (photo_area_height - drawn_h) / 2;
            gfx_ScaledTransparentSprite_NoClip(spr, x, y, scale, scale);
            draw_photo_border(x, y, drawn_w, drawn_h);
        }
    } else {
        gfx_sprite_t *spr1 = (current_index < num_sprites) ? sprites[current_index] : NULL;
        gfx_sprite_t *spr2 = (current_index + 1 < num_sprites) ? sprites[current_index + 1] : NULL;
        int w1 = spr1 ? spr1->width : 0;
        int h1 = spr1 ? spr1->height : 0;
        int w2 = spr2 ? spr2->width : 0;
        int h2 = spr2 ? spr2->height : 0;

        int x1, y1, x2, y2;
        if (is_horizontal) {
            int total_w = w1 + w2 + PADDING;
            int max_h = (h1 > h2 ? h1 : h2);
            x1 = (320 - total_w) / 2;
            y1 = 20 + (photo_area_height - max_h) / 2;
            x2 = x1 + w1 + PADDING;
            y2 = y1;
        } else {
            int max_w = (w1 > w2 ? w1 : w2);
            int total_h = h1 + h2 + PADDING;
            x1 = (320 - max_w) / 2;
            y1 = 20 + (photo_area_height - total_h) / 2;
            x2 = x1;
            y2 = y1 + h1 + PADDING;
        }

        if (spr1) {
            gfx_TransparentSprite(spr1, x1, y1);
            draw_photo_border(x1, y1, w1, h1);
        }
        if (spr2) {
            gfx_TransparentSprite(spr2, x2, y2);
            draw_photo_border(x2, y2, w2, h2);
        } else if (spr1) {
            gfx_SetTextFGColor(COLOR_WHITE);
            gfx_SetTextScale(2, 2);
            gfx_PrintStringXY("End of Album", x2 + 10, y2 + 50);
            gfx_SetTextScale(1, 1);
        }
    }
    
    gfx_Blit(gfx_buffer);
}

void slide_transition(int direction) {
    int step = (view_mode == VIEW_PAIR ? 2 : 1);
    int new_index = (current_index - direction * step + num_sprites) % num_sprites;

    int dx = 0;
    int dy = 0;
    int slide_size = 0;
    if (view_mode == VIEW_SINGLE || is_horizontal) {
        dx = direction * (MAX_PHOTO_WIDTH + PADDING);
        slide_size = MAX_PHOTO_WIDTH + PADDING;
    } else {
        dy = direction * (MAX_PHOTO_HEIGHT + PADDING);
        slide_size = MAX_PHOTO_HEIGHT + PADDING;
    }

    for (int i = 0; i < slide_size; i += 16) {
        
        gfx_FillScreen(COLOR_BLACK);
        draw_status_bar(false);

        // Draw current
        gfx_sprite_t *spr1 = (current_index < num_sprites) ? sprites[current_index] : NULL;
        gfx_sprite_t *spr2 = (view_mode == VIEW_PAIR && current_index + 1 < num_sprites) ? sprites[current_index + 1] : NULL;
        int w1 = spr1 ? spr1->width : 0;
        int h1 = spr1 ? spr1->height : 0;
        int w2 = spr2 ? spr2->width : 0;
        int h2 = spr2 ? spr2->height : 0;

        int x1, y1, x2, y2;
        if (view_mode == VIEW_SINGLE) {
            int scale = min(320 / w1, (240 - 20) / h1);
            scale = min(255, scale > 1 ? scale : 1);
            int drawn_w = w1 * scale;
            int drawn_h = h1 * scale;
            x1 = (320 - drawn_w) / 2 + dx - i;
            y1 = 20 + ((240 - 20) - drawn_h) / 2 + dy - i;
            if (spr1) gfx_ScaledTransparentSprite_NoClip(spr1, x1, y1, scale, scale);
        } else {
            if (is_horizontal) {
                int total_w = w1 + w2 + PADDING;
                int max_h = max(h1, h2);
                x1 = (320 - total_w) / 2 + dx - i;
                y1 = 20 + ((240 - 20) - max_h) / 2;
                x2 = x1 + w1 + PADDING;
                y2 = y1;
            } else {
                int max_w = max(w1, w2);
                int total_h = h1 + h2 + PADDING;
                x1 = (320 - max_w) / 2;
                y1 = 20 + ((240 - 20) - total_h) / 2 + dy - i;
                x2 = x1;
                y2 = y1 + h1 + PADDING;
            }
            if (spr1) gfx_TransparentSprite(spr1, x1, y1);
            if (spr2) gfx_TransparentSprite(spr2, x2, y2);
        }

        // Draw new
        gfx_sprite_t *newspr1 = (new_index < num_sprites) ? sprites[new_index] : NULL;
        gfx_sprite_t *newspr2 = (view_mode == VIEW_PAIR && new_index + 1 < num_sprites) ? sprites[new_index + 1] : NULL;
        int nw1 = newspr1 ? newspr1->width : 0;
        int nh1 = newspr1 ? newspr1->height : 0;
        int nw2 = newspr2 ? newspr2->width : 0;
        int nh2 = newspr2 ? newspr2->height : 0;

        int nx1, ny1, nx2, ny2;
        if (view_mode == VIEW_SINGLE) {
            int nscale = min(320 / nw1, (240 - 20) / nh1);
            nscale = min(255, nscale > 1 ? nscale : 1);
            int ndrawn_w = nw1 * nscale;
            int ndrawn_h = nh1 * nscale;
            nx1 = (320 - ndrawn_w) / 2 - dx + i;
            ny1 = 20 + ((240 - 20) - ndrawn_h) / 2 - dy + i;
            if (newspr1) gfx_ScaledTransparentSprite_NoClip(newspr1, nx1, ny1, nscale, nscale);
        } else {
            if (is_horizontal) {
                int ntotal_w = nw1 + nw2 + PADDING;
                int nmax_h = max(nh1, nh2);
                nx1 = (320 - ntotal_w) / 2 - dx + i;
                ny1 = 20 + ((240 - 20) - nmax_h) / 2;
                nx2 = nx1 + nw1 + PADDING;
                ny2 = ny1;
            } else {
                int nmax_w = max(nw1, nw2);
                int ntotal_h = nh1 + nh2 + PADDING;
                nx1 = (320 - nmax_w) / 2;
                ny1 = 20 + ((240 - 20) - ntotal_h) / 2 - dy + i;
                nx2 = nx1;
                ny2 = ny1 + nh1 + PADDING;
            }
            if (newspr1) gfx_TransparentSprite(newspr1, nx1, ny1);
            if (newspr2) gfx_TransparentSprite(newspr2, nx2, ny2);
        }

        gfx_Blit(gfx_buffer);
        os_Wait(10);
    }
}

void scroll_photos(int direction) {
    int step = (view_mode == VIEW_PAIR ? 2 : 1);
    int new_index = (current_index + direction * step + num_sprites) % num_sprites;
    
    if (new_index != current_index) {
        slide_transition(direction);
        current_index = new_index;
        display_current();
    }
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