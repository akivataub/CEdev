#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#define paddle_width 4
#define paddle_height 20
#define paddle_speed 3

#define SCREEN_WIDTH 320 //x
#define SCREEN_HEIGHT 240 //y
//hi2
int ball_x, ball_y;
int ball_vel_x, ball_vel_y;
    
int score_left_person = 0;
int score_right_person = 0;
int lastscorelp = 0;
int lastscorerp = 0;

//first paddle
int paddlex = 2;
int paddley = 160;
//second paddle
int paddle2x = 312;
int paddle2y = 160;

int menuitem = 1;

bool endg = false;

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
    gfx_Begin();
    gfx_FillScreen(255);
    gfx_PrintStringXY("Play Again?", 20, 10);
    gfx_PrintStringXY("> Yes", 30, 20);
    gfx_PrintStringXY("  No", 30, 30);
    while (1) {
        kb_Scan();
            if (kb_IsDown(kb_KeyDown)) {
                menuitem = 2;
                delay(150);
            } else if (kb_IsDown(kb_KeyUp)) {
                menuitem = 1;
                delay(150);
            }
        if (menuitem == 1) {
            gfx_FillScreen(255);
            gfx_PrintStringXY("Play Again?", 20, 10);
            gfx_PrintStringXY("> Yes", 30, 20);
            gfx_PrintStringXY("  No", 30, 30);
            gfx_SwapDraw();
        }
        if (menuitem == 2) {
            gfx_FillScreen(255);
            gfx_PrintStringXY("Play Again?", 20, 10);
            gfx_PrintStringXY("  Yes", 30, 20);
            gfx_PrintStringXY("> No", 30, 30);
            gfx_SwapDraw();
        }
        if (menuitem == 3) {
            menuitem = 1;
        }
        if (menuitem == 0) {
            menuitem = 2;
        }
        if (menuitem == 1 && kb_IsDown(kb_KeyEnter)) {
            gfx_FillScreen(255);
            score_left_person = 0;
            score_right_person = 0;
            lastscorelp = 0;
            lastscorerp = 0;
            break;
        } else if (menuitem == 2 && kb_IsDown(kb_KeyEnter)) {
            gfx_FillScreen(255);
            goto ending;
        }
    } // while (1) loop ending

    startgame:

    endg = false;

    ball_x = 10;
    ball_y = 10;
    ball_vel_x = 2; // Start moving right
    ball_vel_y = 2; // Slight vertical movement

    //itoa(paddley, paddleystring); TEST1
    while (!kb_IsDown(kb_KeyClear)) {
        kb_Scan();
        bool balldrawn = false;
        gfx_SetColor(0);
            if (kb_IsDown(kb_Key2nd)) {
                if (paddley == 220)
                {
                    
                } else {
                    paddley = paddley - 3;
                }
            }
            else if (kb_IsDown(kb_KeyAlpha)) {
                if (paddley == 0)
                {
                    
                } else {
                    paddley = paddley + 3;
                }
            }
            // second paddle
            if (kb_IsDown(kb_KeyDown)) {
                if (paddle2y == 220)
                {

                } else {
                    paddle2y = paddle2y + 3;
                }
            }
            else if (kb_IsDown(kb_KeyUp)) {
                if (paddle2y == 0)
                {

                } else {
                    paddle2y = paddle2y - 3;
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
        if (ball_x >= paddle2x &&  ball_x <= paddle2x + paddle_width && ball_y >= paddle2y && ball_y <= paddle2y + paddle_height) {
            ball_vel_x = ball_vel_x - ball_vel_x - ball_vel_x;
            //ifhit = true; TEST
        }

        if (ball_y >= 238 || ball_y <= 0) {
            ball_vel_y = ball_vel_y - ball_vel_y - ball_vel_y;
        }
        if (ball_x >= 318) {
            lastscorerp = score_right_person;
            score_right_person++;
            if (score_right_person == 5) {
                    gfx_FillScreen(255);
                    gfx_SetTextScale(2, 2);
                    gfx_PrintStringXY("P1 Wins!", 20, 30);
                    gfx_SetTextScale(1, 1);
                    gfx_SwapDraw();
                    delay(1500);
                    endg = true;
                    goto endgame;
            } else if (score_right_person > lastscorerp) {
                    gfx_FillScreen(255);
                    gfx_SetTextScale(2, 2);
                    gfx_PrintStringXY("P1 Scores!", 20, 30);
                    gfx_SetTextScale(1, 1);
                    gfx_SwapDraw();
                    delay(1500);
                    goto startgame;
            }
        }

        if (ball_x <= 2) {
            lastscorelp = score_left_person;
            score_left_person++;
            if (score_left_person == 5) {
                    gfx_FillScreen(255);
                    gfx_SetTextScale(2, 2);
                    gfx_PrintStringXY("P2 Wins!", 20, 30);
                    gfx_SetTextScale(1, 1);
                    gfx_SwapDraw();
                    delay(1500);
                    endg = true;
                    goto endgame;
            } else if (score_left_person > lastscorelp) {
                    gfx_FillScreen(255);
                    gfx_SetTextScale(2, 2);
                    gfx_PrintStringXY("P2 Scores!", 20, 30);
                    gfx_SetTextScale(1, 1);
                    gfx_SwapDraw();
                    delay(1500);
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
        itoa(score_right_person, slps);
        gfx_PrintStringXY(slps ,20 ,5);
        gfx_PrintStringXY("P1:" ,5 ,5);
        char srps[2];
        itoa(score_left_person, srps);
        gfx_PrintStringXY(srps ,300 ,5);
        gfx_PrintStringXY("P2:" ,285 ,5);

        
        

        //drawing ball and paddle
        gfx_FillCircle(ball_x, ball_y, 2);
        gfx_FillRectangle(paddlex, paddley, paddle_width, paddle_height);
        gfx_FillRectangle(paddle2x, paddle2y, paddle_width, paddle_height);

        endgame:
        if (endg == true) {
            gfx_FillScreen(255);
            gfx_PrintStringXY("Play Again?", 20, 10);
            gfx_PrintStringXY("> Yes", 30, 20);
            gfx_PrintStringXY("  No", 30, 30);
            while (1) {
                kb_Scan();
                    if (kb_IsDown(kb_KeyDown)) {
                        menuitem = 2;
                        delay(150);
                    } else if (kb_IsDown(kb_KeyUp)) {
                        menuitem = 1;
                        delay(150);
                    }
                if (menuitem == 1) {
                    gfx_FillScreen(255);
                    gfx_PrintStringXY("Play Again?", 20, 10);
                    gfx_PrintStringXY("> Yes", 30, 20);
                    gfx_PrintStringXY("  No", 30, 30);
                    gfx_SwapDraw();
                }
                if (menuitem == 2) {
                    gfx_FillScreen(255);
                    gfx_PrintStringXY("Play Again?", 20, 10);
                    gfx_PrintStringXY("  Yes", 30, 20);
                    gfx_PrintStringXY("> No", 30, 30);
                    gfx_SwapDraw();
                }
                if (menuitem == 3) {
                    menuitem = 1;
                }
                if (menuitem == 0) {
                    menuitem = 2;
                }
                if (menuitem == 1 && kb_IsDown(kb_KeyEnter)) {
                    gfx_FillScreen(255);
                    score_left_person = 0;
                    score_right_person = 0;
                    lastscorelp = 0;
                    lastscorerp = 0;
                    break;
                } else if (menuitem == 2 && kb_IsDown(kb_KeyEnter)) {
                    gfx_FillScreen(255);
                    goto ending;
                }
            } // while (1) loop ending
        } // endg = true ending
        gfx_SwapDraw();
        delay(20);
    } // game loop end      
    ending:
    gfx_End();
    return 0;
} //full loop end
