#include "bmp.h"
#include "stego.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void print_error(int error)
{
    switch (error)
    {
        case MEMORY_ALLOC_ERROR:
            printf("Memory allocation error\n");
            break;
        case FILE_OPEN_ERROR:
            printf("File open error\n");
            break;
        case NE_ARGS_ERROR:
            printf("Not enough arguments\n");
            break;
        case READING_ERROR:
            printf("Reading data error\n");
            break;
        case BIT_COUNT_ERROR:
            printf("Invalid bit per pixel count: expected 24\n");
            break;
        case BMP_FORMAT_ERROR:
            printf("Not little endian or not bmp format\n");
            break;
        case BOARDER_ERROR:
            printf("Invalid boarder size\n");
            break;
        case OPERATION_ERROR:
            printf("Unknown operation\n");
            break;
        case INSERT_ERROR:
            printf("Inserting error\n");
            break;
        case EXTRACT_ERROR:
            printf("Extracting error\n");
            break;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        print_error(NE_ARGS_ERROR);
        return 1;
    }

    char *function = argv[1];
    char *in_file  = argv[2];
    bmp_file image = {0};

    if (strcmp(function, "crop-rotate") == 0)
    {
        if (argc != 8)
        {
            print_error(NE_ARGS_ERROR);
            return 1;
        }

        char *out_file = argv[3];
        int pos_x  = atoi(argv[4]);
        int pos_y  = atoi(argv[5]);
        int crop_w = atoi(argv[6]);
        int crop_h = atoi(argv[7]);

        int func_result;

        if ((func_result = load_bmp(&image, in_file)) != SUCCESS)
        {
            print_error(func_result);
            return 1;
        }

        if (pos_x < 0 || pos_x + crop_w > image.info_header.width || pos_y < 0 || pos_y + crop_h > image.info_header.height)
        {
            print_error(BOARDER_ERROR);
            close_bmp(&image);
            return 1;
        }

        if ((func_result = crop(&image, pos_x, pos_y, crop_w, crop_h)) != SUCCESS)
        {
            print_error(func_result);
            close_bmp(&image);
            return 1;
        }

        if ((func_result = rotate(&image)) != SUCCESS)
        {
            print_error(func_result);
            close_bmp(&image);
            return 1;
        }

        if ((func_result = save_bmp(&image, out_file)) != SUCCESS)
        {
            print_error(func_result);
            close_bmp(&image);
            return 1;
        }

        close_bmp(&image);
    }
    else if (strcmp(function, "insert") == 0)
    {
        if (argc != 6)
        {
            print_error(NE_ARGS_ERROR);
            return 1;
        }

        char *out_file = argv[3];
        char *key_file = argv[4];
        char *message_file = argv[5];

        int func_result;

        if ((func_result = load_bmp(&image, in_file)) != SUCCESS)
        {
            print_error(func_result);
            return 1;
        }

        if ((func_result = insert_message(&image, key_file, message_file)) != SUCCESS)
        {
            print_error(func_result);
            close_bmp(&image);
            return 1;
        }

        if ((func_result = save_bmp(&image, out_file)) != SUCCESS)
        {
            print_error(func_result);
            close_bmp(&image);
            return 1;
        }

        close_bmp(&image);
    }
    else if (strcmp(function, "extract") == 0)
    {
        if (argc != 5)
        {
            print_error(NE_ARGS_ERROR);
            return 1;
        }

        char *key_file = argv[3];
        char *message_file = argv[4];

        int func_result;

        if ((func_result = load_bmp(&image, in_file)) != SUCCESS)
        {
            print_error(func_result);
            return 1;
        }

        if ((func_result = extract_message(&image, key_file, message_file)) != SUCCESS)
        {
            print_error(func_result);
            close_bmp(&image);
            return 1;
        }

        close_bmp(&image);
    }
    else
    {
        print_error(OPERATION_ERROR);
        return 1;
    }

    return 0;
}
