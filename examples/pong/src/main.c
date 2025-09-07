#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#define paddle_width 4
#define paddle_height 20
#define paddle_speed 3
#define SCREEN_WIDTH 320 //x
#define SCREEN_HEIGHT 240 //y

int ball_x, ball_y;
int ball_vel_x, ball_vel_y;
#define ball_size 4

int paddle_left_y = 120;
int paddle_right_y = 120;
    
int scroefp = 0;
int scorefa = 0;

int paddlex = 0;
int paddley = 160;


char paddleystring[4];
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

int main(void) {
    ball_x = 1;
    ball_y = 1;
    ball_vel_x = 2; // Start moving right
    ball_vel_y = 2; // Slight vertical movement

    gfx_Begin();
    // --- THE GAME LOOP ---
    itoa(paddley, paddleystring);
    while (!kb_IsDown(kb_KeyClear)) { // Run until the 'Clear' key is pressed
        kb_Scan();
        bool balldrawn = false;
        // 4. Read user input
            gfx_SetColor(0);
            if (kb_IsDown(kb_KeyDown)) {
                if (paddley == 220)
                {
                    
                } else {
                paddley = paddley + 5;
                }
            }
            else if (kb_IsDown(kb_KeyUp)) {
                if (paddley <= 0)
                {
                    
                } else {
                paddley = paddley - 5;
                }
            }

        // 5. Update game logic (move paddles, move ball, check for collisions)
        
        // 6. Draw everything to the buffer // Clear screen with black (color 0)


        // Draw the net (a dashed line in the middle)
        // Draw the paddles
        // Draw the ball
        // Draw the scores
        if (ball_x == (paddlex + 4)) {
            ball_vel_x = ball_vel_x - ball_vel_x - ball_vel_x;
        }

        if (ball_y >= 238 || ball_y <= -2) {
            ball_vel_y = ball_vel_y - ball_vel_y - ball_vel_y;
        }
        if (ball_x >= 318 || ball_x <= 0) {
            ball_vel_x = ball_vel_x - ball_vel_x - ball_vel_x;
        }

        if (balldrawn == false) {
            ball_x += ball_vel_x;
            ball_y += ball_vel_y;
            balldrawn = true;
        }

        //hi3
        // 7. Swap the buffer to the screen
        gfx_FillScreen(255);
        char paddlexs [4];
        char bys [4];
        char byx [4];
        itoa(paddlex, paddlexs);
        itoa(ball_y, bys);
        itoa(ball_x, byx);
        gfx_PrintStringXY(bys ,20, 20);
        gfx_PrintStringXY(byx ,20, 40);
        gfx_PrintStringXY(paddlexs ,20, 60);
        gfx_FillCircle(ball_x, ball_y, 2);
        gfx_FillRectangle(paddlex, paddley, paddle_width, paddle_height);
        gfx_SwapDraw();
        delay(20);
        // 8. A small delay to control game speed // Adjust this to make the game faster or slower
    }
            

    // 9. End graphics mode
    gfx_End();

    return 0;
}