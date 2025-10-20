#include <ti/getcsc.h>
#include <keypadc.h>    // For key scanning
#include <graphx.h>
#include <ti/realclock.h>  // For timers
#include <fileioc.h>    // For AppVars (optional loading)
#include "gfx/gfx.h" 
#include "f1.h"
#include "2.h"   // Your sprites

#define MAX_SPRITES 10  // Adjust based on your images
#define PHOTO_WIDTH 128 // Assume your photo size; adjust
#define PHOTO_HEIGHT 128
#define PADDING 10      // Space between photos
#define ARROW_SIZE 20   // Arrow triangle size

gfx_sprite_t *sprites[MAX_SPRITES] = {sprite_1, sprite_2, sprite_3, sprite_4, sprite_5 /* Add more */};
int num_sprites = 5;  // Starting number; can "add" more
bool auto_scroll = true;
int scroll_speed = 3;  // Seconds
int current_pair = 0;  // Index of first photo in pair (0 for 1-2, 2 for 3-4, etc.)

// Function to draw a left arrow (triangle)
void draw_left_arrow(int x, int y) {
    gfx_SetColor(gfx_white);
    gfx_FillTriangle_NoClip(x, y, x + ARROW_SIZE, y + ARROW_SIZE / 2, x, y + ARROW_SIZE);
}

// Function to draw a right arrow
void draw_right_arrow(int x, int y) {
    gfx_SetColor(gfx_white);
    gfx_FillTriangle_NoClip(x + ARROW_SIZE, y, x, y + ARROW_SIZE / 2, x + ARROW_SIZE, y + ARROW_SIZE);
}

// Function to draw borders around photos
void draw_photo_border(int x, int y) {
    gfx_SetColor(gfx_blue);  // Nice border color
    gfx_Rectangle_NoClip(x - 2, y - 2, PHOTO_WIDTH + 4, PHOTO_HEIGHT + 4);
}

// Simple fade out/in transition
void fade_transition() {
    for (int i = 255; i >= 0; i -= 16) {
        gfx_SetTextFGColor(i);  // Dim text too if needed
        gfx_BlitBuffer();
        delay(20);
    }
    gfx_FillScreen(gfx_black);
    for (int i = 0; i <= 255; i += 16) {
        gfx_SetTextFGColor(i);
        gfx_BlitBuffer();
        delay(20);
    }
}

// Display the two photos
void display_pair() {
    int x1 = (320 - 2 * PHOTO_WIDTH - PADDING) / 2;  // Center first photo
    int x2 = x1 + PHOTO_WIDTH + PADDING;
    int y = (240 - PHOTO_HEIGHT) / 2;  // Center vertically

    gfx_FillScreen(gfx_black);

    // Draw arrows
    draw_left_arrow(5, 110);  // Left side
    draw_right_arrow(320 - ARROW_SIZE - 5, 110);  // Right side

    // Draw photos if they exist
    if (current_pair < num_sprites) {
        gfx_TransparentSprite(sprites[current_pair], x1, y);
        draw_photo_border(x1, y);
    }
    if (current_pair + 1 < num_sprites) {
        gfx_TransparentSprite(sprites[current_pair + 1], x2, y);
        draw_photo_border(x2, y);
    }

    // Title bar and status
    gfx_SetColor(gfx_white);
    gfx_HorizLine(0, 0, 320);  // Top bar
    gfx_PrintStringXY("Photo Viewer", 10, 5);
    char status[20];
    sprintf(status, "Photos %d-%d of %d", current_pair + 1, current_pair + 2, num_sprites);
    gfx_PrintStringXY(status, 200, 5);

    gfx_BlitBuffer();
}

// Load additional sprite from AppVar (placeholder; assumes pre-saved sprite data)
void add_picture() {
    ti_var_t appvar = ti_Open("EXTRAPIC", "r");
    if (appvar) {
        if (num_sprites < MAX_SPRITES) {
            sprites[num_sprites] = gfx_MallocSprite(PHOTO_WIDTH, PHOTO_HEIGHT);
            ti_Read(sprites[num_sprites], PHOTO_WIDTH * PHOTO_HEIGHT, 1, appvar);
            num_sprites++;
        }
        ti_Close(appvar);
    }
    // In practice, save extra sprites as AppVars via another program or toolchain.
}

// Simple menu
int show_menu() {
    gfx_FillScreen(gfx_black);
    gfx_PrintStringXY("Menu:", 10, 10);
    gfx_PrintStringXY("1: Toggle Auto-Scroll (Current: %s)", 10, 30, auto_scroll ? "On" : "Off");
    gfx_PrintStringXY("2: Change Speed (Current: %d sec)", 10, 50, scroll_speed);
    gfx_PrintStringXY("3: Add Picture", 10, 70);
    gfx_PrintStringXY("4: Exit", 10, 90);
    gfx_BlitBuffer();

    while (1) {
        kb_Scan();
        if (kb_Data[7] == kb_1) { auto_scroll = !auto_scroll; return 0; }
        if (kb_Data[7] == kb_2) { scroll_speed = (scroll_speed == 3) ? 5 : 3; return 0; }  // Toggle 3/5 sec
        if (kb_Data[7] == kb_3) { add_picture(); return 0; }
        if (kb_Data[7] == kb_4) { return 1; }  // Exit
        if (kb_Data[6] == kb_Clear) { return 0; }  // Back
    }
}

void main(void) {
    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(0);
    gfx_SetTextFGColor(gfx_white);
    gfx_SetDrawBuffer();  // Double buffer for no flicker

    // Enable real-time clock if needed
    rtc_Enable(RTC_ENABLE_CLOCK);

    uint32_t last_time = rtc_Time();

    display_pair();

    while (1) {
        kb_Scan();

        // Check for menu (e.g., 2nd key)
        if (kb_Data[1] == kb_2nd) {
            if (show_menu()) break;
            display_pair();
        }

        if (auto_scroll) {
            uint32_t current_time = rtc_Time();
            if (current_time - last_time >= scroll_speed) {
                fade_transition();
                current_pair = (current_pair + 2) % num_sprites;  // Loop every 2
                display_pair();
                last_time = current_time;
            }
        } else {
            // Manual navigation
            if (kb_Data[7] == kb_Left) {
                fade_transition();
                current_pair = (current_pair - 2 + num_sprites) % num_sprites;  // Loop back
                display_pair();
            }
            if (kb_Data[7] == kb_Right) {
                fade_transition();
                current_pair = (current_pair + 2) % num_sprites;
                display_pair();
            }
        }

        if (kb_Data[6] == kb_Clear) break;  // Exit
    }

    gfx_End();
}