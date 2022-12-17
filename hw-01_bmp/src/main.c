#include "bmp.h"

#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 8)
        return 1;

    char *function = argv[1];
    char *in_file  = argv[2];
    char *out_file = argv[3];

    int
        pos_x  = atoi(argv[4]),
        pos_y  = atoi(argv[5]),
        crop_w = atoi(argv[6]),
        crop_h = atoi(argv[7]);

    if (strcmp(function, "crop-rotate") == 0)
    {
        bmp_file image = {0};

        if (load_bmp(&image, in_file) == 0)
            return 1;

        if (crop(&image, pos_x, pos_y, crop_w, crop_h) == 0)
        {
            close_bmp(&image);
            return 1;
        }

        if (rotate(&image) == 0)
        {
            close_bmp(&image);
            return 1;
        }

        if (save_bmp(&image, out_file) == 0)
        {
            close_bmp(&image);
            return 1;
        }

        close_bmp(&f);
    }

    return 0;
}