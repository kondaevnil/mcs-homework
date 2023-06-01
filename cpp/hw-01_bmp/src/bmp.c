#include "bmp.h"

#include <stdlib.h>
#include <stdio.h>

#define BMP_LE 0x4D42

const size_t bmp_file_header_size = 14;
const size_t bmp_info_header_size = 40;

static void skip_bytes(FILE *file, int padding)
{
    fseek(file, padding, SEEK_CUR);
}

static void zero_bytes(FILE *file, int padding)
{
    static int zero = 0;

    fwrite(&zero, padding, 1, file);
}

static void calculate_size(bmp_file *bmp, int pixel_size)
{
    int padding = (4 - (bmp->info_header.width * pixel_size) % 4) % 4;

    bmp->info_header.size_image = (bmp->info_header.width * pixel_size + padding) * bmp->info_header.height;
    bmp->file_header.size = bmp_info_header_size + bmp_file_header_size + bmp->info_header.size_image;
}

int load_bmp(bmp_file *bmp, char *filename)
{
    FILE *file;

    if ((file = fopen(filename, "rb")) == NULL)
        return FILE_OPEN_ERROR;

    if (fread(&bmp->file_header, bmp_file_header_size, 1, file) != 1)
    {
        fclose(file);
        return READING_ERROR;
    }

    if (fread(&bmp->info_header, bmp_info_header_size, 1, file) != 1)
    {
        fclose(file);
        return READING_ERROR;
    }

    if (bmp->file_header.type != BMP_LE)
    {
        fclose(file);
        return BMP_FORMAT_ERROR;
    }

    if (bmp->info_header.bit_count != 24)
    {
        fclose(file);
        return BIT_COUNT_ERROR;
    }

    int pixel_size = bmp->info_header.bit_count / 8;

    if ((bmp->bytes = malloc(bmp->info_header.width * bmp->info_header.height * pixel_size)) == NULL)
    {
        fclose(file);
        return MEMORY_ALLOC_ERROR;
    }

    fseek(file, bmp->file_header.offset, SEEK_SET);

    int padding = (4 - (bmp->info_header.width * pixel_size) % 4) % 4;
    int reading_format = bmp->info_header.height > 0;

    unsigned char *bytes_pointer = bmp->bytes;
    int shift = pixel_size * bmp->info_header.width;

    if (reading_format >= 0)
    {
        bytes_pointer += pixel_size * bmp->info_header.width * (bmp->info_header.height - 1);
        shift = -shift;
    }

    for (int h = 0; h < bmp->info_header.height; h++)
    {
        fread(bytes_pointer, pixel_size, bmp->info_header.width, file);

        if (padding != 0)
            skip_bytes(file, padding);

        bytes_pointer += shift;
    }

    fclose(file);

    return SUCCESS;
}


int save_bmp(bmp_file *bmp, char *filename)
{
    FILE *file;

    if ((file = fopen(filename, "wb")) == NULL)
        return FILE_OPEN_ERROR;

    fwrite(&bmp->file_header, bmp_file_header_size, 1, file);
    fwrite(&bmp->info_header, bmp_info_header_size, 1, file);

    int pixel_size = bmp->info_header.bit_count / 8;

    int reading_format = bmp->info_header.height > 0;
    int padding = (4 - (bmp->info_header.width * pixel_size) % 4) % 4;

    unsigned char *bytes_pointer = bmp->bytes;
    int shift = pixel_size * bmp->info_header.width;

    if (reading_format >= 0)
    {
        bytes_pointer += pixel_size * bmp->info_header.width * (bmp->info_header.height - 1);
        shift = -shift;
    }

    for (int h = 0; h < bmp->info_header.height; h++)
    {
        fwrite(bytes_pointer, pixel_size, bmp->info_header.width, file);

        if (padding != 0)
            zero_bytes(file, padding);

        bytes_pointer += shift;
    }

    fclose(file);

    return SUCCESS;
}

int crop(bmp_file *bmp, int x, int y, int w, int h)
{
    unsigned char *tmp_bytes;
    int pixel_size = bmp->info_header.bit_count / 8;
    int y_pos = y * pixel_size;
    int x_pos = x * pixel_size;

    if ((tmp_bytes = malloc(w * h * pixel_size)) == NULL)
        return MEMORY_ALLOC_ERROR;

    for (int i = 0; i < h * pixel_size; i += pixel_size)
    {
        for (int j = 0; j < w * pixel_size; j += pixel_size)
        {
            for (int k = 0; k < pixel_size; k++)
                tmp_bytes[i * w + j + k] = bmp->bytes[(y_pos + i) * bmp->info_header.width + (x_pos + j) + k];
        }
    }

    bmp->info_header.width = w;
    bmp->info_header.height = h;

    calculate_size(bmp, pixel_size);

    free(bmp->bytes);
    bmp->bytes = tmp_bytes;

    return SUCCESS;
}

int rotate(bmp_file *bmp)
{
    unsigned char *tmp_bytes;
    int pixel_size = bmp->info_header.bit_count / 8;
    int w = bmp->info_header.width;
    int h = bmp->info_header.height;

    if ((tmp_bytes = malloc(w * h * pixel_size)) == NULL)
        return MEMORY_ALLOC_ERROR;

    for (int j = 0; j < w * pixel_size; j += pixel_size)
    {
        for (int i = 0; i < h * pixel_size; i += pixel_size)
        {
            for (int k = 0; k < pixel_size; k++)
                tmp_bytes[(j + pixel_size) * h - (i + pixel_size) + k] = bmp->bytes[i * w + j + k];
        }
    }

    free(bmp->bytes);
    bmp->bytes = tmp_bytes;

    bmp->info_header.width = h;
    bmp->info_header.height = w;
    calculate_size(bmp, pixel_size);

    return SUCCESS;
}

void close_bmp(bmp_file *bmp)
{
    free(bmp->bytes);
}
