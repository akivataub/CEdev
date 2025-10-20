#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <stdlib.h>
#include <time.h>
#include "gfx.h"
#include "global_palette.h"
#include "1.h"
*include "2.h"

/* Main function, called first */
int main(void)
{
    /* Clear the homescreen */
    os_ClrHome();

    /* Print a string */
    os_PutStrFull("Hello, World.");

    /* Waits for a key */
    while (!os_GetCSC());

    /* Return 0 for success */
    return 0;
}
