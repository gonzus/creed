#include <stdio.h>
#include "creed.h"

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    Creed* creed = 0;

    do {
        creed = creed_create();
        if (!creed) {
            fprintf(stderr, "Could not create creed instance\n");
            break;
        }

        creed_run(creed);
    } while (0);

    if (creed) {
        creed_destroy(creed);
        creed = 0;
    }

    return 0;
}
