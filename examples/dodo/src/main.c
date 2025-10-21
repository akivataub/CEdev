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
#define PHOTO_WIDTH 128
#define PHOTO_HEIGHT 128
#define PADDING 10
#define ARROW_SIZE 12 

gfx_sprite_t *sprites[MAX_SPRITES] = {b1, b2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
int num_sprites = 2;
bool auto_scroll = true;
int scroll_speed = 3;
int current_pair = 0;

#define COLOR_BLACK 0
#define COLOR_WHITE 255
#define COLOR_BLUE 1
#define COLOR_RED 2 
#define COLOR_TRANSPARENT_GRAY 100 

void draw_left_arrow_overlay(int x, int y, int color) {
    gfx_SetColor(color);
    // Draw a small triangle pointing left inside the photo area
    gfx_FillTriangle_NoClip(x, y + ARROW_SIZE, x, y - ARROW_SIZE, x - ARROW_SIZE, y);
}

void draw_right_arrow_overlay(int x, int y, int color) {
    gfx_SetColor(color);
    // Draw a small triangle pointing right inside the photo area
    gfx_FillTriangle_NoClip(x, y - ARROW_SIZE, x, y + ARROW_SIZE, x + ARROW_SIZE, y);
}

void draw_photo_border(int x, int y) {
    gfx_SetColor(COLOR_BLUE);
    gfx_Rectangle_NoClip(x - 2, y - 2, PHOTO_WIDTH + 4, PHOTO_HEIGHT + 4);
}

void draw_status_bar(bool menu_active) {
    gfx_SetColor(COLOR_BLACK);
    gfx_FillRectangle_NoClip(0, 0, 320, 20); 

    gfx_SetTextFGColor(COLOR_WHITE);
    gfx_SetTextScale(1, 1);
    gfx_PrintStringXY("Photo Viewer", 5, 5);

    char status[30];
    sprintf(status, "Photo %d/%d", current_pair + 1, num_sprites); 
    gfx_PrintStringXY(status, 320 - gfx_GetStringWidth(status) - 5, 5);

    gfx_SetColor(COLOR_BLUE);
    gfx_HorizLine(0, 20, 320);

    if (menu_active) {
        gfx_SetTextFGColor(COLOR_RED);
        gfx_PrintStringXY("[2ND] MENU", 100, 5);
    }
}

void display_pair() {
    int x1 = (320 - 2 * PHOTO_WIDTH - PADDING) / 2;
    int x2 = x1 + PHOTO_WIDTH + PADDING;
    int y = (240 - PHOTO_HEIGHT) / 2;

    gfx_FillScreen(COLOR_BLACK);
    draw_status_bar(false);

    if (current_pair < num_sprites) {
        gfx_TransparentSprite(sprites[current_pair], x1, y);
        draw_photo_border(x1, y);
    }
    if (current_pair + 1 < num_sprites) {
        gfx_TransparentSprite(sprites[current_pair + 1], x2, y);
        draw_photo_border(x2, y);
    } else {
        gfx_SetTextFGColor(COLOR_WHITE);
        gfx_SetTextScale(2, 2);
        gfx_PrintStringXY("End of Album", x2 + 10, y + 50);
        gfx_SetTextScale(1, 1);
    }
    
    // Draw tiny navigation arrows on the pictures
    if (num_sprites > 2) {
        // Arrow for previous (left photo)
        draw_left_arrow_overlay(x1 + 10, y + PHOTO_HEIGHT / 2, COLOR_WHITE);
        // Arrow for next (right photo)
        draw_right_arrow_overlay(x2 + PHOTO_WIDTH - 10, y + PHOTO_HEIGHT / 2, COLOR_WHITE);
    }
    
    gfx_Blit(gfx_buffer);
}

void slide_transition(int direction) {
    int dx = direction * (PHOTO_WIDTH + PADDING); 

    for (int i = 0; i < PHOTO_WIDTH + PADDING; i += 16) {
        
        gfx_FillScreen(COLOR_BLACK);
        draw_status_bar(false);

        // Draw current/next pair
        if (current_pair < num_sprites) {
            gfx_TransparentSprite(sprites[current_pair], (320 - 2 * PHOTO_WIDTH - PADDING) / 2 + dx - i, (240 - PHOTO_HEIGHT) / 2);
        }
        if (current_pair + 1 < num_sprites) {
            gfx_TransparentSprite(sprites[current_pair + 1], (320 - 2 * PHOTO_WIDTH - PADDING) / 2 + PHOTO_WIDTH + PADDING + dx - i, (240 - PHOTO_HEIGHT) / 2);
        }

        // Draw previous/new pair coming in
        int new_pair = (current_pair - direction * 2 + num_sprites) % num_sprites;
        int new_x_offset = (320 - 2 * PHOTO_WIDTH - PADDING) / 2 - dx;

        if (new_pair < num_sprites) {
            gfx_TransparentSprite(sprites[new_pair], new_x_offset + i, (240 - PHOTO_HEIGHT) / 2);
        }
        if (new_pair + 1 < num_sprites) {
            gfx_TransparentSprite(sprites[new_pair + 1], new_x_offset + PHOTO_WIDTH + PADDING + i, (240 - PHOTO_HEIGHT) / 2);
        }

        gfx_Blit(gfx_buffer);
        delay(10);
    }
}

void scroll_photos(int direction) {
    int new_pair = (current_pair + direction * 2 + num_sprites) % num_sprites;
    
    // Perform a slide transition if the new pair index is different
    if (new_pair != current_pair) {
        // Slide right (direction 1) if going to a higher index, otherwise slide left (direction -1)
        slide_transition(direction); 
        current_pair = new_pair;
        // Final display to clean up
        display_pair();
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

    gfx_SetTextFGColor(COLOR_BLUE);
    gfx_PrintStringXY("EXIT", 10, 110);
    
    gfx_SetTextFGColor(COLOR_RED);
    gfx_PrintStringXY("Press CLEAR to return", 10, 200);

    while (1) {
        kb_Scan();
        if (kb_Data[7] & kb_1) { auto_scroll = !auto_scroll; return 0; }
        if (kb_Data[7] & kb_2) { scroll_speed = (scroll_speed == 3) ? 5 : 3; return 0; }
        if (kb_Data[6] & kb_Clear) { return 0; }
        if (kb_Data[7] & kb_4) { return 1; }
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

    display_pair();

    while (1) {
        kb_Scan();

        if (kb_Data[1] & kb_2nd) {
            if (show_menu()) break;
            gfx_SetDrawBuffer(); 
            display_pair();
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