#include "bmp.h"

#include <stdlib.h>
#include <stdio.h>

const size_t bmp_file_header_size = 14;
const size_t bmp_info_header_size = 40;

static void skip_bytes(FILE *F, int padding)
{
    fseek(F, padding, SEEK_CUR);
}

static void zero_bytes(FILE *F, int padding)
{
    static int zero = 0;

    fwrite(&zero, padding, 1, F);
}

static void calculate_size(bmp_file *bmp, int pixel_size)
{
    int padding = (4 - (bmp->info_header.width * pixel_size) % 4) % 4;

    bmp->info_header.size_image = (bmp->info_header.width * pixel_size + padding) * bmp->info_header.height;
    bmp->file_header.size = bmp_info_header_size + bmp_file_header_size + bmp->info_header.size_image;
}

int load_bmp(bmp_file *bmp, char *filename)
{
    FILE *F;

    if ((F = fopen(filename, "rb")) == NULL)
        return 0;

    fread(&bmp->file_header, bmp_file_header_size, 1, F);
    fread(&bmp->info_header, bmp_info_header_size, 1, F);

    int pixel_size = bmp->info_header.bit_count / 8;

    if ((bmp->bytes = malloc(bmp->info_header.width * bmp->info_header.height * pixel_size)) == NULL)
    {
        fclose(F);
        return 0;
    }

    fseek(F, bmp->file_header.offset, SEEK_SET);

    int padding = (4 - (bmp->info_header.width * pixel_size) % 4) % 4;
    unsigned char *bytes_pointer = bmp->bytes;

    /*for (int h = 0; h < bmp->info_header.height; h++)
    {
        fread(bytes_pointer, pixel_size, bmp->info_header.width, F);

        if (padding != 0)
            skip_bytes(F, padding, pixel_size);

        bytes_pointer += pixel_size * bmp->info_header.width;
    }*/

    bytes_pointer = bmp->bytes + pixel_size * bmp->info_header.width * (bmp->info_header.height - 1);
    for (int h = 0; h < bmp->info_header.height; h++)
    {
        fread(bytes_pointer, pixel_size, bmp->info_header.width, F);

        if (padding != 0)
            skip_bytes(F, padding);

        bytes_pointer -= pixel_size * bmp->info_header.width;
    }

    fclose(F);

    return 1;
}


int save_bmp(bmp_file *bmp, char *filename)
{
    FILE *F;

    if ((F = fopen(filename, "wb")) == NULL)
        return 0;

    fwrite(&bmp->file_header, bmp_file_header_size, 1, F);
    fwrite(&bmp->info_header, bmp_info_header_size, 1, F);

    int pixel_size = bmp->info_header.bit_count / 8;
    int padding = (4 - (bmp->info_header.width * pixel_size) % 4) % 4;
    unsigned char *bytes_pointer = bmp->bytes;

    /*for (int h = 0; h < bmp->info_header.height; h++)
    {
        fwrite(bytes_pointer, pixel_size, bmp->info_header.width, F);

        if (padding != 0)
            zero_bytes(F, padding, pixel_size);

        bytes_pointer += pixel_size * bmp->info_header.width;
    }*/

    bytes_pointer = bmp->bytes + pixel_size * bmp->info_header.width * (bmp->info_header.height - 1);

    for (int h = 0; h < bmp->info_header.height; h++)
    {
        fwrite(bytes_pointer, pixel_size, bmp->info_header.width, F);

        if (padding != 0)
            zero_bytes(F, padding);

        bytes_pointer -= pixel_size * bmp->info_header.width;
    }

    fclose(F);

    return 1;
}

int crop(bmp_file *bmp, int x, int y, int w, int h)
{
    unsigned char *tmp_bytes;
    int pixel_size = bmp->info_header.bit_count / 8;
    int y_pos = y * pixel_size;
    int x_pos = x * pixel_size;

    if ((tmp_bytes = malloc(w * h * pixel_size)) == NULL)
        return 0;

    for (int i = 0; i < h * pixel_size; i += pixel_size)
        for (int j = 0; j < w * pixel_size; j += pixel_size)
            for (int k = 0; k < pixel_size; k++)
                tmp_bytes[i * w + j + k] = bmp->bytes[(y_pos + i) * bmp->info_header.width + (x_pos + j) + k];

    bmp->info_header.width = w;
    bmp->info_header.height = h;

    calculate_size(bmp, pixel_size);

    free(bmp->bytes);
    bmp->bytes = tmp_bytes;

    return 1;
}

int rotate(bmp_file *bmp)
{
    unsigned char *tmp_bytes;
    int pixel_size = bmp->info_header.bit_count / 8;
    int w = bmp->info_header.width;
    int h = bmp->info_header.height;

    if ((tmp_bytes = malloc(w * h * pixel_size)) == NULL)
        return 0;

    for (int j = 0; j < w * pixel_size; j += pixel_size)
        for (int i = 0; i < h * pixel_size; i += pixel_size)
            for (int k = 0; k < pixel_size; k++)
                tmp_bytes[(j + pixel_size) * h - (i + pixel_size) + k] = bmp->bytes[i * w + j + k];

    free(bmp->bytes);
    bmp->bytes = tmp_bytes;

    bmp->info_header.width = h;
    bmp->info_header.height = w;
    calculate_size(bmp, pixel_size);

    return 1;
}

void close_bmp(bmp_file *bmp)
{
    free(bmp->bytes);
}
