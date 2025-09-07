#include <tice.h>
#include <graphx.h>
#include <keypadc.h>

int guess = 0;
int digits = 0;

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

// Main function
int main(void) {
    // Initialize graphics
    gfx_Begin();
    // Set background color to white
    gfx_FillScreen(255);

    // Set text color to black
    gfx_SetTextFGColor(0);

    // Print a message
    gfx_PrintStringXY("Hello, User Welcome To RNG!", 50, 100);
    gfx_PrintStringXY("Stands For Random Number Guesser", 50, 120);
    delay(2000);
    gfx_FillScreen(255);
    gfx_PrintStringXY("Press any key to generate a random number", 50, 140);
    int n = randInt(1, 100);
    while (1) {
        uint8_t key = os_GetCSC();
start_guess:
        gfx_FillScreen(255);
        gfx_PrintStringXY("guess your number!!:", 50, 100);
        char buf[8];
        itoa(n, buf);
        gfx_PrintStringXY("your last number was ", 50, 120);
        gfx_PrintStringXY(buf, 52, 120);
        delay(500);
        gfx_FillScreen(255);
        while (!os_GetCSC()) {
            if (key == sk_1) {
                guess = guess * 10 + 1;
                digits = digits + 1;
            }
            if (key == sk_2) {
                guess = guess * 10 + 2;
                digits = digits + 1;
            }
            if (key == sk_3) {
                guess = guess * 10 + 3;
                digits = digits + 1;
            }
            if (key == sk_4) {
                guess = guess * 10 + 4;
                digits = digits + 1;
            }
            if (key == sk_5) {
                guess = guess * 10 + 5;
                digits = digits + 1;
            }
            if (key == sk_6) {
                guess = guess * 10 + 6;
                digits = digits + 1;
            }
            if (key == sk_7) {
                guess = guess * 10 + 7;
                digits = digits + 1;
            }
            if (key == sk_8) {
                guess = guess * 10 + 8;
                digits = digits + 1;
            }
            if (key == sk_9) {
                guess = guess * 10 + 9;
                digits = digits + 1;
            }
            if (key == sk_Del) {
                guess = guess /10;
            }
            if (key == sk_0) {
                guess = guess * 10 + 0;
                digits = digits + 1;
            }
            if (key == sk_Enter) {
                if (digits == 0) {
                    gfx_FillScreen(255);
                    gfx_PrintStringXY("put in a number first!", 50, 160);
                }
                if (guess < n) {
                    gfx_FillScreen(255);
                    gfx_PrintStringXY("Too Low!", 50, 160);
                    delay(2000);
                }
                if (guess > n) {
                    gfx_FillScreen(255);
                    gfx_PrintStringXY("Too High!", 50, 160);
                    delay(2000);
                }
                if (guess == n) {
                    gfx_FillScreen(255);
                    gfx_PrintStringXY("You Got It!!", 50, 160);
                    delay(2000);
                    gfx_PrintStringXY("Press 2nd to play again", 50, 180);
                    gfx_PrintStringXY("Or press mode to exit", 50, 200);
                    while (1) {
                        if (key == sk_2nd) {
                            guess = 0;
                            goto start_guess;
                        }
                        if (key == sk_Mode) {
                            break;
                        }
        }       }   }
    }

    // End graphics
    gfx_End();
}}
