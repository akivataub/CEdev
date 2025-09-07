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

int paddle_left_y = 120;
int paddle_right_y = 120;
    
int score_left_person = 0;
int score_right_person = 0;
int lastscorelp = 0;
int lastscorerp = 0;

int paddlex = 2;
int paddley = 160;

//bool ifhit = false; TEST
//char paddleystring[4]; TEST

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
    startgame:
    
    ball_x = 10;
    ball_y = 10;
    ball_vel_x = 2; // Start moving right
    ball_vel_y = 2; // Slight vertical movement

    gfx_Begin();
    //itoa(paddley, paddleystring); TEST
    while (!kb_IsDown(kb_KeyClear)) {
        kb_Scan();
        bool balldrawn = false;
            gfx_SetColor(0);
            if (kb_IsDown(kb_KeyDown)) {
                if (paddley == 220)
                {
                    
                } else {
                paddley = paddley + 3;
                }
            }
            else if (kb_IsDown(kb_KeyUp)) {
                if (paddley <= 0)
                {
                    
                } else {
                paddley = paddley - 3;
                }
            }

        // 5. Update game logic (move paddles, move ball, check for collisions)
        
        // 6. Draw everything to the buffer // Clear screen with black (color 0)


        // Draw the net (a dashed line in the middle)
        // Draw the paddles
        // Draw the ball
        // Draw the scores
        //paddley + 20 paddlex + 4
        if (ball_x >= paddlex &&  ball_x <= paddlex + paddle_width && ball_y >= paddley && ball_y <= paddley + paddle_height) {
            ball_vel_x = ball_vel_x - ball_vel_x - ball_vel_x;
            //ifhit = true; TEST
        }

        if (ball_y >= 238 || ball_y <= 0) {
            ball_vel_y = ball_vel_y - ball_vel_y - ball_vel_y;
        }
        if (ball_x >= 318) {
            ball_vel_x = ball_vel_x - ball_vel_x - ball_vel_x;
        }

        if (ball_x <= 2) {
            lastscorelp = score_left_person;
            score_left_person++;
            if (score_left_person > lastscorelp) {
            gfx_FillScreen(255);
            gfx_SetTextScale(2, 2);
            gfx_PrintStringXY("Right Scores!", 20, 30);
            gfx_SetTextScale(1, 1);
            delay(1000);
            goto startgame;
            }
        }

        if (balldrawn == false) {
            ball_x += ball_vel_x;
            ball_y += ball_vel_y;
            balldrawn = true;
        }

        // 7. Swap the buffer to the screen
        gfx_FillScreen(255);
        //if (ifhit == true) { TEST
            //gfx_PrintStringXY("HIT", 20, 80); TEST
        //} TEST
        //char paddlexs [4]; TEST
        //char bys [4]; TEST
        //char byx [4]; TEST
        //itoa(paddlex, paddlexs); TEST
        //itoa(ball_y, bys); TEST
        //itoa(ball_x, byx); TEST
        //gfx_PrintStringXY(bys ,20, 20); TEST
        //gfx_PrintStringXY(byx ,20, 40); TEST
        //gfx_PrintStringXY(paddlexs ,20, 60); TEST

        //scores
        char slps[2];
        itoa(score_left_person, slps);
        gfx_PrintStringXY(slps ,20 ,40);
        

        //drawing ball and paddle
        gfx_FillCircle(ball_x, ball_y, 2);
        gfx_FillRectangle(paddlex, paddley, paddle_width, paddle_height);
        gfx_SwapDraw();
        delay(20);
    }
            

    gfx_End();

    return 0;
}