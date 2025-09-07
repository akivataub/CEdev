#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <string.h>

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

int main(void) {
    // Initialize random seed
    srand(rtc_Time());

    // Initialize graphics
    gfx_Begin();
    gfx_FillScreen(255); // White background
    gfx_SetTextFGColor(0); // Black text

    // Display welcome message
    gfx_PrintStringXY("Hello, User Welcome To RNG!", 50, 100);
    gfx_PrintStringXY("Stands For Random Number Guesser", 50, 120);
    delay(2000);

    while (1) {
        // Generate random number
        int n = randInt(1, 100);
        guess = 0;
        digits = 0;

        // Prompt for game start
        gfx_FillScreen(255);
        gfx_PrintStringXY("Press any key to start", 50, 140);
        while (!os_GetCSC()); // Wait for any key press

        while (1) {
            gfx_PrintStringXY("Guess the number (1-100):", 50, 100);

            // Display current guess
            char buf[8];
            itoa(guess, buf);
            gfx_PrintStringXY("Your guess: ", 50, 120);
            gfx_PrintStringXY(buf, 150, 120);

            // Get key press
            uint8_t key = os_GetCSC();
            if (key) {
                if (key == sk_1) { guess = guess * 10 + 1; digits++; gfx_FillScreen(255);}
                else if (key == sk_2) { guess = guess * 10 + 2; digits++; gfx_FillScreen(255);}
                else if (key == sk_3) { guess = guess * 10 + 3; digits++; gfx_FillScreen(255);}
                else if (key == sk_4) { guess = guess * 10 + 4; digits++; gfx_FillScreen(255);}
                else if (key == sk_5) { guess = guess * 10 + 5; digits++; gfx_FillScreen(255);}
                else if (key == sk_6) { guess = guess * 10 + 6; digits++; gfx_FillScreen(255);}
                else if (key == sk_7) { guess = guess * 10 + 7; digits++; gfx_FillScreen(255);}
                else if (key == sk_8) { guess = guess * 10 + 8; digits++; gfx_FillScreen(255);}
                else if (key == sk_9) { guess = guess * 10 + 9; digits++; gfx_FillScreen(255);}
                else if (key == sk_0) { guess = guess * 10; digits++; gfx_FillScreen(255);}
                else if (key == sk_Del) { guess /= 10; digits--; gfx_FillScreen(255);}
                else if (key == sk_Enter && digits > 0) {
                    gfx_FillScreen(255);
                    if (guess < n) {
                        gfx_PrintStringXY("Too Low!", 50, 160);            guess = 0;
                        guess = 0;

                    } else if (guess > n) {
                        gfx_PrintStringXY("Too High!", 50, 160);
                        guess = 0;

                    } else {
                        gfx_PrintStringXY("You Got It!!", 50, 160);
                        gfx_PrintStringXY("Press [2nd] to play again", 50, 180);
                        gfx_PrintStringXY("Press [mode] to exit", 50, 200);
                        while (1) {
                            key = os_GetCSC();
                            if (key == sk_2nd) {
                                break;
                                guess = 0;
                            } else if (key == sk_Mode) {
                                gfx_End();
                                return 0;
                            }
                        }
                        break;
                    }
                    delay(2000);
                } else if (key == sk_Mode) {
                    gfx_End();
                    return 0;
                }
            }
        }
    }

    gfx_End();
    return 0;
}